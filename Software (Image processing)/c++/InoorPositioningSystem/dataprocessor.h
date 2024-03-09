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
#include <eigen3/Eigen/Dense>
#include <cmath>

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
    void calculatePolynomialRegression(const std::vector<double>& referenceValues);
    void updateOriginalWithAdjustedValues();
    void calculateUWBCoordinates(UWBData& data);

signals:
    void requestShowPlot();
    void requestShowAvailableTags(const std::vector<int>& availableTagIDs);
    void requestShowAvailableAnchors(const std::vector<int>& availableAnchorIDs);
    void requestShowPlotDistancesVsTimestamps(const std::vector<long long>& timestamps, std::vector<double*> distances);
    void requestShowPlotRollingDeviations(const std::vector<long long>& timestamps, const std::vector<double>& deviations);
    void requestShowDatasetSegments(const std::vector<double>& datasetSegmentMeans);
    void requestShowOriginalVsAdjustedDistances(const std::vector<long long>& timestampsToAnalyze, std::vector<double*> distancesToAnalyzeOriginal, const std::vector<double>& distancesToAnalyzeAdjusted);

private:
    ThreadSafeQueue& frameQueue;
    // std::atomic<bool>& toCalculateUWBLocalization;

    std::vector<long long> timestampsVector;
    std::vector<UWBData> uwbDataVector;
    std::unordered_map<int, std::vector<UWBData*>> uwbDataPerTag;
    std::vector<UWBVideoData> uwbVideoDataVector;
    std::vector<int> uniqueTagIDs;
    std::vector<double> rollingDeviations;
    std::vector<double> datasetSegmentMeans;

    std::span<UWBData> uwbDataRangeToAnalyze;
    std::vector<UWBData*> tagDataToAnalyze;
    // std::vector<Anchor> anchorDataToAnalyze;

    std::vector<long long> timestampsToAnalyze;
    std::vector<double*> distancesToAnalyzeOriginal;
    std::vector<double> distancesToAnalyzeAdjusted;
    std::ifstream videoDataFile;
    std::ifstream uwbDataFile;


    UWBData linearSearchUWB(const long long& frameTimestamp);
    UWBData binarySearchUWB(const long long& frameTimestamp);
    UWBData binarySearchUWB(const long long& frameTimestamp, const std::vector<UWBData*>& uwbDataVector);

};

#endif // DATAPROCESSOR_H
