#ifndef DATAPROCESSOR_H
#define DATAPROCESSOR_H

/*********************************************** Data Processor ***********************************************
 *
 * This class is responsible for data processing. It prepares data for visualization, analysis, export in GUI.
 * Calculates coordinates for each method.
 * It operates in a separate thread for player optimization.
 *
****************************************************************************************************************/

#include <fstream>
#include <sstream>
#include <vector>
#include <span>
#include <algorithm>
#include <QImage>
#include <QObject>
#include <QString>
#include <QThread>
#include <eigen3/Eigen/Dense>
#include <opencv2/opencv.hpp>
#include <cmath>
#include <filesystem>
#include <QMutex>
#include <xgboost/c_api.h>
#include <QMap>

#include "threadsafequeue.h"
#include "structures.h"

class DataProcessor: public QObject
{
    Q_OBJECT
public:

    DataProcessor(ThreadSafeQueue& frameQueue);
    ~DataProcessor();

    void loadData(const std::string& folderName, const std::string& UWBDataFilename, const std::string& videoDataFilename);
    long long getVideoTimestampById(int id);
    int getTotalFrames();
    int binarySearchVideoFrameID(const long long& uwbTimestamp);
    int setPredict(bool toPredict, PredictionType type);
    int loadPixelToRealModelParams(const QString& filename);
    void setCameraMatrix(const cv::Mat& matrix);
    QPointF predictWorldCoordinatesPixelToReal(const DetectionResult& detection);
    QPointF predictWorldCoordinatesOptical(const DetectionResult& detection, const cv::Size& cameraFrameSize, const cv::Size& detectionFrameSize);

public slots:

    void onFindUWBMeasurementAndEnqueue(int frameIndex, QImage qImage, DetectionData detectedPeople);
    void onFindUWBMeasurementAndExport(int frameIndex, int rangeIndex, ExportType type, const DetectionData& detectedPeople, bool lastRecord);
    void setAnchorPositions(std::vector<AnchorPosition> anchorPositions);
    void calculateUWBCoordinates(UWBData& data);
    void collectDataForTag(const QString &tagIDText);
    void setRangeForDataAnalysis(const long long startTimeSec, const long long endTimeSec);
    void splitDataset(const double threshold);
    void collectDataForPlotDistancesVsTimestamps(const int anchorID);
    void calculateRollingDeviation(const int windowSize);
    void calculatePolynomialRegression(const std::vector<double>& referenceValues);
    void updateOriginalWithAdjustedValues();
    std::vector<int> getSegmentFrameIDs();

private slots:
    void cleanup();

signals:
    void requestChangePredictionButtonName(PredictionType type, bool isPredictionRequested);
    void requestShowPlot();
    void requestShowAvailableTags(const std::vector<int>& availableTagIDs);
    void requestShowAvailableAnchors(const std::vector<int>& availableAnchorIDs);
    void requestShowPlotDistancesVsTimestamps(const std::vector<long long>& timestamps, std::vector<double*> distances);
    void requestShowPlotRollingDeviations(const std::vector<long long>& timestamps, const std::vector<double>& deviations);
    void requestShowDatasetSegments(const std::vector<double>& segmentMeans);
    void requestShowOriginalVsAdjustedDistances(const std::vector<long long>& timestampsToAnalyze, std::vector<double*> distancesToAnalyzeOriginal, const std::vector<double>& distancesToAnalyzeAdjusted);
    void exportProgressUpdated(int progress);

private:
    ThreadSafeQueue& frameQueue;
    std::unique_ptr<QThread> dataProcessorThread;

    // Load data
    std::string projectFolderName;
    std::ifstream videoDataFile;
    std::ifstream uwbDataFile;
    std::vector<long long> videoTimestampsVector;
    std::vector<AnchorPosition> anchorPositions;
    std::vector<UWBData> uwbDataVector;
    std::unordered_map<int, std::vector<UWBData*>> uwbDataPerTag;
    std::vector<UWBVideoData> uwbVideoDataVector;
    std::vector<int> uniqueTagIDs;
    QMap<int, QPointF> coordinateHistory;

    // Data Analysis
    std::span<UWBData> uwbDataRangeToAnalyze;
    std::vector<UWBData*> tagDataToAnalyze;
    std::vector<long long> timestampsToAnalyze;
    std::vector<double> rollingDeviations;
    std::vector<double> segmentMeans;
    std::vector<int> segmentFrameIDs;
    std::vector<UWBData> segmentRepresentatives;
    std::vector<int> segmentSizes;
    std::vector<double*> distancesToAnalyzeOriginal;
    std::vector<double> distancesToAnalyzeAdjusted;

    // Prediction
    std::string modelFilename;
    std::string intrinsicCalibrationFilename;
    std::atomic<PredictionType> predictionType;
    std::atomic<bool> isPredictionByPixelToRealRequested, isPredictionByOpticalRequested;
    BoosterHandle booster = nullptr;
    cv::Mat optimalCameraMatrix, cameraMatrix, distCoeffs;
    std::ofstream outputFileUWB, outputFileOptical, outputFilePixelToReal;
    int fileIncrementer;

    // private functions
    UWBData linearSearchUWB(const long long& frameTimestamp);
    UWBData binarySearchUWB(const long long& frameTimestamp);
    UWBData binarySearchUWB(const long long& frameTimestamp, const std::vector<UWBData*>& uwbDataVector);
};

#endif // DATAPROCESSOR_H
