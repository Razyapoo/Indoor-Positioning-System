#ifndef CAMERA_H
#define CAMERA_H

/*********************************************** Camera ********************************************************
 * This class is responsible for accessing the webcam using OpenCV
****************************************************************************************************************/

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <cstdlib>
#include <string>

class Camera
{
public:
    static int cameraIndex;

    static cv::VideoCapture camera;

    static void initCamera(const int &cameraIndex);
    static cv::Mat getFrame();
    static void release(); // safe termination
};

#endif