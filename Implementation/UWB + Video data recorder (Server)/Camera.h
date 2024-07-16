#ifndef CAMERA_H
#define CAMERA_H

/*********************************************** Camera ********************************************************
 * This class is responsible for accessing the webcam using OpenCV
 * Executed in a separated thread (in main.cpp) together with VideoManager - to not block UWB data collection
****************************************************************************************************************/

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <cstdlib>
#include <string>

class Camera
{
public:
    static cv::VideoCapture camera;
    static int cameraIndex;
    static cv::Mat frame;

    static void initCamera(const int &cameraIndex);
    static cv::Mat &getFrame();
    static cv::Size getCameraSize();
    static double getCameraFPS();
    static void release(); // release camera
};

#endif