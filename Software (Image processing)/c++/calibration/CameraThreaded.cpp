#include "CameraThreaded.h"
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <cstdlib>

Camera::Camera() {}

Camera::Camera(const int& cameraIndex): cameraIndex(cameraIndex) {
    camera = cv::VideoCapture(cameraIndex);

    if (!camera.isOpened()) {
        throw std::runtime_error("Failed to open camera " + std::to_string(cameraIndex));

    }
}

Camera::Camera(const std::string& cameraURL): cameraURL(cameraURL) {
    
    const std::string options = "rtsp_transport;tcp";
    const std::string envvar = "OPENCV_FFMPEG_CAPTURE_OPTIONS"; 
    setenv(envvar.c_str(), options.c_str(), 1);

    camera = cv::VideoCapture(cameraURL, cv::CAP_FFMPEG);

    if (!camera.isOpened()) {
        throw std::runtime_error("Failed to open camera " + cameraURL);

    }
}

Camera::~Camera() {
    camera.release();
}

const cv::Mat& Camera::getFrame() {
    if (!camera.read(frame)) {
        if (cameraURL != "") throw std::runtime_error("Failed to read the frame from the camera " + cameraIndex);
        else throw std::runtime_error("Failed to read the frame from the camera " + std::to_string(cameraIndex));
    }

    if (cameraURL != "") cv::resize(frame, frame, cv::Size(620, 480)); 
    
    return frame;
}