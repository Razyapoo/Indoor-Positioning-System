#include "HumanDetector.hpp"


void HumanDetector::initHumanDetection() {

    net = cv::dnn::readNetFromDarknet(modelConfiguration, modelWeights);

    net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
    net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
    
}

std::pair<std::vector<cv::Rect>, std::vector<int>>& HumanDetector::detectPeople(cv::Mat& frame) {
    HumanDetector::blob = cv::Mat();
    HumanDetector::classIDs.clear();
    HumanDetector::classIDs.shrink_to_fit();
    HumanDetector::confidences.clear();
    HumanDetector::confidences.shrink_to_fit();
    HumanDetector::boxes.clear();
    HumanDetector::boxes.shrink_to_fit();

    cv::dnn::blobFromImage(frame, HumanDetector::blob, 1/255.0, cv::Size(608, 608), cv::Scalar(0, 0, 0), true, false);

    HumanDetector::net.setInput(HumanDetector::blob);

    HumanDetector::layerNames = HumanDetector::net.getLayerNames();
    HumanDetector::outLayers = HumanDetector::net.getUnconnectedOutLayers();
    HumanDetector::outputNames.resize(HumanDetector::outLayers.size());
    for (size_t i = 0; i < HumanDetector::outLayers.size(); i++) HumanDetector::outputNames[i] = HumanDetector::layerNames[HumanDetector::outLayers[i] - 1];

    HumanDetector::net.forward(HumanDetector::outputs, HumanDetector::outputNames);

    for (const cv::Mat& output: HumanDetector::outputs) {
        for (size_t i = 0; i < output.rows; i++) {
            const cv::Mat& scores = output.row(i).colRange(5, output.cols);
            cv::minMaxLoc(scores, 0, &HumanDetector::confidence, 0, &HumanDetector::classIDPoint);
            if (HumanDetector::confidence > 0.5) {
                HumanDetector::centerX = static_cast<int>(output.at<float>(i, 0) * frame.cols);
                HumanDetector::centerY = static_cast<int>(output.at<float>(i, 1) * frame.rows);
                HumanDetector::width = static_cast<int>(output.at<float>(i, 2) * frame.cols);
                HumanDetector::height = static_cast<int>(output.at<float>(i, 3) * frame.rows);
                HumanDetector::left = HumanDetector::centerX - HumanDetector::width / 2;
                HumanDetector::top = HumanDetector::centerY - HumanDetector::height / 2;

                HumanDetector::classIDs.push_back(HumanDetector::classIDPoint.x);
                HumanDetector::confidences.push_back(static_cast<float>(HumanDetector::confidence));
                HumanDetector::boxes.emplace_back(HumanDetector::left, HumanDetector::top, HumanDetector::width, HumanDetector::height);
            }
        }
    }

    cv::dnn::NMSBoxes(HumanDetector::boxes, HumanDetector::confidences, 0.5, 0.4, HumanDetector::indices);
    for (size_t i = 0; i < HumanDetector::indices.size(); i++) {
        const cv::Rect& box = HumanDetector::boxes[HumanDetector::indices[i]];
        cv::rectangle(frame, box, cv::Scalar(0, 0, 255), 2);
    }
    HumanDetector::pair = std::make_pair(HumanDetector::boxes, HumanDetector::indices);
    // cv::imshow("People Detection", frame);

    return pair;

}