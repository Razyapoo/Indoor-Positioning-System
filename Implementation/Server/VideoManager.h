#ifndef VIDEOMANAGER_H
#define VIDEOMANAGER_H

/*********************************************** Video Manager **********************************************************
 * Responsible for:
 *  - video recording
 *  - index file creation (to further access video in GUI Indoor Positioning System)
 * Allows to play / pause / stop (terminate) recording of both UWB and Video (by cv::imshow)
 * 
***********************************************************************************************************************/

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
