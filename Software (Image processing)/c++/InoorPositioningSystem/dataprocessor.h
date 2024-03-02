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
#include <QString>


#include "threadsafequeue.h"
#include "structures.h"

class DataProcessor: public QObject
{
    Q_OBJECT
public:

    DataProcessor(ThreadSafeQueue& frameQueue);
    ~DataProcessor();



public slots:
    void loadData(const QString& UWBDataFilename, const QString& videoDataFilename);

    // Find uwb data based on video timestamp. Timestamp is given by position.
    void findUWBMeasurementAndEnqueue(int frameIndex, QImage qImage);
    void collectDataForTag(const QString &tagIDText);
    void setRangeForDataAnalysis(const long long startFrameIndex, const long long endFrameIndex);
    // void collectDataForAnchor(const int anchorID);
    void collectDataForPlotDistancesVsTimestamps(const int anchorID);
    void calculateRollingDeviation(const int windowSize);
    void splitDataset(const double threshold);

signals:
    void requestShowPlot();
    void requestShowAvailableTags(const std::vector<int>& availableTagIDs);
    void requestShowAvailableAnchors(const std::vector<int>& availableAnchorIDs);
    void requestShowPlotDistancesVsTimestamps(const std::vector<long long>& timestamps, const std::vector<double>& distances);
    void requestShowPlotRollingDeviations(const std::vector<long long>& timestamps, const std::vector<double>& deviations);
    void requestShowDatasetSegments(const std::vector<double>& datasetSegmentMeans);

private:
    ThreadSafeQueue& frameQueue;
    std::vector<long long> timestampsVector;
    std::vector<UWBData> uwbDataVector;
    std::vector<UWBVideoData> uwbVideoDataVector;
    std::vector<int> uniqueTagIDs;
    std::vector<double> rollingDeviations;
    std::vector<double> datasetSegmentMeans;

    std::span<UWBData> uwbDataRangeToAnalyze;
    std::vector<UWBData> tagDataToAnalyze;
    // std::vector<Anchor> anchorDataToAnalyze;

    std::vector<long long> timestampsToAnalyze;
    std::vector<double> distancesToAnalyze;
    std::ifstream videoDataFile;
    std::ifstream uwbDataFile;


    UWBData linearSearchUWB(const long long& frameTimestamp);
    UWBData binarySearchUWB(const long long& frameTimestamp);
};

#endif // DATAPROCESSOR_H
