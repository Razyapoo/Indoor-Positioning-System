#ifndef CALIBRATION_H
#define CALIBRATION_H


/*********************************************** Calibrator ***************************************************
 * Responsible for Camera intrinsic calibration
 * 
 * Requirenments: 
 *  - functional webcam
 *  - 6x9 chessboard pattern (either printed or electronic)
****************************************************************************************************************/

#include <iostream>
#include <stdio.h>
#include <vector>
#include <memory>
#include <filesystem>

#include "Camera.h"

class Calibrator
{
public:
    static cv::VideoCapture videoSource;
    static std::string intrinsicFilePath;
    static bool intrinsicParamsSaved;

    // Chessboard size
    static uint8_t chessboardHeight, chessboardWidth;
    
    // Calibration parameters
    static float squareSize, alpha;
    static double reprojectionError;
    const static cv::Size chessboardSize, imageSize;
    const static cv::TermCriteria criteriaMono;
    const static int numberOfCells;
    static std::vector<std::vector<cv::Point3f>> objectPoints;
    static std::vector<std::vector<cv::Point2f>> imagePoints;
    static std::vector<cv::Point2f> corners;
    static std::vector<cv::Point3f> objectPoint;
    
    // Working with frames
    static cv::Mat image, gray;
    static bool found;
    static uint8_t key;
    static uint16_t imageCounter;
    
    // Output matrices that are used in GUI
    static cv::Mat cameraMatrix, optimalCameraMatrix, distortionCoeffs;
    static std::vector<cv::Mat> rotationVecs, translationVecs;

    // Methods
    static void initCameraCalibration();
    static void detectChessboard();
    static void intrinsicCalibration();
    static double getReprojectionError(const std::vector<std::vector<cv::Point3f>> &objectPoints, const std::vector<std::vector<cv::Point2f>> &imagePoints, const std::vector<cv::Mat> &rotationVecs, const std::vector<cv::Mat> &translationVecs, const cv::Mat &cameraMatrix, const cv::Mat &distortionCoeffs);
};

#endif