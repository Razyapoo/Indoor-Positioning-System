#ifndef DISPARITY_H
#define DISPARITY_H

#include <opencv2/calib3d.hpp>
#include <opencv2/ximgproc.hpp>
#include "StereoCamera.hpp"

class Disparity {
    public:
        static cv::FileStorage intrinsicFile, extrinsicFile, rectificationFile;
        const static cv::Size imageSize;
        
        static cv::Mat imageLeft, imageRight, grayLeft, grayRight, rectifiedLeft, rectifiedRight;

        static cv::Mat optimalCameraMatrixLeft, mapLeftX, mapLeftY, mapRightX, mapRightY, disparityMatrix;
        static cv::Point3f cameraPoint, worldPoint;
        static cv::Mat points3D;

        static std::string filter;
        static cv::Ptr<cv::StereoSGBM> leftMatcher, sgbm;
        static cv::Ptr<cv::StereoMatcher> rightMatcher;
        static cv::Ptr<cv::ximgproc::DisparityWLSFilter> wlsFilter;
        
        static int minDisparity, numDisparities, blockSize, P1, P2, disp12MaxDiff, preFilterCap, uniquenessRatio, speckleWindowSize, speckleRange;
        static int mode;
        
        static double lambda, sigma; //Post-filter parameters
        
        static cv::Mat disparityLeft, disparityRight, depth, filteredDisparity, filteredDisparityVis, disparity8U, disparity8UColored;
        // static double baseline = 2;
        static uint8_t key;
        
        static void computeDepth(const std::string& intrinsicFilePath, const std::string& extrinsicFilePath, const std::string& rectificationFilePath);
        static void createTrackBars();
        static void loadParameters(const std::string& intrinsicFilePath, const std::string& extrinsicFilePath, const std::string& rectificationFilePath);
        static void onMouse(int event, int x, int y, int flags, void* data);
        static void onTrackBar(int value, void* data);
};

#endif 