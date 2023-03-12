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
    cv::namedWindow("Depth map");

    minDisparity = 0;
    numDisparities = 5*16;
    blockSize = 1;
    P1 = 8 * 3 * blockSize * blockSize;
    P2 = 32 * 3 * blockSize * blockSize;
    disp12MaxDiff = 1;
    uniquenessRatio = 10;
    speckleWindowSize = 100;
    speckleRange = 32;

    lambda = 8000.0;
    sigma = 1.5;
    mode = cv::StereoSGBM::MODE_SGBM_3WAY;

    sgbm = cv::StereoSGBM::create(minDisparity, numDisparities, blockSize, P1, P2, disp12MaxDiff, uniquenessRatio, speckleWindowSize, speckleRange, mode);
    // leftMatcher = cv::StereoSGBM::create(minDisparity, numDisparities, blockSize, P1, P2, disp12MaxDiff, uniquenessRatio, speckleWindowSize, speckleRange, mode);
    // rightMatcher = cv::ximgproc::createRightMatcher(leftMatcher);
    // cv::Ptr<cv::ximgproc::DisparityWLSFilter> wlsFilter = cv::ximgproc::createDisparityWLSFilter(leftMatcher);
    cv::Ptr<cv::ximgproc::DisparityWLSFilter> wlsFilter = cv::ximgproc::createDisparityWLSFilter(sgbm);

    wlsFilter->setLambda(lambda);
    wlsFilter->setSigmaColor(sigma);
    
    while (true) {
        imageLeft = stereoCamera.getLeftFrame();
        imageRight = stereoCamera.getRightFrame();


        cv::remap(imageLeft, rectifiedLeft, mapLeftX, mapLeftY, cv::INTER_LINEAR);
        cv::remap(imageRight, rectifiedRight, mapRightX, mapRightY, cv::INTER_LINEAR);

        sgbm->compute(rectifiedLeft, rectifiedRight, disparityLeft);
        sgbm->compute(rectifiedRight, rectifiedLeft, disparityRight);

        // cv::cvtColor(rectifiedLeft, grayLeft, cv::COLOR_BGR2GRAY);
        // cv::cvtColor(rectifiedRight, grayRight, cv::COLOR_BGR2GRAY);

        // leftMatcher->compute(grayLeft, grayRight, disparityLeft);
        // rightMatcher->compute(grayRight, grayLeft, disparityRight);

        //TODO: right matcher?
        
        // cv::normalize(grayLeft, grayLeft, 0, 255, cv::NORM_MINMAX, CV_8U);
        // cv::normalize(grayRight, grayRight, 0, 255, cv::NORM_MINMAX, CV_8U);

        // disparityLeft.convertTo(image8U, CV_8U, 255.0 / (sgbm->getNumDisparities() * 16));

        // wlsFilter = cv::ximgproc::createDisparityWLSFilter(sgbm);
        // wlsFilter->filter(disparityLeft, grayLeft, filteredDisparity, disparityRight);

        // Normalize the output 
        // cv::normalize(filteredDisparity, filteredDisparity, 0, 255, cv::NORM_MINMAX, CV_8U);
        cv::normalize(disparityLeft, disparityLeft, 0, 255, cv::NORM_MINMAX, CV_8UC1);

        // cv::reprojectImageTo3D(filteredDisparity, depth, disparityMatrix, false, CV_32F);
        cv::reprojectImageTo3D(disparityLeft, depth, disparityMatrix, false, CV_32F);

        // cv::setMouseCallback("Disparity map", onMouse, this);

        cv::imshow("Disparity map", disparityLeft);
        // cv::imshow("Disparity map", filteredDisparity);

        cv::imshow("Depth map", depth);
        
        key = cv::waitKey(1);
        if (key == 'x') exit(0);
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

void Disparity::onTrackBar(int value, void* data) {

    Disparity* disparityObject = static_cast<Disparity*>(data);

    disparityObject->minDisparity = cv::getTrackbarPos("Min Disparity", "Trackbars") - 200;
    disparityObject->numDisparities = (cv::getTrackbarPos("Num Disparities", "Trackbars") == 0) ? 16 : 16 * cv::getTrackbarPos("Num Disparities", "Trackbars");
    disparityObject->blockSize = cv::getTrackbarPos("Block Size", "Trackbars");
    disparityObject->disp12MaxDiff = cv::getTrackbarPos("Disp12 Max Diff", "Trackbars");
    disparityObject->uniquenessRatio = cv::getTrackbarPos("Uniqueness Ratio", "Trackbars");
    disparityObject->speckleWindowSize = cv::getTrackbarPos("Speckle Window Size", "Trackbars");
    disparityObject->speckleRange = cv::getTrackbarPos("Speckle Range", "Trackbars");
    disparityObject->preFilterCap = cv::getTrackbarPos("Pre Filter Cap", "Trackbars");

    disparityObject->P1 = 8 * 3 * disparityObject->blockSize * disparityObject->blockSize;
    disparityObject->P2 = 32 * 3 * disparityObject->blockSize * disparityObject->blockSize;

    disparityObject->sgbm->setMinDisparity(disparityObject->minDisparity);
    disparityObject->sgbm->setNumDisparities(disparityObject->numDisparities);
    disparityObject->sgbm->setBlockSize(disparityObject->blockSize);
    disparityObject->sgbm->setDisp12MaxDiff(disparityObject->disp12MaxDiff);
    disparityObject->sgbm->setUniquenessRatio(disparityObject->uniquenessRatio);
    disparityObject->sgbm->setSpeckleWindowSize(disparityObject->speckleWindowSize);
    disparityObject->sgbm->setSpeckleRange(disparityObject->speckleRange);
    disparityObject->sgbm->setPreFilterCap(disparityObject->preFilterCap);

    disparityObject->sgbm->setP1(disparityObject->P1);
    disparityObject->sgbm->setP2(disparityObject->P2);
}

void Disparity::createTrackBars(Disparity& disparityObject) {
    cv::namedWindow("Trackbars");

    cv::createTrackbar("Min Disparity", "Trackbars", nullptr, 255, onTrackBar, &disparityObject);
    cv::createTrackbar("Num Disparities", "Trackbars", nullptr, 16, onTrackBar, &disparityObject);
    cv::createTrackbar("Block Size", "Trackbars", nullptr, 11, onTrackBar, &disparityObject);
    cv::createTrackbar("Disp12 Max Diff", "Trackbars", nullptr, 100, onTrackBar, &disparityObject);
    cv::createTrackbar("Uniqueness Ratio", "Trackbars", nullptr, 100, onTrackBar, &disparityObject);
    cv::createTrackbar("Speckle Window Size", "Trackbars", nullptr, 300, onTrackBar, &disparityObject);
    cv::createTrackbar("Speckle Range", "Trackbars", nullptr, 50, onTrackBar, &disparityObject);
    cv::createTrackbar("Pre Filter Cap", "Trackbars", nullptr, 100, onTrackBar, &disparityObject);

    // cv::setTrackbarMin("Num Disparities", "Trackbars", 1);
    // cv::setTrackbarPos("Min Disparity", "Trackbars", disparityObject.minDisparity);
    // cv::setTrackbarPos("Speckle Range", "Trackbars", disparityObject.speckleRange);
    // cv::setTrackbarPos("Speckle Window Size", "Trackbars", disparityObject.speckleWindowSize);
    // cv::setTrackbarPos("Num Disparities", "Trackbars", disparityObject.numDisparities);
    // cv::setTrackbarPos("Uniqueness Ratio", "Trackbars", disparityObject.uniquenessRatio);
    // cv::setTrackbarPos("Disp12 Max Diff", "Trackbars", disparityObject.disp12MaxDiff);
    // cv::setTrackbarPos("Block Size", "Trackbars", disparityObject.blockSize);
}