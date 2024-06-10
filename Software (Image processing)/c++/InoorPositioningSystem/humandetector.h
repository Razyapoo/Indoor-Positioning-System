#ifndef HUMANDETECTOR_H
#define HUMANDETECTOR_H

#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>

class HumanDetector
{
public:
    HumanDetector();

    void initHumanDetection(const std::string &modelConfiguration, const std::string &modelWeights);
    std::pair<std::vector<cv::Rect>, std::vector<int>> detectPeople(const cv::Mat &frame, const cv::Size& detectionFrameSize);
    // static std::pair<std::vector<std::vector<cv::Point2f>>, std::vector<int>> detectAruo(const cv::Mat &frame);
    ~HumanDetector();
    bool isInitialized() const;
private:
    // std::string modelConfiguration = "../weights/yolov4-tiny.cfg";
    // std::string modelWeights = "../weights/yolov4-tiny.weights";
    cv::dnn::Net net;
    bool _isInitialized;
};

#endif // HUMANDETECTOR_H
