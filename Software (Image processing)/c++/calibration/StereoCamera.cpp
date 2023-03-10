#include "StereoCamera.h"
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>

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

StereoCamera::~StereoCamera() {
    leftCamera.release();
    rightCamera.release();
    cv::destroyAllWindows();
}

const cv::Mat& StereoCamera::getLeftFrame() {
    if (!leftCamera.read(readFrame)) {
        throw std::runtime_error("Failed to read the frame from the camera " + std::to_string(leftCameraIndex));
    }

    return readFrame;
}

const cv::Mat& StereoCamera::getRightFrame() {
    if (!rightCamera.read(readFrame)) {
        throw std::runtime_error("Failed to read the frame from the camera " + std::to_string(rightCameraIndex));
    }

    return readFrame;
}