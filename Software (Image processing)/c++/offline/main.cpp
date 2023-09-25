#include "Disparity.hpp"
#include "HumanDetector.hpp"
#include "VideoManager.hpp"

int main()
{

    // Disparity::init("parameters/intrinsic_parameters.xml", "parameters/extrinsic_parameters.xml", "parameters/rectification_parameters.xml");
    // HumanDetector::initHumanDetection();

    VideoManager::videoLoader("video_from_left_camera.avi", "video_from_right_camera.avi");
    // Disparity::computeDepth();
}