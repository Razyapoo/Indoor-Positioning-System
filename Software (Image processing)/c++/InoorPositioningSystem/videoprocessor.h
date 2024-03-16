#ifndef VIDEOPROCESSOR_H
#define VIDEOPROCESSOR_H

#include <QObject>
#include <QImage>
#include <opencv2/opencv.hpp>
#include <QDebug>

#include "dataprocessor.h"
#include "structures.h"
#include "humandetector.h"

class VideoProcessor : public QObject
{
    Q_OBJECT

public:
    VideoProcessor(ThreadSafeQueue& frameQueue, DataProcessor* dataProcessor);
    ~VideoProcessor();
    void init(const QString& filename);
    double getVideoDuration() const;
    double getFPS() const;
    int getTotalFrames() const;

    void continueProcessing();
    void stopProcessing();
    bool isRunning();


public slots:
    void processVideo();
    void seekToFrame(int position);
    void onDataExport(int endPosition);


signals:
    // void latestFrame(const QImage& qImage, const int& position);
    void finished();
    void seekingDone();
    void processingIsStopped();
    void requestFindUWBMeasurementAndEnqueue(int position, QImage qImage);
    void requestFindUWBMeasurementAndExport(int position, const std::vector<QPoint>& detectedBoxCenterPoint, bool lastRecord);
    void exportFinished();

private:
    ThreadSafeQueue& frameQueue;
    DataProcessor* dataProcessor;
    HumanDetector humanDetector;

    std::atomic<bool> keepProcessingVideo;
    cv::VideoCapture camera;
    cv::Mat frame;
    QImage qImage;
    std::string filename;

    double fps;
    double videoDuration;
    int totalFrames;
    int keyframeInterval;

    std::atomic<bool> isExportRequested;
    int endDataExportPosition;

    std::vector<QPoint> detectPeople(cv::Mat& frame);

};

#endif // VIDEOPROCESSOR_H
