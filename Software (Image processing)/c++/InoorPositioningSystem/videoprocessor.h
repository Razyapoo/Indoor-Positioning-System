#ifndef VIDEOPROCESSOR_H
#define VIDEOPROCESSOR_H

#include <QObject>
#include <QImage>
#include <opencv2/opencv.hpp>
#include <QDebug>

#include "dataprocessor.h"
#include "structures.h"

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


signals:
    // void latestFrame(const QImage& qImage, const int& position);
    void finished();
    void seekingDone();
    void processingIsStopped();
    void requestFindUWBMeasurement(int position, QImage qImage);

private:
    ThreadSafeQueue& frameQueue;
    DataProcessor* dataProcessor;

    std::atomic<bool> keepProcessingVideo;
    cv::VideoCapture camera;
    cv::Mat frame;
    QImage qImage;
    std::string filename;

    double fps;
    double videoDuration;
    int totalFrames;
    int keyframeInterval;


};

#endif // VIDEOPROCESSOR_H
