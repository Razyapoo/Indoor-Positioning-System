#ifndef VIDEOPROCESSOR_H
#define VIDEOPROCESSOR_H

/*********************************************** Video Processor ********************************************************************************
 *
 * This class is responsible for video processing. It reads video, detects people and prepares video data for DataProcessor (for synchronization)
 * It operates in a separate thread for player optimization.
 * For example, this way people detection (involves very high resource consumption) do not block the Video Player (GUI) and Data Processor.
 *
*************************************************************************************************************************************************/

#include <QObject>
#include <QThread>
#include <atomic>
#include <QMutex>
#include <QWaitCondition>
#include <QImage>
#include <opencv2/opencv.hpp>

#include "dataprocessor.h"
#include "structures.h"
#include "humandetector.h"

class VideoProcessor : public QObject
{
    Q_OBJECT

public:
    VideoProcessor(ThreadSafeQueue& frameQueue, DataProcessor* dataProcessor);
    ~VideoProcessor();

    // Load video
    void init(const std::string& filename);
    double getVideoDuration() const;
    double getFPS() const;
    int getTotalFrames() const;

    // Play / Pause / Seek
    void resumeProcessing();
    void pauseProcessing();
    void stopProcessing();
    void seekToFrame(int position);
    void setFrameRangeToExport(const std::vector<int>& frameRange, ExportType type);

    void stopExport(); // export

    // Handle people detection
    void initHumanDetector(const std::string &modelConfiguration, const std::string &modelWeights);
    void setOptimalCameraMatrix(const cv::Mat& matrix);
    void setCameraMatrix(const cv::Mat& matrix);
    void setDistCoeffs(const cv::Mat& matrix);
    int setPredict(bool toPredict);

public slots:
    void processVideo();

private slots:
    void cleanup();

signals:
    void seekingDone();

    void requestFindUWBMeasurementAndEnqueue(int position, QImage qImage, DetectionData detectedPeople);
    void requestFindUWBMeasurementAndExport(int position, int rangeIndex, ExportType type, const DetectionData& detectedPeople, bool lastRecord);

    void exportFinished(bool success);

    void humanDetectorNotInitialized();
    void distCoeffLoaded();

private:
    ThreadSafeQueue& frameQueue;
    DataProcessor* dataProcessor;
    HumanDetector humanDetector;
    std::unique_ptr<QThread> videoProcessorThread;

    cv::VideoCapture camera;
    cv::Size cameraFrameSize, detectionFrameSize;
    cv::Mat frame;
    QImage qImage;
    double fps;
    double videoDuration;
    int totalFrames;
    int seekPosition;

    QMutex mutex;
    QWaitCondition pauseCondition;
    std::atomic<bool> shouldStopVideoProcessing, isSeekRequested, isExportRequested, isPaused, shouldStopExport, isPredictionRequested;

    ExportType exportType;
    std::vector<int> frameRangeToExport;

    cv::Mat optimalCameraMatrix, cameraMatrix, distCoeffs;
    bool isDistCoeffSet;

    void detectPeople(cv::Mat& frame, std::vector<DetectionResult>& detectionsVector);
};

#endif // VIDEOPROCESSOR_H
