#include "dataprocessor.h"

DataProcessor::DataProcessor(ThreadSafeQueue& frameQueue): frameQueue(frameQueue){}
DataProcessor::~DataProcessor() {}

void DataProcessor::loadData(const QString& UWBDataFilename, const QString& videoDataFilename) {
    std::ifstream videoDataFile(videoDataFilename.toStdString());

    int id;
    long long timestamp;
    while (videoDataFile >> id >> timestamp)
    {
        // assuming data are recorded sequentially and no intermidiate data is missing
        timestampsVector.push_back(timestamp);
    }

    std::ifstream UWBDataFile(UWBDataFilename.toStdString());
    UWBData record;
    std::string line;
    Anchor anchor;

    while (std::getline(UWBDataFile, line, '\n'))
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
    UWBDataFile.close();
}

void DataProcessor::findUWBMeasurementAndEnqueue(int frameIndex, QImage qImage) {

    long long frameTimestamp = timestampsVector[frameIndex];

    // qDebug() << "Searching for UWB data...";



    VideoData videoData(frameIndex, std::move(qImage), frameTimestamp);

    UWBData closestUWB = linearSearchUWB(frameTimestamp);

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

void DataProcessor::analyseData(const long long startFrameIndex, const long long endFrameIndex) {
    long long startFrameTimestamp = timestampsVector[startFrameIndex];
    long long endFrameTimestamp = timestampsVector[endFrameIndex];

    // Assuming data are stored sequentially, it is sufficient to find onlt start and then sequentially read till the end
    // UWBVideoData uwbVideoDataStart = uwbV

    emit requestShowPlot();
}
