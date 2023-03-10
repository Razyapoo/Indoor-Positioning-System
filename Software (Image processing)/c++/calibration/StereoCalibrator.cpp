#include "StereoCalibrator.h"
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>

StereoCalibrator::StereoCalibrator(const StereoCamera& stereoCamera): stereoCamera(stereoCamera) {}

StereoCalibrator::~StereoCalibrator() {}


void StereoCalibrator::initCameraCalibration(const std::string& pIntrinsicFilePath, const std::string& pExtrinsicFilePath, const std::string& pRectificationFilePath) {

    intrinsicFilePath = pIntrinsicFilePath;
    extrinsicFilePath = pExtrinsicFilePath;
    rectificationFilePath = pRectificationFilePath;
    
    for (auto i = 0; i < chessboardHeight; i++)
        for (auto j = 0; j < chessboardWidth; i++)
            objectPoint.push_back(cv::Point3f(i * squareSize, j * squareSize, 0));

    char key = -1;
    bool foundLeft = false;
    bool foundRight = false;
    uint16_t imageCounter = 0;

    // StereoCamera cameraLeft = StereoCamera::StereoCamera(0);
    // StereoCamera cameraRight = StereoCamera::StereoCamera(1);

    std::cout << "Stage - IDLE. This stage is intended for your preparation. Please press one of the following buttons to continue: " << std::endl;
    std::cout << "  n: continue to the next stage" << std::endl;
    std::cout << "  x: exit" << std::endl;

    cv::namedWindow("Left Frame");
    cv::namedWindow("Right Frame");

    while (true) {
        imageLeft = stereoCamera.getLeftFrame();
        imageRight = stereoCamera.getRightFrame();

        cv::imshow("Left Frame", grayLeft);
        cv::imshow("Right Frame", grayRight);

        key = cv::waitKey(1);
        if (key == 'x') std::exit(0);
        if (key == 'n') {
            cv::destroyAllWindows();
            StereoCalibrator::detectChessboard();
        }
    }

    
}

void StereoCalibrator::detectChessboard() {

    std::cout << "Stage - DETECT CHESSBOARD" << std::endl;
    std::cout << "  n: continue to the next stage" << std::endl;
    std::cout << "  x: exit" << std::endl;
    std::cout << "  s: save the pair of images" << std::endl;
    std::cout << "  c: repeat detection without saving" << std::endl;

    cv::namedWindow("Left Frame");
    cv::namedWindow("Right Frame");

    while (true) {
        imageLeft = stereoCamera.getLeftFrame();
        imageRight = stereoCamera.getRightFrame();

        cv::cvtColor(imageLeft, grayLeft, cv::COLOR_BGR2GRAY);
        cv::cvtColor(imageRight, grayRight, cv::COLOR_BGR2GRAY);

        foundLeft = cv::findChessboardCorners(imageLeft, chessboardSize, cornersLeft, cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_FILTER_QUADS | cv::CALIB_CB_NORMALIZE_IMAGE);
        foundRight = cv::findChessboardCorners(imageRight, chessboardSize, cornersRight, cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_FILTER_QUADS | cv::CALIB_CB_NORMALIZE_IMAGE);

        if (foundLeft) {
            // cv::TermCriteria criteria(cv::CV_TERMCRIT_EPS | cv::CV_TERMCRIT_ITER, 30, 0.1);
            cv::cornerSubPix(grayLeft, cornersLeft, cv::Size(11, 11), cv::Size(-1, -1), criteria);
            cv::drawChessboardCorners(grayLeft, chessboardSize, cornersLeft, foundLeft);
        }

        if (foundRight) {
            // cv::TermCriteria criteria(cv::CV_TERMCRIT_EPS | cv::CV_TERMCRIT_ITER, 30, 0.1);
            cv::cornerSubPix(grayRight, cornersRight, cv::Size(11, 11), cv::Size(-1, -1), criteria);
            cv::drawChessboardCorners(grayRight, chessboardSize, cornersRight, foundRight);
        }

        cv::imshow("Left Frame", grayLeft);
        cv::imshow("Right Frame", grayRight);

        key = cv::waitKey(1);

        if (foundLeft && foundRight) key = cv::waitKey(0);
        
        if (key == 'x') {
            cv::destroyAllWindows();
            std::exit(0);
        }
        if (key == 's' && foundLeft && foundRight) {
            imagePointsLeft.push_back(cornersLeft);
            imagePointsRight.push_back(cornersRight);
            objectPoints.push_back(objectPoint);
            std::cout << "Number of stored corners: " << ++imageCounter << std::endl;
        }
        if (key == 'c') {
            // cv::destroyAllWindows();
            continue;
        }
        if (key == 'n') {
            cv::destroyAllWindows();
            StereoCalibrator::intrinsicCalibration();
        }
    }
}

void StereoCalibrator::intrinsicCalibration() {
    // cv::TermCriteria criteria(cv::CV_TERMCRIT_EPS | cv::CV_TERMCRIT_ITER, 30, 0.1);
    std::cout << "Stage - Compute Intrinsic Parameters" << std::endl;

    reprojectionErrorLeft = cv::calibrateCamera(objectPoints, imagePointsLeft, imageSize, cameraMatrixLeft, distortionMatrixLeft, rotationMatrixLeft, translationVectorLeft);
    reprojectionErrorRight = cv::calibrateCamera(objectPoints, imagePointsRight, imageSize, cameraMatrixRight, distortionMatrixRight, rotationMatrixRight, translationVectorRight);

    
    optimalCameraMatrixLeft = cv::getOptimalNewCameraMatrix(cameraMatrixLeft, distortionMatrixLeft, imageSize, alpha, imageSize, 0);
    optimalCameraMatrixRight = cv::getOptimalNewCameraMatrix(cameraMatrixRight, distortionMatrixRight, imageSize, alpha, imageSize, 0);

    std::cout << "Intrinsic calibration is finished" << std::endl;
    std::cout << "Calibration Error Left Camera: " <<  reprojectionErrorLeft;
    std::cout << "Calibration Error Right Camera: " <<  reprojectionErrorRight;

    std::cout << "Please press one of the following buttons to continue:" << std::endl;
    std::cout << "  n: continue to the next stage" << std::endl;
    std::cout << "  x: exit" << std::endl;
    std::cout << "  s: save intrinsic parameters" << std::endl;

    while (true) {

        key = cv::waitKey(1);
        
        if (key == 'x') std::exit(0);
        if (key == 's') {
            cv::FileStorage fs(intrinsicFilePath, cv::FileStorage::WRITE);

            fs << "cameraMatrixLeft" <<  cameraMatrixLeft;
            fs << "cameraMatrixRight" <<  cameraMatrixRight;
            fs << "optimalCameraMatrixLeft" <<  optimalCameraMatrixLeft;
            fs << "optimalCameraMatrixRight" <<  optimalCameraMatrixRight;
            fs << "distortionMatrixLeft" << distortionMatrixLeft;
            fs << "distortionMatrixRight" << distortionMatrixRight;

            fs.release();
        }
        if (key == 'n') StereoCalibrator::extrinsicCalibration();
    }
}

void StereoCalibrator::extrinsicCalibration() {
    std::cout << "Stage - Compute Extrinsic Parameters" << std::endl;

    double stereoReprojectionError = cv::stereoCalibrate(objectPoints, imagePointsLeft, imagePointsRight, optimalCameraMatrixLeft, distortionMatrixLeft, optimalCameraMatrixRight, distortionMatrixRight, imageSize, rotationMatrixCommon, translationVector, essentialMatrix, fundamentalMatrix, cv::CALIB_USE_INTRINSIC_GUESS, criteria);

    std::cout << "Extrinsic calibration is finished" << std::endl;
    std::cout << "Stereo calibration error: " <<  stereoReprojectionError;

    std::cout << "Please press one of the following buttons to continue:" << std::endl;
    std::cout << "  n: continue to the next stage" << std::endl;
    std::cout << "  x: exit" << std::endl;
    std::cout << "  s: save extrinsic parameters" << std::endl;

    while (true) {

        key = cv::waitKey(1);
        
        if (key == 'x') std::exit(0);
        if (key == 's') {
            cv::FileStorage fs(extrinsicFilePath, cv::FileStorage::WRITE);

            fs << "rotationMatrixCommon" <<  rotationMatrixCommon;
            fs << "translationVector" << translationVector;
            fs << "essentialMatrix" << essentialMatrix;
            fs << "fundamentalMatrix" << fundamentalMatrix;

            fs.release();
        }
        if (key == 'n') StereoCalibrator::stereoRectification();
    }
}

void StereoCalibrator::stereoRectification() {
    std::cout << "Stage - Stereo rectification" << std::endl;
    cv::stereoRectify(optimalCameraMatrixLeft, distortionMatrixLeft, optimalCameraMatrixRight, distortionMatrixRight, imageSize, rotationMatrixCommon, translationVector, rotationMatrixLeft, rotationMatrixRight, projectionMatrixLeft, projectionMatrixRight, disparityMatrix);
    std::cout << "Stereo rectification is finished" << std::endl;

    std::cout << "Undistortion..." << std::endl;
    cv::initUndistortRectifyMap(cameraMatrixLeft, distortionMatrixLeft, rotationMatrixLeft, optimalCameraMatrixLeft, imageSize, CV_32FC1, mapLeftX, mapLeftY);
    cv::initUndistortRectifyMap(cameraMatrixRight, distortionMatrixRight, rotationMatrixRight, optimalCameraMatrixRight, imageSize, CV_32FC1, mapRightX, mapRightY);
    std::cout << "Undistortion is finished" << std::endl;

    std::cout << "Result of undistortion:" << std::endl;
    std::cout << "Please press one of the following buttons to continue:" << std::endl;
    std::cout << "  n: continue to the next stage" << std::endl;
    std::cout << "  x: exit" << std::endl;
    std::cout << "  s: save extrinsic parameters" << std::endl;

    cv::namedWindow("Left Frame");
    cv::namedWindow("Right Frame");
    cv::namedWindow("Rectified pair");

    while (true) {
        imageLeft = stereoCamera.getLeftFrame();
        imageRight = stereoCamera.getRightFrame();

        cv::cvtColor(imageLeft, imageLeft, cv::COLOR_BGR2RGB);
        cv::cvtColor(imageRight, imageRight, cv::COLOR_BGR2RGB);

        cv::remap(imageLeft, undistortedImageLeft, mapLeftX, mapLeftY, cv::INTER_LINEAR);
        cv::remap(imageRight, undistortedImageRight, mapRightX, mapRightY, cv::INTER_LINEAR);
        
        // Show separately
        cv::imshow("Left Frame", imageLeft);
        cv::imshow("Right Frame", imageRight);
        
        // Show together
        rectifiedPair = cv::Mat::zeros(undistortedImageLeft.rows, undistortedImageLeft.cols*2, undistortedImageLeft.type());

        undistortedImageLeft.copyTo(rectifiedPair(cv::Range::all(), cv::Range(0, undistortedImageLeft.cols)));
        undistortedImageRight.copyTo(rectifiedPair(cv::Range::all(), cv::Range(undistortedImageLeft.cols, undistortedImageLeft.cols*2)));

        for (auto i = 0; i < undistortedImageRight.rows; i += 10) {
            cv::Point start = cv::Point(0, i);
            cv::Point end = cv::Point(rectifiedPair.cols, i);
            cv::line(rectifiedPair, start, end, cv::Scalar(0, 255, 0));  
        }

        cv::imshow("Rectified pair", rectifiedPair);

        key = cv::waitKey(1);
        
        if (key == 'x') { 
            cv::destroyAllWindows();
            std::exit(0);
        }
        if (key == 's') {
            cv::FileStorage fs(rectificationFilePath, cv::FileStorage::WRITE);

            fs << "rotationMatrixLeft" <<  rotationMatrixLeft;
            fs << "rotationMatrixRight" <<  rotationMatrixRight;
            fs << "projectionMatrixLeft" << projectionMatrixLeft;
            fs << "projectionMatrixRight" << projectionMatrixRight;
            fs << "disparityMatrix" << disparityMatrix;
            fs << "mapLeftX" << mapLeftX;
            fs << "mapLeftY" << mapLeftY;
            fs << "mapRightX" << mapRightX;
            fs << "mapRightY" << mapRightY;

            fs.release();
        }
        if (key == 'n') { 
            cv::destroyAllWindows();
            std::cout << "Stereo Calibration stage is finished" << std::endl;
        }
    }
}


