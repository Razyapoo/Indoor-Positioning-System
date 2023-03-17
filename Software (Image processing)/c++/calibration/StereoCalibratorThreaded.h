#ifndef STEREOCALIBRATION_H
#define STEREOCALIBRATION_H

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <iostream>
#include <stdio.h>
#include "CameraThreaded.h"
#include "Disparity.h"

class StereoCalibrator {
    public: 
        StereoCalibrator(Camera& Camera);
        ~StereoCalibrator();

        void initCameraCalibration(const std::string& pIntrinsicFilePath, const std::string& pExtrinsicFilePath, const std::string& pRectificationFilePath);
        void detectChessboard();
        void intrinsicCalibration();
        void extrinsicCalibration();
        void stereoRectification();
        void reprojectionError();

    private: 

        // Camera cameraLeft;
        // Camera cameraRight;

        std::string intrinsicFilePath, extrinsicFilePath, rectificationFilePath;
        Camera cameraLeft, cameraRight;
        uint8_t chessboardHeight = 6;
        uint8_t chessboardWidth = 9;
        uint8_t key;
        uint16_t imageCounter = 0;
        float squareSize = 2.5f;
        float alpha = 1;
        double reprojectionErrorLeft;
        double reprojectionErrorRight;

        const cv::Size chessboardSize = cv::Size(chessboardWidth, chessboardHeight);
        const cv::Size imageSize = cv::Size(640, 360);
        const cv::TermCriteria& criteria = cv::TermCriteria(cv::TermCriteria::MAX_ITER + cv::TermCriteria::EPS, 100, 0.001);
        const int numberOfCells = chessboardHeight * chessboardWidth;

        std::vector<std::vector<cv::Point3f>> objectPoints;
        std::vector<std::vector<cv::Point2f>> imagePointsLeft, imagePointsRight;
        std::vector<cv::Point2f> cornersLeft, cornersRight;
        std::vector<cv::Point3f> objectPoint;
        
        // Working with frames
        cv::Mat imageLeft, imageRight, grayLeft, grayRight;
        bool foundLeft, foundRight;

        // Intrinsic calibration
        cv::Mat cameraMatrixLeft, cameraMatrixRight, distortionCoeffsLeft, distortionCoeffsRight, rotationVecsLeft, rotationVecsRight, translationVecsLeft, translationVecsRight;
        cv::Mat optimalCameraMatrixLeft, optimalCameraMatrixRight;
        // Extrinsic calibration 
        cv::Mat rotationMatrixCommon, translationVector, essentialMatrix, fundamentalMatrix;
        // Rectification
        cv::Mat projectionMatrixLeft, projectionMatrixRight, disparityMatrix; 
        // Undistortion
        cv::Mat mapLeftX, mapLeftY, mapRightX, mapRightY;
        cv::Mat undistortedImageLeft, undistortedImageRight;
        cv::Mat rectifiedPair;
};

#endif