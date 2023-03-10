#include "Disparity.h"
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>

Disparity::Disparity(const StereoCamera& stereoCamera): stereoCamera(stereoCamera) {
}

Disparity::~Disparity() {}


void Disparity::loadParameters(const std::string& intrinsicFilePath, const std::string& extrinsicFilePath, const std::string& rectificationFilePath) {
    intrinsicFile.open(intrinsicFilePath, cv::FileStorage::READ);
    if (!intrinsicFile.isOpened()) {
        std::cerr << "Failed to open file: " << intrinsicFilePath <<std::endl;
        exit(1);
    }

    
    // extrinsicFile.open(extrinsicFilePath, cv::FileStorage::READ);
    // if (!extrinsicFile.isOpened()) {
    //     std::cerr << "Failed to open file: " << extrinsicFilePath <<std::endl;
    //     exit(1);
    // }
    
    
    rectificationFile.open(rectificationFilePath, cv::FileStorage::READ);
    if (!rectificationFile.isOpened()) {
        std::cerr << "Failed to open file: " << rectificationFilePath <<std::endl;
        exit(1);
    }

    intrinsicFile["optimalCameraMatrixLeft"] >> optimalCameraMatrixLeft;

    rectificationFile["mapLeftX"] >> mapLeftX;
    rectificationFile["mapLeftY"] >> mapLeftY;
    rectificationFile["mapRightX"] >> mapRightX;
    rectificationFile["mapRightY"] >> mapRightY;
    rectificationFile["disparityMatrix"] >> disparityMatrix;
    
}

void Disparity::computeDepth(const std::string& intrinsicFilePath, const std::string& extrinsicFilePath, const std::string& rectificationFilePath) {

    loadParameters(intrinsicFilePath, extrinsicFilePath, rectificationFilePath);

    std::cout << "Stage - Depth calculation" << std::endl;
    // std::cout << "  n: continue to the next stage" << std::endl;
    std::cout << "  x: exit" << std::endl;
    // std::cout << "  s: save the pair of images" << std::endl;
    // std::cout << "  c: repeat detection without saving" << std::endl;

    cv::namedWindow("Disparity map");

    minDisparity = 0;
    numDisparities = 5*16;
    blockSize = 1;
    P1 = 8 * imageLeft.channels() * blockSize * blockSize;
    P2 = 32 * imageLeft.channels() * blockSize * blockSize;
    disp12MaxDiff = 1;
    uniquenessRatio = 10;
    speckleWindowSize = 100;
    speckleRange = 32;


    lambda = 8000.0;
    sigma = 1.5;
    
    sgbm = cv::StereoSGBM::create(minDisparity, numDisparities, blockSize, P1, P2, disp12MaxDiff, uniquenessRatio, speckleWindowSize, speckleRange, mode);
    cv::Ptr<cv::ximgproc::DisparityWLSFilter> wlsFilter = cv::ximgproc::createDisparityWLSFilter(sgbm);
    
    while (true) {
        imageLeft = stereoCamera.getLeftFrame();
        imageRight = stereoCamera.getRightFrame();

        cv::remap(imageLeft, rectifiedLeft, mapLeftX, mapLeftY, cv::INTER_LINEAR);
        cv::remap(imageRight, rectifiedRight, mapRightX, mapRightY, cv::INTER_LINEAR);

        sgbm->compute(rectifiedLeft, rectifiedRight, disparity);

        //TODO: right matcher?
        
        cv::cvtColor(imageLeft, grayLeft, cv::COLOR_BGR2GRAY);
        cv::cvtColor(imageRight, grayRight, cv::COLOR_BGR2GRAY);

        // disparity.convertTo(disparity8U, cv::CV_8U, 255.0 / (sgbm->getNumDisparities() * 16));

        // wlsFilter = cv::ximgproc::createDisparityWLSFilter(sgbm);
        wlsFilter->setLambda(lambda);
        wlsFilter->setSigmaColor(sigma);
        wlsFilter->filter(disparity, imageLeft, filteredDisparity);

        // Normalize the output 
        // cv::normalize(image8U, image8U, 0, 255, cv::Norm_MINMAX, cv::CV_8U);

        cv::reprojectImageTo3D(disparity, depth, disparityMatrix, false, CV_32F);
        cv::setMouseCallback("Disparity map", onMouse, this);

        cv::imshow("Disparity map", disparity);
        
        key = cv::waitKey(1);
        if (key == 'x') std::exit(0);
    }
}

void Disparity::onMouse(int event, int x, int y, int flags, void* data) {
    if (event != cv::EVENT_LBUTTONDOWN) 
        return;

    Disparity* disparityObject = static_cast<Disparity*>(data);

    cv::Point2f point(x, y);

    double depthAtPoint = disparityObject->depth.at<float>(point);

    disparityObject->cameraPoint.x = (float) ((double)x - disparityObject->optimalCameraMatrixLeft.at<double>(0, 2)) / disparityObject->optimalCameraMatrixLeft.at<double>(0, 0) * depthAtPoint;
    disparityObject->cameraPoint.y = (float) ((double)y - disparityObject->optimalCameraMatrixLeft.at<double>(1, 2)) / disparityObject->optimalCameraMatrixLeft.at<double>(1, 1) * depthAtPoint;
    disparityObject->cameraPoint.z = (float) disparityObject->disparityMatrix.at<double>(3,2) / depthAtPoint - disparityObject->disparityMatrix.at<double>(3,3);

    disparityObject->worldPoint.x = disparityObject->cameraPoint.x;
    disparityObject->worldPoint.y = disparityObject->cameraPoint.y;
    disparityObject->worldPoint.z = disparityObject->cameraPoint.z;

    std::cout << "Depth of clicked point: " << disparityObject->worldPoint.z << " meters" << std::endl;
}