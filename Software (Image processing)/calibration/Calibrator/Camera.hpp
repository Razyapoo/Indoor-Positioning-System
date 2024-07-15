#ifndef CAMERA_H
#define CAMERA_H

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
    static void release();

    static cv::Mat getFrame();
};

#endif