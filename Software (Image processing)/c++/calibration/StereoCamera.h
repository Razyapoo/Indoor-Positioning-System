#ifndef STEREOCAMERA_H
#define STEREOCAMERA_H

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <string>

class StereoCamera {
    public: 
        StereoCamera();
        StereoCamera(const int& leftCameraIndex, const int& rightCameraIndex);
        ~StereoCamera();

        const cv::Mat& getLeftFrame();
        const cv::Mat& getRightFrame();

    private: 
        int leftCameraIndex, rightCameraIndex;
        cv::VideoCapture leftCamera, rightCamera;
        cv::Mat readFrame;

};

#endif 