#include "dataprocessor.h"

DataProcessor::DataProcessor(ThreadSafeQueue& frameQueue): frameQueue(frameQueue) {

    dataProcessorThread.reset(new QThread);
    moveToThread(dataProcessorThread.get());

    dataProcessorThread->start();

    segmentFrameIDs = {};
    segmentRepresentatives = {};

}

DataProcessor::~DataProcessor() {
    // cleanup();
    QMetaObject::invokeMethod(this, "cleanup");
    dataProcessorThread->wait();
}

void DataProcessor::cleanup() {
    if (uwbDataFile.is_open()){
        uwbDataFile.close();
    }

    if (videoDataFile.is_open()) {
        videoDataFile.close();
    }
    dataProcessorThread->quit();
}

void DataProcessor::loadData(const std::string& UWBDataFilename, const std::string& videoDataFilename) {
    videoDataFile = std::ifstream(videoDataFilename);
    videoTimestampsVector.clear();

    int id;
    long long timestamp;
    while (videoDataFile >> id >> timestamp)
    {
        // assuming data are recorded sequentially and no intermidiate data is missing
        videoTimestampsVector.push_back(timestamp);
    }

    uwbDataFile = std::ifstream(UWBDataFilename);
    UWBData record;
    std::string line;
    Anchor anchor;
    uwbDataVector.clear();
    uwbDataPerTag.clear();
    while (std::getline(uwbDataFile, line, '\n'))
    {
        std::istringstream ss(line);

        ss >> record.id >> record.timestamp >> record.tagID; // guaranteed to be present


        int i = 0;
        record.anchorList.clear();
        for (int i = 0; i < 2; i++) // guaranteed to be 2. It is possible to use while when more anchors found. This exact number is need for time measurement variable
        {
            ss >> anchor.anchorID >> anchor.distance;
            record.anchorList.push_back(anchor);
        }

        double temp;
        if (ss.rdbuf()->in_avail()) {
            ss >> temp;
            record.measurementTime = temp;
        } else {
            record.measurementTime = std::nullopt;
        }

        uwbDataVector.push_back(record);

    }

    for (UWBData& data: uwbDataVector) {
        uwbDataPerTag[data.tagID].push_back(&data); //Add Pointer to uwbDataVector
    }

    // qDebug() << "Data are loaded";


    videoDataFile.close();
    uwbDataFile.close();
}

long long DataProcessor::getVideoTimestampById(int id) {
    return videoTimestampsVector[id];
}

int DataProcessor::getTotalFrames() {
    return videoTimestampsVector.size();
}



void DataProcessor::onFindUWBMeasurementAndEnqueue(int frameIndex, QImage qImage, std::vector<QPointF> pixelToRealCoordinates, std::vector<QPointF> opticalCoordinates) {

    long long frameTimestamp = videoTimestampsVector[frameIndex - 1];

    // qDebug() << "Searching for UWB data...";

    VideoData videoData(frameIndex, std::move(qImage), frameTimestamp);
    // qDebug() << "Frame id: " << frameIndex;

    std::vector<UWBData> closestForEachTag;
    for (auto& data: uwbDataPerTag) {
        UWBData closestUWB = binarySearchUWB(frameTimestamp, data.second);
        // Calculating coordinates here and not when data are read, because data can be updated in the future
        calculateUWBCoordinates(closestUWB);
        closestForEachTag.push_back(closestUWB);
    }


    // it is better to make a copy of UWB Data and then move it to the queue rather than push a pointer to existing array, just in case UWBData array will be deleted.
    UWBVideoData uwbVideoData(std::move(videoData), std::move(closestForEachTag), std::move(pixelToRealCoordinates), std::move(opticalCoordinates));

    frameQueue.enqueue(std::move(uwbVideoData));

}

void DataProcessor::onFindUWBMeasurementAndExport(int frameIndex, int rangeIndex, ExportType exportType, const std::vector<DetectionResult>& detectionsVector, bool lastRecord) {

    long long frameTimestamp = videoTimestampsVector[frameIndex - 1];
    std::string outputFilePath = "uwb_to_bb_mapping.txt";

    if (!outputFile.is_open()) {
        outputFile = std::ofstream(outputFilePath, std::ios::out);
    }

    if (exportType == ExportType::FrameByFrameExport) {
        auto data = uwbDataPerTag.begin();
        // Vector of bottom edge centers is used for the future improvement of the code. When more people are used for model calibration
        // As for now only one person in the scene assumed
        for (int i = 0; i < detectionsVector.size(); ++i) {
            if (data != uwbDataPerTag.end()) {
                UWBData closestUWB = binarySearchUWB(frameTimestamp, data->second);
                calculateUWBCoordinates(closestUWB);
                outputFile << frameIndex << " " << closestUWB.coordinates.x() << " " << closestUWB.coordinates.y() << " " << detectionsVector[i].bottomEdgeCenter.x() << " " << detectionsVector[i].bottomEdgeCenter.y() << std::endl;
                ++data;
            } else {
                // qDebug() << "Intruder is found!! There is no tag to match with people";
            }
        }
    // Always only for one tag for which data analysis is performed. Frame timestamp and UWBData are already known and synchornized. Synzhronization was done during split of dataset into segments
    // Assuming only one person in the scene. Otherwise need to solve synchronization between indexes of detected people and indexes of the tags they are wearing
    } else if (exportType == ExportType::SegmentFramesExport) {
        double segmentDistanceSum = 0.0;
        UWBData* data;
        for (Anchor& segmentRepresentativeAnchor: segmentRepresentatives[rangeIndex].anchorList) {
            for (int i = 0; i < segmentSizes[rangeIndex]; ++i) {
                int middleIdx = segmentSizes[rangeIndex] / 2;
                data = &uwbDataVector[segmentRepresentatives[rangeIndex].id - middleIdx + i]; // assuming id of record corresponds to index of vector -1 position. Seeking the first record of range
                std::vector<Anchor>::iterator anchor = std::find_if(data->anchorList.begin(), data->anchorList.end(), [segmentRepresentativeAnchor](const Anchor& anchor2) {
                    return anchor2.anchorID == segmentRepresentativeAnchor.anchorID;
                });
                if (anchor != data->anchorList.end()) {
                    segmentDistanceSum += anchor->distance;
                } else {
                    int j = 0;
                    j++;
                }
                // ++data;
            }
            double distanceMean = segmentDistanceSum / segmentSizes[rangeIndex];
            segmentRepresentativeAnchor.distance = distanceMean;
            segmentDistanceSum = 0.0;
        }

        calculateUWBCoordinates(segmentRepresentatives[rangeIndex]);
        outputFile << frameIndex << " " << segmentRepresentatives[rangeIndex].coordinates.x() << " " << segmentRepresentatives[rangeIndex].coordinates.y() << " " << detectionsVector[0].bottomEdgeCenter.x() << " " << detectionsVector[0].bottomEdgeCenter.y() << std::endl;
    }

    if (lastRecord && outputFile.is_open()) {
        segmentFrameIDs.clear();
        segmentSizes.clear();
        segmentRepresentatives.clear();
        outputFile.close();
    }

    emit exportProgressUpdated(rangeIndex);
}

void DataProcessor::calculateUWBCoordinates(UWBData& tag) {

    // // As for now, assuming only two anchors 101 and 102. Anchor 102 has coordinates (0, 0) in UWB coordinate system, Anchor 101 has coordinates (2.5, 0) in UWB corrdinate system
    // QPointF anchor101Coordinates(0.627, 0);
    // QPointF anchor102Coordinates(3.127, 0);
    // QPointF anchor103Coordinates(3.127, 15);
    // QPointF anchor104Coordinates(0.627, 15);

    // Triangulation
    QPointF anchor101Coordinates(3.127, 0);
    QPointF anchor102Coordinates(0.627, 0);
    QPointF anchor103Coordinates(0.627, 15);
    QPointF anchor104Coordinates(3.127, 15);

    QMap<int, QPointF> anchorCoordinates;
    anchorCoordinates[101] = anchor101Coordinates;
    anchorCoordinates[102] = anchor102Coordinates;
    anchorCoordinates[103] = anchor103Coordinates;
    anchorCoordinates[104] = anchor104Coordinates;

    double distanceAnchor1 = 0;
    double distanceAnchor2 = 0;
    int anchor1ID = 0, anchor2ID = 0;
    QPointF anchor1Coordinates;
    QPointF anchor2Coordinates;

    // Extract distances from the anchor list and find the pair of anchors
    for (const Anchor& anchor: tag.anchorList) {
        // qDebug() << "AnchorID: " << anchor.anchorID;
        if (anchorCoordinates.contains(anchor.anchorID)) {
            if (distanceAnchor1 == 0) {
                distanceAnchor1 = anchor.distance;
                anchor1Coordinates = anchorCoordinates[anchor.anchorID];
                anchor1ID = anchor.anchorID;
            } else if (distanceAnchor2 == 0) {
                distanceAnchor2 = anchor.distance;
                anchor2Coordinates = anchorCoordinates[anchor.anchorID];
                anchor2ID = anchor.anchorID;
                break; // We have found two distances, we can break out of the loop
            }
        }
    }

    double anchorBaseline = std::sqrt(std::pow(anchor1Coordinates.x() - anchor2Coordinates.x(), 2) +
                                      std::pow(anchor1Coordinates.y() - anchor2Coordinates.y(), 2));

    double x = 0, y = 0;

    if (anchor1ID == 103 || anchor1ID == 104) {
        x = std::abs((std::pow(distanceAnchor1, 2) - std::pow(distanceAnchor2, 2) + std::pow(anchorBaseline, 2)) / (2 * anchorBaseline));
        y = std::sqrt(std::pow(distanceAnchor1, 2) - std::pow(x, 2));
    } else if (anchor1ID == 101 || anchor1ID == 102) {
        x = std::abs((std::pow(distanceAnchor2, 2) - std::pow(distanceAnchor1, 2) + std::pow(anchorBaseline, 2)) / (2 * anchorBaseline));
        y = std::sqrt(std::pow(distanceAnchor2, 2) - std::pow(x, 2));
    }


    // }

    tag.coordinates.setX(x + 0.627); // Transform x-coordinate to camera/world coordinate system
    tag.coordinates.setY(std::abs(y - std::max(anchor1Coordinates.y(), anchor2Coordinates.y())));

    // qDebug() << "Tag ID: " << tag.tagID << "Distance 101: " << distanceAnchor1 << ", Distance 102: " << distanceAnchor2 << ";";
    // qDebug() << "Tag ID: " << tag.tagID << "Coordinates: (" << tag.coordinates.x() << ", " << tag.coordinates.y() << ");";

}


UWBData DataProcessor::linearSearchUWB(const long long &frameTimestamp) {
    UWBData* closestUWB = &uwbDataVector[0];
    long long minDif = std::abs(frameTimestamp - closestUWB->timestamp);

    for (auto &data : uwbDataVector)
    {
        long long dif = std::abs(frameTimestamp - data.timestamp);
        if (dif < minDif)
        {
            closestUWB = &data;
            minDif = dif;
        }
    }

    return *closestUWB;
}


UWBData DataProcessor::binarySearchUWB(const long long &frameTimestamp, const std::vector<UWBData*>& uwbDataVectorPtr) {
    int left = 0;
    int right = uwbDataVectorPtr.size() - 1;

    UWBData* closestUWB = uwbDataVectorPtr[0];
    long long minDif = std::abs(frameTimestamp - closestUWB->timestamp);
    int mid;
    long long dif;

    while (left <= right) {
        mid = left + (right - left) / 2;
        dif = std::abs(frameTimestamp - uwbDataVectorPtr[mid]->timestamp);

        if (dif < minDif) {
            minDif = dif;
            closestUWB = uwbDataVectorPtr[mid];
        }

        if (frameTimestamp > uwbDataVectorPtr[mid]->timestamp) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    return *closestUWB;
}

UWBData DataProcessor::binarySearchUWB(const long long &frameTimestamp) {
    int left = 0;
    int right = uwbDataVector.size() - 1;

    UWBData* closestUWB = &uwbDataVector[0];
    long long minDif = std::abs(frameTimestamp - closestUWB->timestamp);
    int mid;
    long long dif;

    while (left <= right) {
        mid = left + (right - left) / 2;
        dif = std::abs(frameTimestamp - uwbDataVector[mid].timestamp);

        if (dif < minDif) {
            minDif = dif;
            closestUWB = &uwbDataVector[mid];
        }

        if (frameTimestamp > uwbDataVector[mid].timestamp) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    return *closestUWB;
}

int DataProcessor::binarySearchVideoFrameID(const long long &uwbTimestamp) {
    int left = 0;
    int right = videoTimestampsVector.size() - 1;
    int closestID = -1;

    long long minDif = std::abs(uwbTimestamp - videoTimestampsVector[0]);
    int mid;
    long long dif;

    while (left <= right) {
        mid = left + (right - left) / 2;
        dif = std::abs(uwbTimestamp - videoTimestampsVector[mid]);

        if (dif < minDif) {
            minDif = dif;
            closestID = mid;
        }

        if (uwbTimestamp > videoTimestampsVector[mid]) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    return closestID;
}

void DataProcessor::setRangeForDataAnalysis(const long long startTimeSec, const long long endTimeSec) {

    long long startTimestampMS = startTimeSec * 1000 + videoTimestampsVector[0];
    long long endTimestampMS = endTimeSec * 1000 + videoTimestampsVector[0];

    int startFrameIndex = binarySearchVideoFrameID(startTimestampMS);
    int endFrameIndex = binarySearchVideoFrameID(endTimestampMS);

    long long startFrameTimestamp = videoTimestampsVector[startFrameIndex];
    long long endFrameTimestamp = videoTimestampsVector[endFrameIndex];

    // To not copy subarray of elements, it is possible to use span
    UWBData startUWB = binarySearchUWB(startFrameTimestamp);
    UWBData endUWB = binarySearchUWB(endFrameTimestamp);

    uwbDataRangeToAnalyze = std::span<UWBData>(uwbDataVector.begin() + startUWB.id - 1, uwbDataVector.begin() + endUWB.id - 1);

    // Extract unique Tag IDs
    uniqueTagIDs.reserve(uwbDataRangeToAnalyze.size());

    std::transform(uwbDataRangeToAnalyze.begin(), uwbDataRangeToAnalyze.end(), std::back_inserter(uniqueTagIDs), [](const UWBData& obj) { return obj.tagID; });
    std::sort(uniqueTagIDs.begin(), uniqueTagIDs.end());
    std::vector<int>::iterator last = std::unique(uniqueTagIDs.begin(), uniqueTagIDs.end());

    uniqueTagIDs.erase(last, uniqueTagIDs.end());

    emit requestShowAvailableTags(uniqueTagIDs);
}

// Selecting Tag data and then choosing necassery Anchor ID when plotting is faster, than everytime creating new array of pair Tag - Anchor
// Tag is selected every time combobox is changed
void DataProcessor::collectDataForTag(const QString &tagIDText) {
    bool converted;
    int tagID = tagIDText.toInt(&converted);

    // tagDataToAnalyze.erase(tagDataToAnalyze.begin(), tagDataToAnalyze.end());
    tagDataToAnalyze.clear();

    std::vector<int> availableAnchorsForTag;
    for (UWBData& data: uwbDataRangeToAnalyze) {
        if (data.tagID == tagID) {
            tagDataToAnalyze.push_back(&data);
            for (const Anchor& anchor : data.anchorList) {
                if (std::find(availableAnchorsForTag.begin(), availableAnchorsForTag.end(), anchor.anchorID) == availableAnchorsForTag.end()) {
                    availableAnchorsForTag.push_back(anchor.anchorID);
                }
            }
        }
    }
    std::sort(availableAnchorsForTag.begin(), availableAnchorsForTag.end());
    emit requestShowAvailableAnchors(availableAnchorsForTag);
}

// /* Data for anchor is collected only when Analyze button is pressed.
// *  The reason it is not like with tag, because whenever anchor is selected from combobox there is still possibility to choose a tag.
// *  So, when a new tag is chosen, tagDataToAnalyze is changed, but anchorDataToAnalyze stays old, because combobox for anchor was not changed (and corresponding signal was not triggered).
// *  Therefore it is better to call signal to collect anchor data when Analyze button is pressed.
// */
// void DataProcessor::collectDataForAnchor(const int anchorID) {

//     anchorDataToAnalyze.erase(anchorDataToAnalyze.begin(), anchorDataToAnalyze.end());

//     for (const UWBData& data: tagDataToAnalyze) {
//         for (const Anchor& anchor: data.anchorList) {
//             if (anchor.anchorID == anchorID) {
//                 anchorDataToAnalyze.push_back(anchor);
//             }
//         }
//     }
// }

void DataProcessor::collectDataForPlotDistancesVsTimestamps(const int anchorID) {
    // timestampsToAnalyze.erase(timestampsToAnalyze.begin(), timestampsToAnalyze.end());
    // distancesToAnalyzeOriginal.erase(distancesToAnalyzeOriginal.begin(), distancesToAnalyzeOriginal.end());

    timestampsToAnalyze.clear();
    distancesToAnalyzeOriginal.clear();
    for (UWBData* data: tagDataToAnalyze) {
        for (Anchor& anchor: data->anchorList) {
            if (anchor.anchorID == anchorID) {
                timestampsToAnalyze.push_back(data->timestamp - videoTimestampsVector[0]);
                distancesToAnalyzeOriginal.push_back(&(anchor.distance));
            }
        }
    }

    emit requestShowPlotDistancesVsTimestamps(timestampsToAnalyze, distancesToAnalyzeOriginal);

}

void DataProcessor::calculateRollingDeviation(const int windowSize) {
    // rollingDeviations.erase(rollingDeviations.begin(), rollingDeviations.end());
    rollingDeviations.clear();

    // Ensure to handle cases where there are not enough elements to form a full window at the end
    int n = distancesToAnalyzeOriginal.size();
    for (int i = 0; i < n; ++i) {
        // Calculate mean for the window considering available elements
        double sum = 0.0;
        int count = 0; // Count actual elements in the window
        for (int j = i; j < i + windowSize && j < n; ++j, ++count) {
            sum += *distancesToAnalyzeOriginal[j];
        }
        double mean = sum / count;

        // Calculate sum of squares of differences from the mean for the actual window
        double sumSquares = 0.0;
        for (int j = i; j < i + windowSize && j < n; ++j) {
            sumSquares += std::pow(*distancesToAnalyzeOriginal[j] - mean, 2);
        }

        // Calculate standard deviation for the window
        double deviation = std::sqrt(sumSquares / count);
        rollingDeviations.push_back(deviation);
    }

    emit requestShowPlotRollingDeviations(timestampsToAnalyze, rollingDeviations);
}

void DataProcessor::splitDataset(const double threshold) {
    // segmentMeans.erase(segmentMeans.begin(), segmentMeans.end());
    segmentMeans.clear();
    segmentFrameIDs.clear();
    segmentSizes.clear();
    segmentRepresentatives.clear();

    // Detect segments where tag was standing and calculate mean for each segment
    // Assuming segments are continuous
    double segmentDistanceSum = 0.0;
    int segmentSize = 0;
    int i;
    for (i = 0; i < rollingDeviations.size(); ++i) {
        if (rollingDeviations[i] <= threshold) {
            segmentDistanceSum += *distancesToAnalyzeOriginal[i];
            segmentSize += 1;
        } else if (segmentDistanceSum > 0 && segmentSize > 0) {
            double segmentMean = segmentDistanceSum / segmentSize;
            segmentMeans.push_back(segmentMean);
            int middleIdx = segmentSize / 2;
            UWBData segmentRepresentative = *tagDataToAnalyze[i - middleIdx];
            int segmentVideoFrameID = binarySearchVideoFrameID(segmentRepresentative.timestamp);
            segmentFrameIDs.push_back(segmentVideoFrameID); // remember timestamp of the segment's middle record for model training, first frame of the segment is not always good choice
            segmentSizes.push_back(segmentSize);
            segmentRepresentatives.push_back(std::move(segmentRepresentative));
            segmentDistanceSum = 0;
            segmentSize = 0;
        }
    }

    if (segmentDistanceSum > 0 && segmentSize > 0) {
        double segmentMean = segmentDistanceSum / segmentSize;
        segmentMeans.push_back(segmentMean);
        int middleIdx = segmentSize / 2;
        UWBData segmentRepresentative = *tagDataToAnalyze[i - middleIdx];
        int segmentVideoFrameID = binarySearchVideoFrameID(segmentRepresentative.timestamp);
        segmentFrameIDs.push_back(segmentVideoFrameID); // remember timestamp of the segment's middle record for model training, first frame of the segment is not always good choice
        segmentSizes.push_back(segmentSize);
        segmentRepresentatives.push_back(std::move(segmentRepresentative));
    }

    emit requestShowDatasetSegments(segmentMeans);
}

std::vector<int> DataProcessor::getSegmentFrameIDs() {
    return segmentFrameIDs;
}

void DataProcessor::calculatePolynomialRegression(const std::vector<double>& referenceValues) {
    distancesToAnalyzeAdjusted.clear();

    int n = referenceValues.size();

    Eigen::MatrixXd A(n, 3);
    Eigen::VectorXd y(n);

    for (int i = 0; i < n; ++i) {
        A(i, 0) = 1;
        A(i, 1) = segmentMeans[i];
        A(i, 2) = std::pow(segmentMeans[i], 2);
        y(i) = referenceValues[i];
    }

    Eigen::VectorXd betaCoeff = (A.transpose() * A).ldlt().solve(A.transpose() * y);

    for (size_t i = 0; i < distancesToAnalyzeOriginal.size(); ++i) {
        double distance = *(distancesToAnalyzeOriginal[i]);
        distancesToAnalyzeAdjusted.push_back(betaCoeff(0) + betaCoeff(1) * distance + betaCoeff(2) * distance * distance);
    }



    emit requestShowOriginalVsAdjustedDistances(timestampsToAnalyze, distancesToAnalyzeOriginal, distancesToAnalyzeAdjusted);
}

void DataProcessor::updateOriginalWithAdjustedValues() {
    for (int i = 0; i < distancesToAnalyzeAdjusted.size(); ++i) {
        *(distancesToAnalyzeOriginal[i]) = distancesToAnalyzeAdjusted[i];
    }
}
