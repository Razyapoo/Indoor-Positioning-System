#include "StereoCamera.h"
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <cstdlib>

StereoCamera::StereoCamera() {}

StereoCamera::StereoCamera(const int& leftCameraIndex, const int& rightCameraIndex): leftCameraIndex(leftCameraIndex), rightCameraIndex(rightCameraIndex) {
    leftCamera = cv::VideoCapture(leftCameraIndex);
    rightCamera = cv::VideoCapture(rightCameraIndex);

    if (!leftCamera.isOpened()) {
        throw std::runtime_error("Failed to open camera " + std::to_string(leftCameraIndex));

    }

    if (!rightCamera.isOpened()) {
        throw std::runtime_error("Failed to open camera " + std::to_string(rightCameraIndex));
    }
}

StereoCamera::StereoCamera(const std::string& leftCameraURL, const std::string& rightCameraURL): leftCameraURL(leftCameraURL), rightCameraURL(rightCameraURL) {
    std::string options = "rtsp_transport;tcp";
    std::string envvar = "OPENCV_FFMPEG_CAPTURE_OPTIONS"; 
    setenv(envvar.c_str(), options.c_str(), 1);

    leftCamera = cv::VideoCapture(leftCameraURL, cv::CAP_FFMPEG);
    rightCamera = cv::VideoCapture(rightCameraURL, cv::CAP_FFMPEG);

    if (!leftCamera.isOpened()) {
        throw std::runtime_error("Failed to open camera " + leftCameraURL);

    }

    if (!rightCamera.isOpened()) {
        throw std::runtime_error("Failed to open camera " + rightCameraURL);
    }
}

StereoCamera::~StereoCamera() {
    leftCamera.release();
    rightCamera.release();
    cv::destroyAllWindows();
}

const cv::Mat& StereoCamera::getLeftFrame() {
    if (!leftCamera.read(readLeftFrame)) {
        if (leftCameraURL != "") throw std::runtime_error("Failed to read the frame from the camera " + leftCameraIndex);
        else throw std::runtime_error("Failed to read the frame from the camera " + std::to_string(leftCameraIndex));
    }

    if (leftCameraURL != "") cv::resize(readLeftFrame, readLeftFrame, cv::Size(640, 360)); 
    
    return readLeftFrame;
}

const cv::Mat& StereoCamera::getRightFrame() {
    if (!rightCamera.read(readRightFrame)) {
        if (rightCameraURL != "") throw std::runtime_error("Failed to read the frame from the camera " + std::to_string(rightCameraIndex));
        else throw std::runtime_error("Failed to read the frame from the camera " + rightCameraIndex);
    }

    if (rightCameraURL != "") cv::resize(readRightFrame, readRightFrame, cv::Size(640, 360));

    return readRightFrame;
}