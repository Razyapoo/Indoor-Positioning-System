#ifndef STEREOCAMERA_H
#define STEREOCAMERA_H

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <string>

class StereoCamera {
    public: 
        StereoCamera();
        // webcam
        StereoCamera(const int& leftCameraIndex, const int& rightCameraIndex);
        // IP camera
        StereoCamera(const std::string& leftCameraURL, const std::string& rightCameraURL);
        ~StereoCamera();

        const cv::Mat& getLeftFrame();
        const cv::Mat& getRightFrame();

    private: 
        int leftCameraIndex = -1;
        int rightCameraIndex = -1;
        std::string leftCameraURL = "";
        std::string rightCameraURL = "";

        cv::VideoCapture leftCamera, rightCamera;
        cv::Mat readLeftFrame;
        cv::Mat readRightFrame;

};

#endif 