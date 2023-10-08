#ifndef HUMANDETECTOR_H
#define HUMANDETECTOR_H

#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>

class HumanDetector
{
public:
    const static std::string modelConfiguration;
    const static std::string modelWeights;
    static cv::dnn::Net net;
    static cv::Mat blob;
    static std::vector<std::string> layerNames;
    static std::vector<int> outLayers;
    static std::vector<std::string> outputNames;
    static std::vector<cv::Mat> outputs;
    static std::vector<int> classIDs;
    static std::vector<float> confidences;
    static std::vector<cv::Rect> boxes;
    static std::vector<int> indices;
    static cv::Point classIDPoint;
    static double confidence;
    static int centerX, centerY, width, height, left, top;
    // static std::pair<std::vector<cv::Rect>, std::vector<int>> pair;

    static void initHumanDetection();
    static std::pair<std::vector<cv::Rect>, std::vector<int>> detectPeople(cv::Mat &frame);
};

#endif
