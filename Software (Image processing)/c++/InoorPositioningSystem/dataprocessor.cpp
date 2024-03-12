#include "dataprocessor.h"

DataProcessor::DataProcessor(ThreadSafeQueue& frameQueue): frameQueue(frameQueue) {}
DataProcessor::~DataProcessor() {
    if (uwbDataFile.is_open()){
        uwbDataFile.close();
    }

    if (videoDataFile.is_open()) {
        videoDataFile.close();
    }
}

void DataProcessor::loadData(const QString& UWBDataFilename, const QString& videoDataFilename) {
    videoDataFile = std::ifstream(videoDataFilename.toStdString());

    // timestampsVector.erase(timestampsVector.begin(), timestampsVector.end());
    timestampsVector.clear();

    int id;
    long long timestamp;
    while (videoDataFile >> id >> timestamp)
    {
        // assuming data are recorded sequentially and no intermidiate data is missing
        timestampsVector.push_back(timestamp);
    }

    uwbDataFile = std::ifstream(UWBDataFilename.toStdString());
    UWBData record;
    std::string line;
    Anchor anchor;
    // uwbDataVector.erase(uwbDataVector.begin(), uwbDataVector.end());
    uwbDataVector.clear();
    uwbDataPerTag.clear();
    while (std::getline(uwbDataFile, line, '\n'))
    {
        std::istringstream ss(line);

        ss >> record.id >> record.timestamp >> record.tagID;

        record.anchorList.clear();
        while (ss >> anchor.anchorID >> anchor.distance)
        {
            record.anchorList.push_back(anchor);
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

void DataProcessor::findUWBMeasurementAndEnqueue(int frameIndex, QImage qImage) {

    long long frameTimestamp = timestampsVector[frameIndex - 1];

    // qDebug() << "Searching for UWB data...";

    VideoData videoData(frameIndex, std::move(qImage), frameTimestamp);

    std::vector<UWBData> closestForEachTag;
    for (auto& data: uwbDataPerTag) {
        UWBData closestUWB = binarySearchUWB(frameTimestamp, data.second);
        // Calculating coordinates here and not when data are read, because data then can be adjusted
        calculateUWBCoordinates(closestUWB);
        closestForEachTag.push_back(closestUWB);
    }


    // it is better to make a copy of UWB Data and then move it to the queue rather than push a pointer to existing array, just in case UWBData array will be deleted.
    UWBVideoData uwbVideoData(std::move(videoData), std::move(closestForEachTag));
    // uwbVideoDataVector.push_back(uwbVideoData);


    frameQueue.enqueue(std::move(uwbVideoData));

}

void DataProcessor::calculateUWBCoordinates(UWBData& tag) {

    // As for now, assuming only two anchors 101 and 102. Anchor 102 has coordinates (0, 0) in UWB coordinate system, Anchor 101 has coordinates (2.5, 0) in UWB corrdinate system
    QPointF anchor101Coordinates(1, 0);
    QPointF anchor102Coordinates(3.5, 0);

    double anchorBaseline = std::abs(anchor101Coordinates.x() - anchor102Coordinates.x());

    double distanceAnchor101, distanceAnchor102;
    for (const Anchor& anchor: tag.anchorList) {
        if (anchor.anchorID == 101) {
            distanceAnchor101 = anchor.distance;
        } else if (anchor.anchorID == 102) {
            distanceAnchor102 = anchor.distance;
        }
    }

    double x = std::abs((std::pow(distanceAnchor101, 2) - std::pow(distanceAnchor102, 2) + std::pow(anchorBaseline, 2)) / (2 * anchorBaseline));
    double y = std::sqrt(std::pow(distanceAnchor101, 2) - std::pow(x, 2));

    tag.coordinates.setX(x + anchor101Coordinates.x()); // Transform x-coordinate to camera/world coordinate system
    tag.coordinates.setY(y); //TODO: to add offset
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

void DataProcessor::setRangeForDataAnalysis(const long long startFrameIndex, const long long endFrameIndex) {

    long long startFrameTimestamp = timestampsVector[startFrameIndex];
    long long endFrameTimestamp = timestampsVector[endFrameIndex];

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
                timestampsToAnalyze.push_back(data->timestamp);
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
    // datasetSegmentMeans.erase(datasetSegmentMeans.begin(), datasetSegmentMeans.end());
    datasetSegmentMeans.clear();

    // Detect segments where tag was standing and calculate mean for each segment
    // Assuming segments are continuous
    double segmentSum = 0.0;
    int segmentSize = 0;
    for (int i = 0; i < rollingDeviations.size(); ++i) {
        if (rollingDeviations[i] <= threshold) {
            segmentSum += *distancesToAnalyzeOriginal[i];
            segmentSize += 1;
        } else if (segmentSum > 0 && segmentSize > 0) {
            double temp = segmentSum / segmentSize;
            datasetSegmentMeans.push_back(temp);
            segmentSum = 0;
            segmentSize = 0;
        }
    }

    if (segmentSum > 0 && segmentSize > 0) {
        double temp = segmentSum / segmentSize;
        datasetSegmentMeans.push_back(temp);
        segmentSum = 0;
        segmentSize = 0;
    }

    emit requestShowDatasetSegments(datasetSegmentMeans);
}

void DataProcessor::calculatePolynomialRegression(const std::vector<double>& referenceValues) {
    distancesToAnalyzeAdjusted.clear();

    int n = referenceValues.size();

    Eigen::MatrixXd A(n, 3);
    Eigen::VectorXd y(n);

    for (int i = 0; i < n; ++i) {
        A(i, 0) = 1;
        A(i, 1) = datasetSegmentMeans[i];
        A(i, 2) = std::pow(datasetSegmentMeans[i], 2);
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
