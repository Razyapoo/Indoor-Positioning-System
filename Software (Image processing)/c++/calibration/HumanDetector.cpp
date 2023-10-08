#include "HumanDetector.hpp"

// const std::string HumanDetector::modelConfiguration = "../weights/yolov4-tiny.cfg";
// const std::string HumanDetector::modelWeights = "../weights/yolov4-tiny.weights";

cv::dnn::Net HumanDetector::net;
// cv::Mat HumanDetector::blob;
// std::vector<std::string> HumanDetector::layerNames, HumanDetector::outputNames;
// std::vector<int> HumanDetector::outLayers, HumanDetector::classIDs, HumanDetector::indices;
// std::vector<cv::Mat> HumanDetector::outputs;
// std::vector<float> HumanDetector::confidences;
// std::vector<cv::Rect> HumanDetector::boxes;
// cv::Point HumanDetector::classIDPoint;
// double HumanDetector::confidence;
// int HumanDetector::centerX, HumanDetector::centerY, HumanDetector::width, HumanDetector::height, HumanDetector::left, HumanDetector::top;

void HumanDetector::initHumanDetection(const std::string &modelConfiguration, const std::string &modelWeights)
{
    net = cv::dnn::readNetFromDarknet(modelConfiguration, modelWeights);

    net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
    net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
}

std::pair<std::vector<cv::Rect>, std::vector<int>> HumanDetector::detectPeople(const cv::Mat &frame)
{
    cv::Mat blob = cv::Mat();
    std::vector<int> outLayers, indices;
    std::vector<float> confidences;
    std::vector<cv::Rect> boxes;
    std::vector<std::string> layerNames, outputNames;
    std::vector<cv::Mat> outputs;
    cv::Point classIDPoint;
    double confidence;
    int centerX, centerY, width, height, left, top, classID;

    // classIDs.clear();
    // classIDs.shrink_to_fit();
    // confidences.clear();
    // confidences.shrink_to_fit();
    // boxes.clear();
    // boxes.shrink_to_fit();

    cv::dnn::blobFromImage(frame, blob, 1 / 255.0, cv::Size(416, 416), cv::Scalar(0, 0, 0), true, false);

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

                int classID = classIDPoint.x;
                if (classID == 0)
                {
                    confidences.push_back(static_cast<float>(confidence));
                    boxes.emplace_back(left, top, width, height);
                }
            }
        }
    }

    cv::dnn::NMSBoxes(boxes, confidences, 0.5, 0.4, indices);
    // for (size_t i = 0; i < indices.size(); i++)
    // {
    //     const cv::Rect &box = boxes[indices[i]];
    //     cv::rectangle(frame, box, cv::Scalar(0, 0, 255), 2);
    // }
    std::pair<std::vector<cv::Rect>, std::vector<int>> pair = std::make_pair(boxes, indices);
    // cv::imshow("People Detection", frame);

    return pair;
}

std::pair<std::vector<std::vector<cv::Point2f>>, std::vector<int>> HumanDetector::detectAruco(const cv::Mat &frame)
{
    cv::Ptr<cv::aruco::Dictionary> dict = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_7X7_250);

    std::vector<std::vector<cv::Point2f>> corners;
    std::vector<int> ids;
    cv::aruco::detectMarkers(frame, dict, corners, ids);

    std::pair<std::vector<std::vector<cv::Point2f>>, std::vector<int>> pair = std::make_pair(corners, ids);
    return pair;
}