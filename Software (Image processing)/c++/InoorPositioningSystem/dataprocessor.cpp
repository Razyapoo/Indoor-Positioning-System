#include "dataprocessor.h"

DataProcessor::DataProcessor(ThreadSafeQueue& frameQueue): frameQueue(frameQueue){}
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

    timestampsVector.erase(timestampsVector.begin(), timestampsVector.end());

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
    uwbDataVector.erase(uwbDataVector.begin(), uwbDataVector.end());
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


    // qDebug() << "Data are loaded";


    videoDataFile.close();
    uwbDataFile.close();
}

void DataProcessor::findUWBMeasurementAndEnqueue(int frameIndex, QImage qImage) {

    long long frameTimestamp = timestampsVector[frameIndex - 1];

    // qDebug() << "Searching for UWB data...";

    VideoData videoData(frameIndex, std::move(qImage), frameTimestamp);

    UWBData closestUWB = binarySearchUWB(frameTimestamp);

    // it is better to make a copy of UWB Data and then move it to the queue rather than push a pointer to existing array, just in case UWBData array will be deleted.
    UWBVideoData uwbVideoData(std::move(videoData), std::move(closestUWB));
    // uwbVideoDataVector.push_back(uwbVideoData);


    frameQueue.enqueue(std::move(uwbVideoData));

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

    tagDataToAnalyze.erase(tagDataToAnalyze.begin(), tagDataToAnalyze.end());

    std::vector<int> availableAnchorsForTag;
    for (const UWBData& data: uwbDataRangeToAnalyze) {
        if (data.tagID == tagID) {
            tagDataToAnalyze.push_back(data);
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
    timestampsToAnalyze.erase(timestampsToAnalyze.begin(), timestampsToAnalyze.end());
    distancesToAnalyze.erase(distancesToAnalyze.begin(), distancesToAnalyze.end());


    for (const UWBData& data: tagDataToAnalyze) {
        for (const Anchor& anchor: data.anchorList) {
            if (anchor.anchorID == anchorID) {
                timestampsToAnalyze.push_back(data.timestamp);
                distancesToAnalyze.push_back(anchor.distance);
            }
        }
    }

    emit requestShowPlotDistancesVsTimestamps(timestampsToAnalyze, distancesToAnalyze);

}

void DataProcessor::calculateRollingDeviation(const int windowSize) {
    rollingDeviations.erase(rollingDeviations.begin(), rollingDeviations.end());

    for (int i = 0; i <= distancesToAnalyze.size() - windowSize; ++i) {
        // Calculate mean for the window
        double sum = 0.0;
        for (int j = i; j < i + windowSize; ++j) {
            sum += distancesToAnalyze[j];
        }
        double mean = sum / windowSize;

        // Calculate sum of squares of differences from the mean
        double sumSquares = 0.0;
        for (int j = i; j < i + windowSize; ++j) {
            sumSquares += (distancesToAnalyze[j] - mean) * (distancesToAnalyze[j] - mean);
        }

        // Calculate standard deviation for the window
        double deviation = std::sqrt(sumSquares / windowSize);
        rollingDeviations.push_back(deviation);
    }

    emit requestShowPlotRollingDeviations(timestampsToAnalyze, rollingDeviations);
}

void DataProcessor::splitDataset(const double threshold) {
    datasetSegmentMeans.erase(datasetSegmentMeans.begin(), datasetSegmentMeans.end());

    // Detect segments where tag was standing and calculate mean for each segment
    // Assuming segments are continuous
    double segmentSum = 0.0;
    int segmentSize = 0;
    for (int i = 0; i < rollingDeviations.size(); ++i) {
        if (std::isnan(rollingDeviations[i]) || rollingDeviations[i] <= threshold) {
            segmentSum += distancesToAnalyze[i];
            segmentSize += 1;
        } else if (segmentSum > 0 && segmentSize > 0) {
            double temp = segmentSum / segmentSize;
            datasetSegmentMeans.push_back(temp);
            segmentSum = 0;
            segmentSize = 0;
        }
    }

    emit requestShowDatasetSegments(datasetSegmentMeans);
}
