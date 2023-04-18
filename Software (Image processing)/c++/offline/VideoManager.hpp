#ifndef VIDOMANAGER_H
#define VIDOMANAGER_H


#include <iostream>
#include <fstream>
#include <chrono>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <thread>

#include "Disparity.hpp"

class VideoManager {
    public: 

        static size_t frameIndex;
        const static int fps; 
        const static cv::Size frameSize;
        static cv::Mat leftFrame, rightFrame;
        static std::chrono::system_clock::time_point currentTime;
        static std::time_t timestamp;
        static cv::Mat timestampMat;
        static uint8_t key;
        static bool isPause;

        // static std::string getTimestamp();
        // static void videoRecorder();
        static void videoLoader(const std::string& leftSource, const std::string& rightSource);
};

#endif

