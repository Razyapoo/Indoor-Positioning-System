#ifndef STEREOCALIBRATION_H
#define STEREOCALIBRATION_H

#include <iostream>
#include <stdio.h>
#include <vector>
#include <memory>
#include <filesystem>
#include "Disparity.hpp"

class StereoCalibrator
{
public:
    // Variables

    // for video input
    // static bool videoAsSource;
    static cv::VideoCapture leftVideoSource, rightVideoSource;

    // common variables
    static std::string intrinsicFilePath, extrinsicFilePath, rectificationFilePath;
    static bool intrinsicParamsSaved, extrinsicParamsSaved, rectificationParamsSaved;
    static uint8_t chessboardHeight;
    static uint8_t chessboardWidth;
    static uint8_t key;
    static uint16_t imageCounter;
    static float squareSize, alpha;
    static double reprojectionErrorLeft, reprojectionErrorRight;
    const static cv::Size chessboardSize, imageSize;
    const static cv::TermCriteria criteriaMono, criteriaStereo;
    const static int numberOfCells;
    static std::vector<std::vector<cv::Point3f>> objectPoints;
    static std::vector<std::vector<cv::Point2f>> imagePointsLeft, imagePointsRight;
    static std::vector<cv::Point2f> cornersLeft, cornersRight;
    static std::vector<cv::Point3f> objectPoint;
    // Working with frames
    static cv::Mat imageLeft, imageRight, grayLeft, grayRight;
    static bool foundLeft, foundRight;
    // Intrinsic calibration
    static cv::Mat cameraMatrixLeft, cameraMatrixRight, distortionCoeffsLeft, distortionCoeffsRight;
    static std::vector<cv::Mat> rotationVecsLeft, rotationVecsRight, translationVecsLeft, translationVecsRight;
    static cv::Mat optimalCameraMatrixLeft, optimalCameraMatrixRight;
    // Extrinsic calibration
    static cv::Mat rotationMatrixCommon, translationVector, essentialMatrix, fundamentalMatrix;
    // Rectification
    static cv::Mat rotationMatrixLeft, rotationMatrixRight, projectionMatrixLeft, projectionMatrixRight, disparityMatrix;
    // Undistortion
    static cv::Mat mapLeftX, mapLeftY, mapRightX, mapRightY;
    static cv::Mat undistortedImageLeft, undistortedImageRight;
    static cv::Mat rectifiedPair;

    // Methods
    static void initCameraCalibration();
    static void detectChessboard();
    static void intrinsicCalibration();
    static void extrinsicCalibration();
    static void stereoRectification();
    static double getReprojectionError(const std::vector<std::vector<cv::Point3f>> &objectPoints, const std::vector<std::vector<cv::Point2f>> &imagePoints, const std::vector<cv::Mat> &rotationVecs, const std::vector<cv::Mat> &translationVecs, const cv::Mat &cameraMatrix, const cv::Mat &distortionCoeffs);
    static double getEpipolarError(const std::vector<std::vector<cv::Point2f>> &imagePoints);
};

#endif