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
#include <QThread>
#include <eigen3/Eigen/Dense>
#include <opencv2/opencv.hpp>
// #include <Eigen/Dense>
#include <cmath>
#include <filesystem>
#include <QMutex>
#include <xgboost/c_api.h>
#include <opencv2/opencv.hpp>


#include "threadsafequeue.h"
#include "structures.h"

class DataProcessor: public QObject
{
    Q_OBJECT
public:

    DataProcessor(ThreadSafeQueue& frameQueue);
    ~DataProcessor();

    void loadData(const std::string& UWBDataFilename, const std::string& videoDataFilename);
    void getMiddleSegmentDataForModel();
    long long getVideoTimestampById(int id);
    int binarySearchVideoFrameID(const long long& uwbTimestamp);
    int getTotalFrames();
    int setPredict(bool toPredict, PredictionType type);
    int loadPixelToRealModelParams(const QString& filename);
    // void setOptimalCameraMatrix(std::vector<double>&& matrix);
    void setCameraMatrix(const cv::Mat& matrix);
    // void setDistCoeffs(std::vector<double>&& matrix);
    QPointF predictWorldCoordinatesPixelToReal(const DetectionResult& detection);
    QPointF predictWorldCoordinatesOptical(const DetectionResult& detection, const cv::Size& cameraFrameSize, const cv::Size& detectionFrameSize);
    // void loadModelParams(const QString& filename);
    // std::pair<double, double> predictWorldCoordinates(double xPixel, double yPixel);


public slots:

    // void loadData(const std::string& UWBDataFilename, const std::string& videoDataFilename);
    // Find uwb data based on video timestamp. Timestamp is given by position.
    void onFindUWBMeasurementAndEnqueue(int frameIndex, QImage qImage, DetectionData detectionData);
    void onFindUWBMeasurementAndExport(int frameIndex, int rangeIndex, ExportType type, const DetectionData& detectionData, bool lastRecord);
    void collectDataForTag(const QString &tagIDText);
    void setRangeForDataAnalysis(const long long startTimeSec, const long long endTimeSec);
    // void collectDataForAnchor(const int anchorID);
    void collectDataForPlotDistancesVsTimestamps(const int anchorID);
    void calculateRollingDeviation(const int windowSize);
    void splitDataset(const double threshold);
    void calculatePolynomialRegression(const std::vector<double>& referenceValues);
    void updateOriginalWithAdjustedValues();
    void calculateUWBCoordinates(UWBData& data);
    std::vector<int> getSegmentFrameIDs();
    void setAnchorPositions(std::vector<AnchorPosition> anchorPositions);

private slots:
    void cleanup();

signals:
    void requestShowPlot();
    void requestShowAvailableTags(const std::vector<int>& availableTagIDs);
    void requestShowAvailableAnchors(const std::vector<int>& availableAnchorIDs);
    void requestShowPlotDistancesVsTimestamps(const std::vector<long long>& timestamps, std::vector<double*> distances);
    void requestShowPlotRollingDeviations(const std::vector<long long>& timestamps, const std::vector<double>& deviations);
    void requestShowDatasetSegments(const std::vector<double>& segmentMeans);
    void requestShowOriginalVsAdjustedDistances(const std::vector<long long>& timestampsToAnalyze, std::vector<double*> distancesToAnalyzeOriginal, const std::vector<double>& distancesToAnalyzeAdjusted);
    void exportProgressUpdated(int progress);
    void requestChangePredictionButtonName(PredictionType type, bool isPredictionRequested);

private:
    ThreadSafeQueue& frameQueue;
    std::unique_ptr<QThread> dataProcessorThread;
    // std::atomic<bool>& toCalculateUWBLocalization;

    std::vector<AnchorPosition> anchorPositions;
    std::vector<long long> videoTimestampsVector;
    // std::vector<long long> framesRelativeTimeInSeconds;
    std::vector<UWBData> uwbDataVector;
    std::unordered_map<int, std::vector<UWBData*>> uwbDataPerTag;
    std::vector<UWBVideoData> uwbVideoDataVector;
    std::vector<int> uniqueTagIDs;
    std::vector<double> rollingDeviations;
    std::vector<double> segmentMeans;
    std::vector<int> segmentFrameIDs;
    std::vector<UWBData> segmentRepresentatives;
    std::vector<int> segmentSizes;
    QMap<int, QPointF> coordinateHistory;

    std::span<UWBData> uwbDataRangeToAnalyze;
    std::vector<UWBData*> tagDataToAnalyze;
    // std::vector<Anchor> anchorDataToAnalyze;

    std::vector<long long> timestampsToAnalyze;
    std::vector<double*> distancesToAnalyzeOriginal;
    std::vector<double> distancesToAnalyzeAdjusted;
    std::ifstream videoDataFile;
    std::ifstream uwbDataFile;
    // QJsonObject polynRegressionParams;

    std::atomic<PredictionType> predictionType;
    std::atomic<bool> isPredictionByPixelToRealRequested, isPredictionByOpticalRequested;
    std::string modelFilename;
    std::string intrinsicCalibrationFilename;

    BoosterHandle booster = nullptr;
    cv::Mat optimalCameraMatrix, cameraMatrix, distCoeffs;

    std::ofstream outputFileUWB, outputFileOptical, outputFilePixelToReal;
    int fileIncrementer;




    UWBData linearSearchUWB(const long long& frameTimestamp);
    UWBData binarySearchUWB(const long long& frameTimestamp);
    UWBData binarySearchUWB(const long long& frameTimestamp, const std::vector<UWBData*>& uwbDataVector);


};

#endif // DATAPROCESSOR_H
