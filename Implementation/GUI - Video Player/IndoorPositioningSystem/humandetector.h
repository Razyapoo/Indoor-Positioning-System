#ifndef HUMANDETECTOR_H
#define HUMANDETECTOR_H

/*********************************************** Human Detector ************************************************
 * YOLOv4 Human (People) detector
 * requires:
 *  - yolov4-tiny.cfg - or any other
 *  - yolov4-tiny.weights - or any other
 *  * they are loaded though the GUI by the user!!!
****************************************************************************************************************/

#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>

class HumanDetector
{
public:
    HumanDetector();
    ~HumanDetector();

    void initHumanDetection(const std::string &modelConfiguration, const std::string &modelWeights);
    bool isInitialized() const;
    std::pair<std::vector<cv::Rect>, std::vector<int>> detectPeople(const cv::Mat &frame, const cv::Size& detectionFrameSize);

private:
    cv::dnn::Net net;
    bool _isInitialized; // safety check
};

#endif // HUMANDETECTOR_H
