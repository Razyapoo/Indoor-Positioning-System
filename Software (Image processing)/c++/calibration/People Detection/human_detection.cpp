#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include "StereoCamera.h"   

int main() {
    std::string modelConfiguration = "../weights/yolov4-tiny.cfg";
    std::string modelWeights = "../weights/yolov4-tiny.weights";
    cv::dnn::Net net = cv::dnn::readNetFromDarknet(modelConfiguration, modelWeights);

    // net.setPreferableBackend(cv::dnn::DNN_BACKEND_CUDA);
    // net.setPreferableBackend(cv::dnn::DNN_TARGET_CUDA);
    net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
    net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);

    // StereoCamera stereoCamera = StereoCamera(2, 4);
    cv::VideoCapture cam(2);
    cv::Mat leftFrame;

    cv::namedWindow("People Detection");

    while (true) {
        if (!cam.read(leftFrame)) {
            throw std::runtime_error("Failed to read the frame from the camera " + std::to_string(2));
        }


        // cv::Mat rightFrame = stereoCamera.getRightFrame();

        cv::Mat leftBlob, rightBlob; 
        cv::dnn::blobFromImage(leftFrame, leftBlob, 1/255.0, cv::Size(608, 608), cv::Scalar(0, 0, 0), true, false);
        // cv::dnn::blobFromImage(rightFrame, rightBlob, 1/255.0, cv::Size(608, 608), cv::Scalar(0, 0, 0), true, false);

        net.setInput(leftBlob);

        std::vector<std::string> layerNames = net.getLayerNames();
        std::vector<int> outLayers = net.getUnconnectedOutLayers();
        std::vector<std::string> outputNames(outLayers.size());
        for (size_t i = 0; i < outLayers.size(); ++i) outputNames[i] = layerNames[outLayers[i] - 1];

        std::vector<cv::Mat> outputs;
        net.forward(outputs, outputNames);

        std::vector<int> classIDs;
        std::vector<float> confidences; 
        std::vector<cv::Rect> boxes;

        for (const auto& output: outputs) {
            for (size_t i = 0; i < output.rows; ++i) {
                const auto& scores = output.row(i).colRange(5, output.cols);
                cv::Point classIDPoint;
                double confidence;
                cv::minMaxLoc(scores, 0, &confidence, 0, &classIDPoint);
                if (confidence > 0.5) {
                    int centerX = static_cast<int>(output.at<float>(i, 0) * leftFrame.cols);
                    int centerY = static_cast<int>(output.at<float>(i, 1) * leftFrame.rows);
                    int width = static_cast<int>(output.at<float>(i, 2) * leftFrame.cols);
                    int height = static_cast<int>(output.at<float>(i, 3) * leftFrame.rows);
                    int left = centerX - width / 2;
                    int top = centerY - height / 2;

                    classIDs.push_back(classIDPoint.x);
                    confidences.push_back(static_cast<float>(confidence));
                    boxes.emplace_back(left, top, width, height);
                }
            }
        }

        std::vector<int> indices;
        cv::dnn::NMSBoxes(boxes, confidences, 0.5, 0.4, indices);
        for (size_t i = 0; i < indices.size(); ++i) {
            int idx = indices[i];
            const cv::Rect& box = boxes[idx];
            cv::rectangle(leftFrame, box, cv::Scalar(0, 0, 255), 2);
        }
        
        cv::imshow("People Detection", leftFrame);

        int key = cv::waitKey(1);
        if (key == 'q') {
            cv::destroyAllWindows();
            return 0;
        }
    }

    cv::destroyAllWindows();
    return 0;
}