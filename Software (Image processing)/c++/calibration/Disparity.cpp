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
    float disparityMultiplier = 1.0f;
    bool showColored = false;
    bool applyFilter = false;
    cv::Mat floatDisp;

    sgbm = cv::StereoSGBM::create(minDisparity, numDisparities, blockSize, P1, P2, disp12MaxDiff, uniquenessRatio, speckleWindowSize, speckleRange, mode);
    // leftMatcher = cv::StereoSGBM::create(minDisparity, numDisparities, blockSize, P1, P2, disp12MaxDiff, uniquenessRatio, speckleWindowSize, speckleRange, mode);
    rightMatcher = cv::ximgproc::createRightMatcher(sgbm);
    cv::Ptr<cv::ximgproc::DisparityWLSFilter> wlsFilter = cv::ximgproc::createDisparityWLSFilter(sgbm);
    // cv::Ptr<cv::ximgproc::DisparityWLSFilter> wlsFilter = cv::ximgproc::createDisparityWLSFilter(sgbm);

    wlsFilter->setLambda(lambda);
    wlsFilter->setSigmaColor(sigma);
    
    while (true) {
        imageLeft = stereoCamera.getLeftFrame();
        imageRight = stereoCamera.getRightFrame();
        
        cv::cvtColor(imageLeft, grayLeft, cv::COLOR_BGR2GRAY);
        cv::cvtColor(imageRight, grayRight, cv::COLOR_BGR2GRAY);

        cv::remap(imageLeft, rectifiedLeft, mapLeftX, mapLeftY, cv::INTER_LINEAR);
        cv::remap(imageRight, rectifiedRight, mapRightX, mapRightY, cv::INTER_LINEAR);


        // cv::remap(grayLeft, rectifiedLeft, mapLeftX, mapLeftY, cv::INTER_LINEAR);
        // cv::remap(grayRight, rectifiedRight, mapRightX, mapRightY, cv::INTER_LINEAR);

        sgbm->compute(rectifiedLeft, rectifiedRight, disparityLeft);
        if (disparityLeft.type() == CV_16S) {
            disparityMultiplier = 16.0f;
        }
        // sgbm->compute(rectifiedRight, rectifiedLeft, disparityRight);

        // cv::cvtColor(rectifiedLeft, grayLeft, cv::COLOR_BGR2GRAY);
        // cv::cvtColor(rectifiedRight, grayRight, cv::COLOR_BGR2GRAY);

        // leftMatcher->compute(rectifiedLeft, rectifiedRight, disparityLeft);

        //TODO: right matcher?

        disparityLeft.convertTo(disparity8U, CV_8U, 255.0 / (sgbm->getNumDisparities() * 16));
        if (showColored) {
            cv::applyColorMap(disparity8U, disparity8UColored, cv::COLORMAP_TURBO);
            cv::imshow("Disparity map", disparity8UColored);
        } else {
            cv::imshow("Disparity map", disparity8U);
        }

        // cv::normalize(disparityLeft, disparityLeft, 0, 255, cv::NORM_MINMAX, CV_8U);
        // cv::normalize(disparityRight, disparityRight, 0, 255, cv::NORM_MINMAX, CV_8U);

        // disparityLeft.convertTo(image8U, CV_8U, 255.0 / (sgbm->getNumDisparities() * 16));

        // wlsFilter = cv::ximgproc::createDisparityWLSFilter(sgbm);
        if (applyFilter) {
            rightMatcher->compute(rectifiedRight, rectifiedLeft, disparityRight);
            wlsFilter->filter(disparityLeft, rectifiedLeft, filteredDisparity, disparityRight);
            cv::normalize(filteredDisparity, filteredDisparityVis, 0, 255, cv::NORM_MINMAX, CV_8UC1);

            cv::namedWindow("Filtered disparity map");
            cv::imshow("Filtered disparity map", filteredDisparityVis);
            
            filteredDisparity.convertTo(floatDisp, CV_32F, 1.0f / disparityMultiplier);
            // cv::setMouseCallback("Filtered disparity map", onMouse, this);
        } else {
            if (cv::getWindowProperty("Filtered disparity map", cv::WND_PROP_VISIBLE) >= 1) cv::destroyWindow("Filtered disparity map");
            // cv::setMouseCallback("Disparity map", onMouse, this);
            disparityLeft.convertTo(floatDisp, CV_32F, 1.0f / disparityMultiplier);
        }

        // Normalize the output 
        // cv::normalize(disparityLeft, disparityLeft, 0, 255, cv::NORM_MINMAX, CV_8UC1);

        // cv::reprojectImageTo3D(filteredDisparity, depth, disparityMatrix, false, CV_32F);
        // cv::reprojectImageTo3D(disparityLeft, depth, disparityMatrix, false, CV_32F);

        // cv::setMouseCallback("Disparity map", onMouse, this);


        // cv::imshow("Depth map", depth);

        // Calculate and display depth map
        // double baseline = stereoCamera.getBaseline();
        // double focalLength = stereoCamera.getFocalLength();
        // double depthScale = 1.0 / baseline;
        // cv::Mat depth = cv::Mat::zeros(filteredDisparity.size(), CV_32F);
        // for (int y = 0; y < filteredDisparity.rows; ++y) {
        //     for (int x = 0; x < filteredDisparity.cols; ++x) {
        //         float d = static_cast<float>(filteredDisparity.at<uchar>(y, x));
        //         if (d == 0) {
        //             depth.at<float>(y, x) = 0.0f;
        //         } else {
        //             depth.at<float>(y, x) = static_cast<float>(focalLength * depthScale / d);
        //         }
        //     }
        // }
        // // Normalize the output 
        // cv::normalize(depth, depth, 0, 255, cv::NORM_MINMAX, CV_8U);

        // cv::imshow("Depth map", depth);

        // , disparityNorm;
        // cv::normalize(disparityLeft, disparityNorm, 0, 255, cv::NORM_MINMAX, CV_8U);

        cv::reprojectImageTo3D(floatDisp, points3D, disparityMatrix, true);

        cv::Mat depthMap = cv::Mat::zeros(imageSize, CV_32F);
        // cv::Point3f t = points3D.at<cv::Point3f>(290, 260);
        // for (int i = 0; i < points3D.rows; i++) {
        //     cv::Point3f point = points3D.at<cv::Point3f>(i,0);
        //     int x = (int) point.x;
        //     int y = (int) point.y;
        //     if (x >= 0 && x <= imageSize.width && y >= 0 && y <= imageSize.height) depthMap.at<float>(y, x) = point.z;
        // }

        for (int x = 0; x < depthMap.rows; x++) 
        {
            for (int y = 0; y < depthMap.cols; y++)
            {
                cv::Vec3f point = points3D.at<cv::Vec3f>(x, y);
                depthMap.at<float>(x, y) = point[2];
            }        
        }

        // double minValue, maxValue;
        // cv::minMaxLoc(depthMap, &minValue, &maxValue);
        // cv::Mat scaledDepthMap;
        // cv::convertScaleAbs(depthMap, scaledDepthMap, 255.0 / (maxValue - minValue), -minValue);
        // cv::imshow("Depth map", scaledDepthMap);


        // normalize depth values (optional)
        // double min_val, max_val;
        // cv::minMaxLoc(depthMap, &minValue, &maxValue);
        // depthMap = 255 * (depthMap - minValue) / (maxValue - minValue);

        // // convert depth map to grayscale image (optional)
        cv::Mat depthMapGray;
        depthMap.convertTo(depthMapGray, CV_8U);
        cv::imshow("Depth map", depthMap);
        cv::imshow("Depth map gray", depthMapGray);
        cv::setMouseCallback("Depth map", onMouse, this);



        // Visualize 3D point cloud
        // pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZRGB>);
        // for (int i = 0; i < img_size.height; i++) {
        //     for (int j = 0; j < img_size.width; j++) {
        //         cv::Point3f point = points3D.at<Point3f>(i, j);
        //         if (!isnan(point.x) && !isnan(point.y) && !isnan(point.z)) {
        //         cv::Vec3b color = img_left.at<Vec3b>(i, j);
        //         pcl::PointXYZRGB pcl_point(color[2], color[1], color[0]);
        //         pcl_point.x = point.x;
        //         pcl_point.y = point.y;
        //         pcl_point.z = point.z;
        //         cloud->push_back(pcl_point);
        //         }
        //     }
        // }
        // pcl::visualization::CloudViewer viewer("Point Cloud Viewer");
        // viewer.showCloud(cloud);
        
        key = cv::waitKey(1);
        if (key == 'x') exit(0);
        if (key == 'c') showColored = !showColored; 
        if (key == 'f') applyFilter = !applyFilter;
    }
}

void Disparity::onMouse(int event, int x, int y, int flags, void* data) {
    // if (event != cv::EVENT_LBUTTONDOWN) 
    //     return;

    Disparity* disparityObject = static_cast<Disparity*>(data);

    // cv::Point2f point(x, y);

    // double depthAtPoint = disparityObject->depth.at<float>(point);

    // disparityObject->cameraPoint.x = (float) ((double)x - disparityObject->optimalCameraMatrixLeft.at<double>(0, 2)) / disparityObject->optimalCameraMatrixLeft.at<double>(0, 0) * depthAtPoint;
    // disparityObject->cameraPoint.y = (float) ((double)y - disparityObject->optimalCameraMatrixLeft.at<double>(1, 2)) / disparityObject->optimalCameraMatrixLeft.at<double>(1, 1) * depthAtPoint;
    // disparityObject->cameraPoint.z = (float) disparityObject->disparityMatrix.at<double>(3,2) / depthAtPoint - disparityObject->disparityMatrix.at<double>(3,3);

    // disparityObject->worldPoint.x = disparityObject->cameraPoint.x;
    // disparityObject->worldPoint.y = disparityObject->cameraPoint.y;
    // disparityObject->worldPoint.z = disparityObject->cameraPoint.z;

    // std::cout << "Depth of clicked point: " << disparityObject->worldPoint.z << " meters" << std::endl;

    if (event == cv::EVENT_LBUTTONDOWN) {
        // Assume points3D is a cv::Mat containing the 3D points
        cv::Point3f p = disparityObject->points3D.at<cv::Point3f>(y, x);
        std::cout << "Clicked pixel (" << x << ", " << y << ") corresponds to 3D point (" << p.x << ", " << p.y << ", " << p.z << ")" << std::endl;
    }
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

    //SGBM only
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

    // WLS
    // disparityObject->leftMatcher->setMinDisparity(disparityObject->minDisparity);
    // disparityObject->leftMatcher->setNumDisparities(disparityObject->numDisparities);
    // disparityObject->leftMatcher->setBlockSize(disparityObject->blockSize);
    // disparityObject->leftMatcher->setDisp12MaxDiff(disparityObject->disp12MaxDiff);
    // disparityObject->leftMatcher->setUniquenessRatio(disparityObject->uniquenessRatio);
    // disparityObject->leftMatcher->setSpeckleWindowSize(disparityObject->speckleWindowSize);
    // disparityObject->leftMatcher->setSpeckleRange(disparityObject->speckleRange);
    // disparityObject->leftMatcher->setPreFilterCap(disparityObject->preFilterCap);

    // disparityObject->leftMatcher->setP1(disparityObject->P1);
    // disparityObject->leftMatcher->setP2(disparityObject->P2);
}

void Disparity::createTrackBars(Disparity& disparityObject) {
    cv::namedWindow("Trackbars");

    cv::createTrackbar("Min Disparity", "Trackbars", nullptr, 455, onTrackBar, &disparityObject);
    cv::createTrackbar("Num Disparities", "Trackbars", nullptr, 60, onTrackBar, &disparityObject);
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