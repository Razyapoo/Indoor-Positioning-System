#ifndef Camera_H
#define Camera_H

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <string>

class Camera {
    public: 
        Camera();
        // webcam
        Camera(const int& cameraIndex);
        // IP camera
        Camera(const std::string& cameraURL);
        ~Camera();

        const cv::Mat& getFrame();

    private: 
        int cameraIndex = -1;
        std::string cameraURL = "";

        cv::VideoCapture camera;
        cv::Mat frame;

};

#endif 