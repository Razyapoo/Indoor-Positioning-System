#ifndef DATAPROCESSOR_H
#define DATAPROCESSOR_H

#include <fstream>
#include <sstream>
#include <vector>
#include <span>
#include <algorithm>
#include <QImage>
#include <QObject>
#include <QDebug>

#include "threadsafequeue.h"
#include "structures.h"

class DataProcessor: public QObject
{
    Q_OBJECT
public:

    DataProcessor(ThreadSafeQueue& frameQueue);
    ~DataProcessor();

    void dataAnalysisInit(const long long startFrameIndex, const long long endFrameIndex);

public slots:
    void loadData(const QString& UWBDataFilename, const QString& videoDataFilename);

    // Find uwb data based on video timestamp. Timestamp is given by position.
    void findUWBMeasurementAndEnqueue(int frameIndex, QImage qImage);
    void analyzeDataForTag(const int tagID);

signals:
    void requestShowPlot();
    void requestShowAvailableTags(const std::vector<int>& availableTagIDs);
    void requestShowPlotDistancesVsTimestamps(const std::vector<UWBData>& uwbData);

private:
    ThreadSafeQueue& frameQueue;
    std::vector<long long> timestampsVector;
    std::vector<UWBData> uwbDataVector;
    std::vector<UWBVideoData> uwbVideoDataVector;
    std::vector<int> uniqueTagIDs;

    std::span<UWBData> uwbDataRangeToAnalyze;
    std::vector<UWBData> tagDataToAnalyze;


    UWBData linearSearchUWB(const long long& frameTimestamp);
    UWBData binarySearchUWB(const long long& frameTimestamp);
};

#endif // DATAPROCESSOR_H
