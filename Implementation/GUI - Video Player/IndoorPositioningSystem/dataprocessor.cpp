#include "dataprocessor.h"

DataProcessor::DataProcessor(ThreadSafeQueue& frameQueue): frameQueue(frameQueue) {
    // Thread initiation
    dataProcessorThread.reset(new QThread);
    moveToThread(dataProcessorThread.get());
    dataProcessorThread->start();

    segmentFrameIDs = {};
    segmentRepresentatives = {};
}

DataProcessor::~DataProcessor() {
    // cleanup
    QMetaObject::invokeMethod(this, "cleanup");
    dataProcessorThread->wait();
}

void DataProcessor::cleanup() {
    // Close files
    if (uwbDataFile.is_open()){
        uwbDataFile.close();
    }
    if (videoDataFile.is_open()) {
        videoDataFile.close();
    }

    // Free XGBoost
    XGBoosterFree(booster);

    dataProcessorThread->quit();
}

//-------------------------------- Prepare data for processing --------------------------------
void DataProcessor::loadData(const std::string& folderName, const std::string& UWBDataFilename, const std::string& videoDataFilename) {

    projectFolderName = folderName;

    // Video
    videoDataFile = std::ifstream(videoDataFilename);

    int id;
    long long timestamp;
    videoTimestampsVector.clear();

    while (videoDataFile >> id >> timestamp)
    {
        // assuming data are recorded sequentially and no intermidiate data is missing
        videoTimestampsVector.push_back(timestamp);
    }

    // UWB
    uwbDataFile = std::ifstream(UWBDataFilename);

    UWBData record;
    std::string line;
    Anchor anchor;
    uwbDataVector.clear();
    uwbDataPerTag.clear();
    coordinateHistory.clear();

    while (std::getline(uwbDataFile, line, '\n'))
    {
        std::istringstream ss(line);

        ss >> record.id >> record.timestamp >> record.tagID; // guaranteed to be present

        int i = 0;
        record.anchorList.clear();

        // Guaranteed to be 2. This exact number is needed to read time of the request.
        // It is possible to extend for more anchors. However, tags collect data only from 2 anchors (necessary for triangulation).
        for (int i = 0; i < 2; i++)
        {
            ss >> anchor.anchorID >> anchor.distance;
            record.anchorList.push_back(anchor);
        }

        // Optional. Reading time of the request (if available in the data stream).
        double temp;
        if (ss.rdbuf()->in_avail()) {
            ss >> temp;
            record.measurementTime = temp;
        } else {
            record.measurementTime = std::nullopt;
        }

        // Store read data
        uwbDataVector.push_back(record);
    }

    // Add Pointer to uwbDataVector. Needed for "on-the-fly" data correction.
    // After correcting, data shown in the Video Player are atomatically adjusted.
    for (UWBData& data: uwbDataVector) {
        uwbDataPerTag[data.tagID].push_back(&data);
    }

    fileIncrementer = 1; // Needed for export of segments. Each per-segment output file will gain own ID.

    // Data are read. We can close sources.
    videoDataFile.close();
    uwbDataFile.close();
}

long long DataProcessor::getVideoTimestampById(int id) {
    return videoTimestampsVector[id];
}

int DataProcessor::getTotalFrames() {
    return videoTimestampsVector.size();
}

// ---------------- Video and UWB Data Syncrhonization ------------------------------------------------------------------------

// Find closest UWB for a given Frame ID (based on timestamp)
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

// Find Frame for a given UWB record (based on timestamp)
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

// For a given frame, find the closest UWB measurements for each observed person and enque to the ThreadSafeQueue for visualization in Video Player.
void DataProcessor::onFindUWBMeasurementAndEnqueue(int frameIndex, QImage qImage, DetectionData detectedPeople) {

    long long frameTimestamp = videoTimestampsVector[frameIndex - 1];

    VideoData videoData(frameIndex, std::move(qImage), frameTimestamp);

    std::vector<UWBData> closestForEachTag;
    for (auto& data: uwbDataPerTag) {
        UWBData closestUWB = binarySearchUWB(frameTimestamp, data.second);
        // Calculating coordinates here and not when data are read, because data can be updated in the future (e.g. UWB data correction)
        calculateUWBCoordinates(closestUWB);
        closestForEachTag.push_back(closestUWB);
    }

    QPointF pixelToRealCoordinates(0.0, 0.0);
    QPointF opticalCoordinates(0.0, 0.0);
    std::vector<QPointF> pixelToRealCoordinatesVector;
    std::vector<QPointF> opticalCoordinatesVector;

    // Calculate Pixel-to-Real and Optical coordinates if people were found.
    if (detectedPeople.detectionResults.size()) {
        for (const DetectionResult& detection: detectedPeople.detectionResults){
            if (isPredictionByPixelToRealRequested)
            {
                pixelToRealCoordinates = predictWorldCoordinatesPixelToReal(detection);
                pixelToRealCoordinatesVector.push_back(pixelToRealCoordinates);
            }

            if (isPredictionByOpticalRequested) {
                opticalCoordinates = predictWorldCoordinatesOptical(detection, detectedPeople.cameraFrameSize, detectedPeople.detectionFrameSize);
                opticalCoordinatesVector.push_back(opticalCoordinates);
            }
        }
    }


    // it is better to make a copy of UWB Data and then move it to the queue rather than push a pointer to existing array, just in case UWBData array will be deleted.
    UWBVideoData uwbVideoData(std::move(videoData), std::move(closestForEachTag), std::move(pixelToRealCoordinatesVector), std::move(opticalCoordinatesVector));

    frameQueue.enqueue(std::move(uwbVideoData));
}

/* ****************** onFindUWBMeasurementAndExport ************************
 * This function can either be used to export the UWB data for MODEL TRAINING, or export UWB, Pixel-to-Real and Optical data for COMPARISON
 * !!! By default, only export for Model Training is used.
 * !!! To export the UWB, Pixel-to-Real and Optical data for Comparison, please uncomment the corresponding parts.
 * Export for Data Comparison is NOT necessary for the deployemnt of our system (but is planned to be implemented in the future). Only export for Model Training is necessary.
 */

void DataProcessor::onFindUWBMeasurementAndExport(int frameIndex, int rangeIndex, ExportType exportType, const DetectionData& detectedPeople, bool lastRecord) {
    std::string exportDir = projectFolderName + "/export/";
    std::string outputFilePathUWB = exportDir + "uwb_to_bb_mapping_" + std::to_string(fileIncrementer) + ".txt";

    // Check if the directory exists, and create it if it doesn't
    if (!std::filesystem::exists(exportDir)) {
        std::filesystem::create_directories(exportDir);
    }

    if (!outputFileUWB.is_open()) {
        outputFileUWB = std::ofstream(outputFilePathUWB, std::ios::out);
    }

    // -------- Uncomment when exporting coordinates for each method -------
    // std::string outputFilePathPixelToReal = exportDir + "pixel_to_real_to_bb_mapping_" + std::to_string(fileIncrementer) + ".txt";
    // std::string outputFilePathOptical = exportDir + "optical_to_bb_mapping_" + std::to_string(fileIncrementer) + ".txt";

    // if (!outputFilePixelToReal.is_open()) {
    //     outputFilePixelToReal = std::ofstream(outputFilePathPixelToReal, std::ios::out);
    // }

    // if (!outputFileOptical.is_open()) {
    //     outputFileOptical = std::ofstream(outputFilePathOptical, std::ios::out);
    // }
    //------till here--------

    long long frameTimestamp = videoTimestampsVector[frameIndex - 1];

    // Frame-by-frame export. Takes very long time!!!
    if (exportType == ExportType::FrameByFrameExport) {
        auto data = uwbDataPerTag.begin();
        for (int i = 0; i < detectedPeople.detectionResults.size(); ++i) {
            if (data != uwbDataPerTag.end()) {
                UWBData closestUWB = binarySearchUWB(frameTimestamp, data->second);
                calculateUWBCoordinates(closestUWB);
                outputFileUWB << frameIndex << " " << closestUWB.coordinates.x() << " " << closestUWB.coordinates.y() << " " << detectedPeople.detectionResults[i].bottomEdgeCenter.x() << " " << detectedPeople.detectionResults[i].bottomEdgeCenter.y() << std::endl;
                ++data;
            } else {
                // Intruder is found!! There is no tag to match with people
            }
        }

    // Export of segment (standing periods) representatives for the currently analyzed Tag and Anchor pair. Very fast!!!
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
            }
            double distanceMean = segmentDistanceSum / segmentSizes[rangeIndex];
            segmentRepresentativeAnchor.distance = distanceMean;
            segmentDistanceSum = 0.0;
        }

        calculateUWBCoordinates(segmentRepresentatives[rangeIndex]);

        outputFileUWB << frameIndex << " ";

        // -------- Uncomment when exporting coordinates for each method -------
        // QPointF pixelToRealCoordinates(0.0, 0.0);
        // QPointF opticalCoordinates(0.0, 0.0);

        // outputFilePixelToReal << frameIndex << " ";
        // outputFileOptical << frameIndex << " ";
        // for (int i = 0; i < detectedPeople.detectionResults.size(); ++i) {
        //     outputFileUWB << detectedPeople.detectionResults[i].bottomEdgeCenter.x() << " " << detectedPeople.detectionResults[i].bottomEdgeCenter.y() << " ";

        //     pixelToRealCoordinates = predictWorldCoordinatesPixelToReal(detectedPeople.detectionResults[i]);
        //     opticalCoordinates = predictWorldCoordinatesOptical(detectedPeople.detectionResults[i], detectedPeople.cameraFrameSize, detectedPeople.detectionFrameSize);

        //     outputFilePixelToReal << detectedPeople.detectionResults[i].bottomEdgeCenter.x() << " " << detectedPeople.detectionResults[i].bottomEdgeCenter.y() << " ";
        //     outputFileOptical << detectedPeople.detectionResults[i].bottomEdgeCenter.x() << " " << detectedPeople.detectionResults[i].bottomEdgeCenter.y() << " ";

        //     outputFilePixelToReal << pixelToRealCoordinates.x() << " " << pixelToRealCoordinates.y() << " ";
        //     outputFileOptical << opticalCoordinates.x() << " " << opticalCoordinates.y() << " ";
        // }
        // outputFilePixelToReal << std::endl;
        // outputFileOptical << std::endl;
        // ---------till here---------

        // Then comment this, as it duplicates output of UWB coordinates
        for (int i = 0; i < detectedPeople.detectionResults.size(); ++i) {
            outputFileUWB << detectedPeople.detectionResults[i].bottomEdgeCenter.x() << " " << detectedPeople.detectionResults[i].bottomEdgeCenter.y() << " ";
        }
        // till here

        // This is necessary in both cases
        outputFileUWB << segmentRepresentatives[rangeIndex].coordinates.x() << " " << segmentRepresentatives[rangeIndex].coordinates.y() << std::endl;
    }

    if (lastRecord && (outputFileUWB.is_open() || outputFilePixelToReal.is_open() || outputFileOptical.is_open())) {
        fileIncrementer++;
        segmentFrameIDs.clear();
        segmentSizes.clear();
        segmentRepresentatives.clear();
        if (outputFileUWB.is_open()) outputFileUWB.close();
        if (outputFilePixelToReal.is_open()) outputFilePixelToReal.close();
        if (outputFileOptical.is_open()) outputFileOptical.close();
    }

    // Update Export Progress Dialog in main Video Player (GUI)
    emit exportProgressUpdated(rangeIndex);
}

// ---------------- Calculate Coordinates ------------------------------------------------------------------------

void DataProcessor::setAnchorPositions(std::vector<AnchorPosition> positions) {
    anchorPositions = positions;
}

void DataProcessor::calculateUWBCoordinates(UWBData& tag) {

    double distanceAnchor1 = 0;
    double distanceAnchor2 = 0;
    AnchorPosition anchor1Position, anchor2Position;

    // Find origin of the UWB coordinate system (it is set by User)
    auto found = std::find_if(anchorPositions.begin(), anchorPositions.end(), [](const AnchorPosition& pos) {
        return pos.isOrigin;
    });
    AnchorPosition origin = *found;


    // Select distances from the anchor list. We need to know anchor IDs and their positions.
    // We use 4 anchors to be able to walk abck and forth. Tag collects distances only from 2 anchors.
    for (const Anchor& anchor: tag.anchorList) {
        auto position = std::find_if(anchorPositions.begin(), anchorPositions.end(), [&anchor](const AnchorPosition& pos) {
            return pos.anchorID == anchor.anchorID;
        });

        if (position != anchorPositions.end()) {
            if (distanceAnchor1 == 0) {
                distanceAnchor1 = anchor.distance;
                anchor1Position = *position;
            } else if (distanceAnchor2 == 0) {
                distanceAnchor2 = anchor.distance;
                anchor2Position = *position;
                break; // We have found two distances, we can break the loop
            }
        }
    }

    // Apply Triangulation
    double anchorBaseline = std::sqrt(std::pow(anchor1Position.x - anchor2Position.x, 2) + std::pow(anchor1Position.y - anchor2Position.y, 2));

    double a = (std::pow(distanceAnchor1, 2) - std::pow(distanceAnchor2, 2) + std::pow(anchorBaseline, 2)) / (2 * anchorBaseline);
    double h = std::sqrt(std::pow(distanceAnchor1, 2) - std::pow(a, 2)); // could be negative due to descrepancies in measurements

    QPointF Ptemp, P1, P2;
    Ptemp.setX(anchor1Position.x + a * (anchor2Position.x - anchor1Position.x) / anchorBaseline);
    Ptemp.setY(anchor1Position.y + a * (anchor2Position.y - anchor1Position.y) / anchorBaseline);

    P1.setX(Ptemp.x() - h * (anchor2Position.y - anchor1Position.y) / anchorBaseline);
    P1.setY(Ptemp.y() + h * (anchor2Position.x - anchor1Position.x) / anchorBaseline);
    P2.setX(Ptemp.x() + h * (anchor2Position.y - anchor1Position.y) / anchorBaseline);
    P2.setY(Ptemp.y() - h * (anchor2Position.x - anchor1Position.x) / anchorBaseline);



    /*
     * Iterate over pairs of discovered anchors
     * Triangulation gives two points of intersection.
     * Assuming that all anchor positions are known
    */

    // 1st case: Assume that anchors are aligned together along the small edge of the rectangular area created by 4 anchors
    // This allows to choose only one correct coordinate
    if ((anchor1Position.anchorID == 101 && anchor2Position.anchorID == 102) || (anchor1Position.anchorID == 102 && anchor2Position.anchorID == 101)) {
        if (P1.y() > P2.y()) {
            tag.coordinates = P1;
        } else {
            tag.coordinates = P2;
        }
        coordinateHistory[tag.tagID] = tag.coordinates;
    } else if ((anchor1Position.anchorID == 103 && anchor2Position.anchorID == 104) || (anchor1Position.anchorID == 104 && anchor2Position.anchorID == 103)) {
        if (P1.y() < P2.y()) {
            tag.coordinates = P1;
        } else {
            tag.coordinates = P2;
        }
        coordinateHistory[tag.tagID] = tag.coordinates;

    // 2nd case: Anchors are aligned alogn the longest edge or diagonal of the rectagular area
    // This is challenging as there is no deterministic way to estimate the right position. Therefore, we use a heuristic to choose the correct coordinate
    } else {

        if (coordinateHistory.contains(tag.tagID)) {
            // Remember the tag's previous position and calculate its difference with new position
            double delta1 = std::abs(coordinateHistory[tag.tagID].x() - P1.x());
            double delta2 = std::abs(coordinateHistory[tag.tagID].x() - P2.x());

            // consider a new coordinates as correct if it is lower then a threshold value
            if (h > 0 && (delta1 < 0.1 || delta2 < 0.1)){
                if (delta1 < delta2) {
                    tag.coordinates = P1;
                } else {
                    tag.coordinates = P2;
                }
                coordinateHistory[tag.tagID] = tag.coordinates;
            } else {

                // otherwise use last correct coordinate
                tag.coordinates = coordinateHistory[tag.tagID];
            }
        } else {
            tag.coordinates = P2; // if history for the tag has not been created yet
        }
    }
}


// ---------------------- Prediction of Pixel-to-Real and Optical coordinates -----------------------
// Both predictions time are possilbe at the same. The player is optimized to detect people only once.

// Set the type of prediction: Pixel-to-Real, Optical, or both.
int DataProcessor::setPredict(bool toPredict, PredictionType type) {
    predictionType = type;
    if (type == PredictionType::PredictionByPixelToReal) {

        if (!booster) {
            return -1;
        }

        isPredictionByPixelToRealRequested = toPredict;
        emit requestChangePredictionButtonName(type, isPredictionByPixelToRealRequested);

    } else if (type == PredictionType::PredictionByOptical) {

        if (cameraMatrix.empty()) {
            return -1;
        }

        isPredictionByOpticalRequested = toPredict;
        emit requestChangePredictionButtonName(type, isPredictionByOpticalRequested);
    }

    return 0;
}

// Load XGBooster Regressor
int DataProcessor::loadPixelToRealModelParams(const QString& filename) {
    int result = XGBoosterCreate(NULL, 0, &booster);
    if (result == 0) {
        result = XGBoosterLoadModel(booster, filename.toStdString().c_str());
        if (result == -1) {
            booster = nullptr;
        }
    }

    return result;
}

// Load Intrinsic Camera Matrix
void DataProcessor::setCameraMatrix(const cv::Mat& matrix) {
    cameraMatrix = matrix;
}

QPointF DataProcessor::predictWorldCoordinatesPixelToReal(const DetectionResult& detection) {

    QPointF coordinates;

    DMatrixHandle dmatrix;
    const float pixelCoordinates[] = {static_cast<float>(detection.bottomEdgeCenter.x()), static_cast<float>(detection.bottomEdgeCenter.y())};
    XGDMatrixCreateFromMat(pixelCoordinates, 1, 2, NAN, &dmatrix);

    // make prediction
    bst_ulong outLen;
    const float* outResult;
    XGBoosterPredict(booster, dmatrix, 0, 0, 0, &outLen, &outResult);

    // Translation of Pixel-to-Real coordinate system
    auto found = std::find_if(anchorPositions.begin(), anchorPositions.end(), [](const AnchorPosition& pos) {
        return pos.isOrigin;
    });
    AnchorPosition origin = *found;

    coordinates = QPointF(static_cast<double>(outResult[0]) + origin.x - 0.627, static_cast<double>(outResult[1])); // 0.627 is the offset from the left wall in video on which model is trained

    // cleanup
    XGDMatrixFree(dmatrix);

    return coordinates;

}

QPointF DataProcessor::predictWorldCoordinatesOptical(const DetectionResult& detection, const cv::Size& cameraFrameSize, const cv::Size& detectionFrameSize) {

    QPointF coordinates;
    int bboxHeight = detection.bbox.height;
    int imageX = detection.bottomEdgeCenter.x();
    int imageY = detection.bottomEdgeCenter.y();

    // Camera parameters are computed for 640x360 size of an image, but detection was performed on the image of size 640x640 (requirement of YOLOv4).
    // Need a scale factor for 640x640.
    double scaleX = detectionFrameSize.width / cameraFrameSize.width;
    double scaleY = (double)detectionFrameSize.height / (double)cameraFrameSize.height;

    // Export intrinsic parameters
    double fxAdjusted = cameraMatrix.at<double>(0, 0) * scaleX;
    double fyAdjusted = cameraMatrix.at<double>(1, 1) * scaleY;
    double cxAdjusted = cameraMatrix.at<double>(0, 2) * scaleX;
    double cyAdjusted = cameraMatrix.at<double>(1, 2) * scaleY;

    // Heuristic to set correct person's height
    double personHeight = 1.78;
    if (imageX < 280) {
        personHeight = 1.78;
    } else if (280 <= imageX && imageX <= 315) {
        personHeight = 1.86;
    } else {
        personHeight = 1.78;
    }

    double distance = (personHeight * fyAdjusted) / bboxHeight;

    // cx and fx from intrinsic calibration
    double worldX = ((imageX - cxAdjusted) * (distance / fxAdjusted));
    // cy and fy from intrinsic calibration
    double worldY = (imageY - cyAdjusted) * (distance / fyAdjusted); // eliminated; only x and z are interestin for us

    // Translation of Optical coordinate system
    auto found = std::find_if(anchorPositions.begin(), anchorPositions.end(), [](const AnchorPosition& pos) {
        return pos.isOrigin;
    });
    AnchorPosition origin = *found;

    coordinates = QPointF(worldX + origin.x + 1.25, distance - 0.15); // + origin.x - distance between the left wall to the left anchor, + 1.25 - distance between the camera and the left anchor

    return coordinates;
}

// ---------------------------- The part responsible for Data Analysis -------------------------------------------------

// Optimize to analyse only a specific range of the Video, as it might be several hours long
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

    // Find unique Tag IDs to be to select Tag - Anchor pairs for analysis
    uniqueTagIDs.reserve(uwbDataRangeToAnalyze.size());

    std::transform(uwbDataRangeToAnalyze.begin(), uwbDataRangeToAnalyze.end(), std::back_inserter(uniqueTagIDs), [](const UWBData& obj) { return obj.tagID; });
    std::sort(uniqueTagIDs.begin(), uniqueTagIDs.end());
    std::vector<int>::iterator last = std::unique(uniqueTagIDs.begin(), uniqueTagIDs.end());
    // Keep only unique tags
    uniqueTagIDs.erase(last, uniqueTagIDs.end());

    // Show available tags in Data Analysis Window (GUI)
    emit requestShowAvailableTags(uniqueTagIDs);
}

/* Select Tag's data and choose necassery Anchor ID for faster plotting
* Optimized to select Anchor - Tag pair only once for the entire analysis
* Tag is selected every time combobox is changed
*/
void DataProcessor::collectDataForTag(const QString &tagIDText) {
    bool converted;
    int tagID = tagIDText.toInt(&converted);

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

void DataProcessor::collectDataForPlotDistancesVsTimestamps(const int anchorID) {

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

    // Show Distances-vs-Timestamps plot in Data Analysis Window (GUI)
    emit requestShowPlotDistancesVsTimestamps(timestampsToAnalyze, distancesToAnalyzeOriginal);
}

// Optimized to handle the case when there is no amount of elements enough to form a full window for Rolling Deviation
void DataProcessor::calculateRollingDeviation(const int windowSize) {
    rollingDeviations.clear();

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

// Splitting to standing periods
void DataProcessor::splitDataset(const double threshold) {
    segmentMeans.clear();
    segmentFrameIDs.clear();
    segmentSizes.clear();
    segmentRepresentatives.clear();

    // Assuming that the segments are continuous, calculate mean for each segment
    // The mean is used to eliminate the case of choosing outlier as the segment's representant
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
            segmentFrameIDs.push_back(segmentVideoFrameID); // remember timestamp of the segment's middle record for model training, first frame of the segment is not always a good choice
            segmentSizes.push_back(segmentSize);
            segmentRepresentatives.push_back(std::move(segmentRepresentative));
            segmentDistanceSum = 0;
            segmentSize = 0;
        }
    }

    // handle last segment
    if (segmentDistanceSum > 0 && segmentSize > 0) {
        double segmentMean = segmentDistanceSum / segmentSize;
        segmentMeans.push_back(segmentMean);
        int middleIdx = segmentSize / 2;
        UWBData segmentRepresentative = *tagDataToAnalyze[i - middleIdx];
        int segmentVideoFrameID = binarySearchVideoFrameID(segmentRepresentative.timestamp);
        segmentFrameIDs.push_back(segmentVideoFrameID);
        segmentSizes.push_back(segmentSize);
        segmentRepresentatives.push_back(std::move(segmentRepresentative));
    }

    // Show segment's data in Data Analysis Window (GUI)
    emit requestShowDatasetSegments(segmentMeans);
}

// Get Frame ID as a representative of a segment
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

    // Show scatter plot, showing the difference between original (raw) and corrected distances
    emit requestShowOriginalVsAdjustedDistances(timestampsToAnalyze, distancesToAnalyzeOriginal, distancesToAnalyzeAdjusted);
}

// Update "on-the-fly" data to be visualized in Video Player (GUI)
void DataProcessor::updateOriginalWithAdjustedValues() {
    for (int i = 0; i < distancesToAnalyzeAdjusted.size(); ++i) {
        *(distancesToAnalyzeOriginal[i]) = distancesToAnalyzeAdjusted[i];
    }
}
