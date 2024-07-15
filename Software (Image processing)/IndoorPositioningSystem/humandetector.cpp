#include "humandetector.h"

HumanDetector::HumanDetector(): _isInitialized(false) {}

void HumanDetector::initHumanDetection(const std::string &modelConfiguration, const std::string &modelWeights) {

    try {
        net = cv::dnn::readNetFromDarknet(modelConfiguration, modelWeights);

        net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
        net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);

        _isInitialized = true;
    } catch (const cv::Exception &e) {
        std::cerr << "Error: Failed to load network. " << e.what() << std::endl;
        _isInitialized = false;
    }
}

HumanDetector::~HumanDetector() {}

std::pair<std::vector<cv::Rect>, std::vector<int>> HumanDetector::detectPeople(const cv::Mat &frame, const cv::Size& detectionFrameSize)
{
    cv::Mat blob = cv::Mat();
    std::vector<int> outLayers, indices;
    std::vector<float> confidences;
    std::vector<cv::Rect> boxes;
    std::vector<std::string> layerNames, outputNames;
    std::vector<cv::Mat> outputs;
    cv::Point classIDPoint;
    double confidence;
    int centerX, centerY, width, height, left, top;

    cv::dnn::blobFromImage(frame, blob, 1 / 255.0, detectionFrameSize, cv::Scalar(0, 0, 0), true, false);

    net.setInput(blob);

    layerNames = net.getLayerNames();
    outLayers = net.getUnconnectedOutLayers();
    outputNames.resize(outLayers.size());
    for (size_t i = 0; i < outLayers.size(); i++)
        outputNames[i] = layerNames[outLayers[i] - 1];

    net.forward(outputs, outputNames);

    for (const cv::Mat &output : outputs)
    {
        for (size_t i = 0; i < output.rows; i++)
        {
            const cv::Mat &scores = output.row(i).colRange(5, output.cols);
            cv::minMaxLoc(scores, 0, &confidence, 0, &classIDPoint);
            if (confidence > 0.5)
            {
                centerX = static_cast<int>(output.at<float>(i, 0) * frame.cols);
                centerY = static_cast<int>(output.at<float>(i, 1) * frame.rows);
                width = static_cast<int>(output.at<float>(i, 2) * frame.cols);
                height = static_cast<int>(output.at<float>(i, 3) * frame.rows);
                left = centerX - (width / 2);
                top = centerY - (height / 2);

                int classID = classIDPoint.x;
                if (classID == 0)
                {
                    confidences.push_back(static_cast<float>(confidence));

                    // we return only these values in order to draw rectangles;
                    // the bottom center for position estimation will be calculated later
                    boxes.emplace_back(left, top, width, height);
                }
            }
        }
    }

    cv::dnn::NMSBoxes(boxes, confidences, 0.5, 0.4, indices);
    std::pair<std::vector<cv::Rect>, std::vector<int>> pair = std::make_pair(boxes, indices);

    return pair;
}

// Safety check. If weights are not loaded. The GUI will alert the user and request to load.
bool HumanDetector::isInitialized() const {
    return _isInitialized;
}
