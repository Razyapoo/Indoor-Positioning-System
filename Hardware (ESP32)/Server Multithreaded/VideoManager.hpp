#ifndef VIDOMANAGER_H
#define VIDOMANAGER_H

#include <iostream>
#include <fstream>
#include <chrono>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <thread>

#include "Camera.hpp"
#include "SharedData.hpp"

class VideoManager
{
public:
    static size_t frameIndex;
    static double fps;
    static cv::Size frameSize;
    static cv::Mat frame;
    static std::chrono::milliseconds currentTime;
    static std::time_t timestamp;
    static cv::Mat timestampMat;
    static uint8_t key;
    static void runVideoRecorder();
};

#endif
