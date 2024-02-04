#ifndef VIDOMANAGER_H
#define VIDOMANAGER_H

#include <iostream>
#include <fstream>
#include <chrono>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <thread>

#include "StereoCamera.hpp"
#include "SharedData.hpp"

class VideoManager
{
public:
    static size_t frameIndex;
    static double fps;
    static cv::Size frameSize;
    static cv::Mat leftFrame, rightFrame;
    static std::chrono::milliseconds currentTime;
    static std::time_t timestamp;
    static cv::Mat timestampMat;
    static uint8_t key;
    // static bool isPause;

    // static std::string getTimestamp();
    static void videoRecorder();
    static void videoLoader();
};

#endif
