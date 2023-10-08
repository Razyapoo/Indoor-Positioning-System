#include "HumanDetector.hpp"

const std::string HumanDetector::modelConfiguration = "../weights/yolov4-tiny.cfg";
const std::string HumanDetector::modelWeights = "../weights/yolov4-tiny.weights";

cv::dnn::Net HumanDetector::net;
cv::Mat HumanDetector::blob;
std::vector<std::string> HumanDetector::layerNames, HumanDetector::outputNames;
std::vector<int> HumanDetector::outLayers, HumanDetector::classIDs, HumanDetector::indices;
std::vector<cv::Mat> HumanDetector::outputs;
std::vector<float> HumanDetector::confidences;
std::vector<cv::Rect> HumanDetector::boxes;
cv::Point HumanDetector::classIDPoint;
double HumanDetector::confidence;
int HumanDetector::centerX, HumanDetector::centerY, HumanDetector::width, HumanDetector::height, HumanDetector::left, HumanDetector::top;
// std::pair<std::vector<cv::Rect>, std::vector<int>> HumanDetector::pair;

void HumanDetector::initHumanDetection()
{

    net = cv::dnn::readNetFromDarknet(modelConfiguration, modelWeights);

    net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
    net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
}

std::pair<std::vector<cv::Rect>, std::vector<int>> HumanDetector::detectPeople(cv::Mat &frame)
{
    blob = cv::Mat();
    classIDs.clear();
    classIDs.shrink_to_fit();
    confidences.clear();
    confidences.shrink_to_fit();
    boxes.clear();
    boxes.shrink_to_fit();

    cv::dnn::blobFromImage(frame, blob, 1 / 255.0, cv::Size(608, 608), cv::Scalar(0, 0, 0), true, false);

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
                left = centerX - width / 2;
                top = centerY - height / 2;

                classIDs.push_back(classIDPoint.x);
                confidences.push_back(static_cast<float>(confidence));
                boxes.emplace_back(left, top, width, height);
            }
        }
    }

    cv::dnn::NMSBoxes(boxes, confidences, 0.5, 0.4, indices);
    for (size_t i = 0; i < indices.size(); i++)
    {
        const cv::Rect &box = boxes[indices[i]];
        cv::rectangle(frame, box, cv::Scalar(0, 0, 255), 2);
    }
    std::pair<std::vector<cv::Rect>, std::vector<int>> pair = std::make_pair(boxes, indices);
    // cv::imshow("People Detection", frame);

    return pair;
}