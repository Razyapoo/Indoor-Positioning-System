#ifndef STEREOCAMERA_H
#define STEREOCAMERA_H

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <cstdlib>
#include <string>

class StereoCamera
{
public:
    static int leftCameraIndex, rightCameraIndex;
    static std::string leftCameraURL, rightCameraURL;
    static cv::Mat readLeftFrame, readRightFrame;

    static cv::VideoCapture leftCamera, rightCamera;
    const static std::string options, envvar;

    static void initStereoCamera(const int &leftCameraIndex, const int &rightCameraIndex);
    static void initStereoCamera(const std::string &leftCameraURL, const std::string &rightCameraURL);
    static void release();

    static cv::Mat &getLeftFrame();
    static cv::Mat &getRightFrame();

    static cv::Size getCameraSize();
    static double getCameraFPS();
};

#endif