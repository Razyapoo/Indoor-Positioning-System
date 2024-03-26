#ifndef VIDEOPROCESSOR_H
#define VIDEOPROCESSOR_H

#include <QObject>
#include <QThread>
#include <atomic>
#include <QMutex>
#include <QWaitCondition>
#include <QImage>
#include <opencv2/opencv.hpp>
#include <QDebug>
#include <eigen3/Eigen/Dense>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QFile>
#include <QIODevice>


#include "dataprocessor.h"
#include "structures.h"
#include "humandetector.h"

class VideoProcessor : public QObject
{
    Q_OBJECT

public:
    VideoProcessor(ThreadSafeQueue& frameQueue, DataProcessor* dataProcessor);
    ~VideoProcessor();

    void init(const std::string& filename);
    double getVideoDuration() const;
    double getFPS() const;
    int getTotalFrames() const;

    void resumeProcessing();
    void pauseProcessing();
    void stopProcessing();
    bool isRunning();

    void seekToFrame(int position);
    void setFrameRangeToExport(const std::vector<int>& frameRange, ExportType type);
    void stopExport();
    void setPredict(bool toPredict);
    void loadModelParams(const QString& filename);
    std::pair<double, double> predictWorldCoordinates(double xPixel, double yPixel);

public slots:
    void processVideo();

private slots:
    void cleanup();

signals:
    // void latestFrame(const QImage& qImage, const int& position);
    void finished();
    void seekingDone();
    void processingIsPaused();
    void requestFindUWBMeasurementAndEnqueue(int position, QImage qImage);
    void requestFindUWBMeasurementAndExport(int position, int rangeIndex, ExportType type, const std::vector<QPoint>& detectedBoxCenterPoint, bool lastRecord);
    void exportFinished(bool success);

private:
    ThreadSafeQueue& frameQueue;
    DataProcessor* dataProcessor;
    HumanDetector humanDetector;

    std::unique_ptr<QThread> videoProcessorThread;
    std::atomic<bool> shouldStopVideoProcessing, isSeekRequested, isExportRequested, isPaused, shouldStopExport, isPredictionRequested;
    QMutex mutex;
    QWaitCondition pauseCondition;

    // std::atomic<bool> keepProcessingVideo;
    cv::VideoCapture camera;
    cv::Mat frame;
    QImage qImage;
    std::string filename;

    double fps;
    double videoDuration;
    int totalFrames;
    int keyframeInterval;
    int seekPosition;
    ExportType exportType;
    QJsonObject polynRegressionParams;
    // int frameByFrameExportEndPosition;

    std::vector<int> frameRangeToExport;


    std::vector<QPoint> detectPeople(cv::Mat& frame);

};

#endif // VIDEOPROCESSOR_H
