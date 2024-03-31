
#include "StereoCalibrator.hpp"
#include "VideoManager.hpp"
#include "HumanDetector.hpp"
#include <string>
#include <map>
#include <fstream>

int main(int argc, char **argv)
{
    std::map<std::string, std::string> options;
    bool skipCalibration = false;

    uint8_t key;

    options["input_camera_left"] = "2";
    // options["input_camera_right"] = "4";

    // options["input_camera_left"] = "rtsp://admin:Nera1998&@192.168.1.11:554/";
    // options["input_camera_right"] = "rtsp://admin:Nera1998&@192.168.1.12:554/";

    options["intrinsic_file_path"] = "parameters/intrinsic_parameters.xml";
    options["extrinsic_file_path"] = "parameters/extrinsic_parameters.xml";
    options["rectification_file_path"] = "parameters/rectification_parameters.xml";

    options["image_path"] = "images";

    for (int i = 1; i < argc; i++)
    {
        std::string arg = argv[i];
        if (arg.substr(0, 2) == "--")
        {
            std::string option = arg.substr(2);
            if (options.find(option) != options.end())
            {
                if (i + 1 < argc)
                {
                    options[option] = argv[i + 1];
                    i++;
                }
                else
                {
                    std::cerr << "Error: " << option << " option requires a value" << std::endl;
                    return 1;
                }
            }
            else
            {
                std::cerr << "Error: " << option << " option is unknown" << std::endl;
                return 1;
            }
        }
        else
        {
            std::cerr << "Error: " << arg << " is invalid argument" << std::endl;
            return 1;
        }
    }

    std::ifstream intrinsicParametersFile(options["intrinsic_file_path"]);
    std::ifstream extrinsicParametersFile(options["extrinsic_file_path"]);
    std::ifstream rectificationParametersFile(options["rectification_file_path"]);

    if (intrinsicParametersFile && extrinsicParametersFile && rectificationParametersFile)
    {
        std::cout << "All necessary files with parameters exist. Would you like to load existing parameters and skip calibration stage? (Y/n) + Enter: " << std::endl;
        std::string input;
        std::getline(std::cin, input);
        if (input == "Y")
            skipCalibration = true;
        else if (input == "n")
            skipCalibration = false;
        else
        {
            std::cout << "Invalid option" << std::endl;
            return -1;
        }
    }

    char input;
    bool doubleCamera;
    std::cout << "Press \"s + Enter\" to record from single camera" << std::endl;
    std::cout << "Press \"d + Enter\" to record from stereo camera" << std::endl;

    std::cin >> input;

    if (input == 's')
    {
        doubleCamera = false;
    }

    if (input == 'd')
    {
        doubleCamera = true;
    }

    std::cout << "Press \"s + Enter\" to start video streaming. This will turn on the online method" << std::endl;
    std::cout << "Press \"r + Enter\" to start video recording. This will turn on video recording only" << std::endl;
    std::cout << "Press \"l + Enter\" to load recorded video. This will turn on the offline method" << std::endl;
    std::cout << "Press \"d + Enter\" to start people detection. This will turn on online people detection only" << std::endl;

    cv::Mat frame, resizedFrame;
    std::pair<std::vector<cv::Rect>, std::vector<int>> detectedPeople;
    const std::string modelConfiguration = "../weights/yolov4-tiny.cfg";
    const std::string modelWeights = "../weights/yolov4-tiny.weights";
    std::cin >> input;

    switch (input)
    {
    case 's': // stream
        // StereoCalibrator::videoAsSource = false;
        // StereoCamera::initStereoCamera(std::stoi(options["input_camera_left"]), std::stoi(options["input_camera_right"]));
        if (doubleCamera)
        {
            StereoCamera::initStereoCamera(std::stoi(options["input_camera_left"]), std::stoi(options["input_camera_right"]));
        }
        else
        {
            StereoCamera::initSingleCamera(std::stoi(options["input_camera_left"]));
        }
        //        HumanDetector::initHumanDetection(modelConfiguration, modelWeights);
        if (!skipCalibration)
        {
            StereoCalibrator::intrinsicFilePath = options["intrinsic_file_path"];
            StereoCalibrator::extrinsicFilePath = options["extrinsic_file_path"];
            StereoCalibrator::rectificationFilePath = options["rectification_file_path"];

            StereoCalibrator::initCameraCalibration();
        }
        Disparity::createTrackBars();
        Disparity::computeDepth(options["intrinsic_file_path"], options["extrinsic_file_path"], options["rectification_file_path"]);
        break;

    case 'r': // record

        if (doubleCamera)
        {
            StereoCamera::initStereoCamera(std::stoi(options["input_camera_left"]), std::stoi(options["input_camera_right"]));
        }
        else
        {
            StereoCamera::initSingleCamera(std::stoi(options["input_camera_left"]));
        }

        VideoManager::videoRecorder(doubleCamera);
        break;

    case 'l': // load video
        // StereoCalibrator::videoAsSource = true;
        StereoCalibrator::leftVideoSource = cv::VideoCapture("video_from_left_camera.avi");
        StereoCalibrator::rightVideoSource = cv::VideoCapture("video_from_right_camera.avi");
        if (!skipCalibration)
        {
            StereoCalibrator::intrinsicFilePath = options["intrinsic_file_path"];
            StereoCalibrator::extrinsicFilePath = options["extrinsic_file_path"];
            StereoCalibrator::rectificationFilePath = options["rectification_file_path"];

            StereoCalibrator::initCameraCalibration();
        }
        Disparity::createTrackBars();
        Disparity::computeDepth(options["intrinsic_file_path"], options["extrinsic_file_path"], options["rectification_file_path"]);
        break;

    case 'd': // online people detection only
        // StereoCalibrator::videoAsSource = false;

        StereoCamera::initStereoCamera(std::stoi(options["input_camera_left"]), std::stoi(options["input_camera_right"]));
        HumanDetector::initHumanDetection(modelConfiguration, modelWeights);
        cv::namedWindow("Frame");

        while (true)
        {
            frame = StereoCamera::getLeftFrame();
            cv::resize(frame, resizedFrame, cv::Size(1280, 720));
            detectedPeople = HumanDetector::detectPeople(resizedFrame);
            // detectPeople(imageLeft);

            if (!detectedPeople.first.empty() && !detectedPeople.second.empty())
            {
                for (size_t i = 0; i < detectedPeople.second.size(); i++)
                {
                    int idx = detectedPeople.second[i];
                    const cv::Rect &box = detectedPeople.first[idx];
                    cv::rectangle(resizedFrame, box, cv::Scalar(0, 0, 255), 2);
                }

                cv::imshow("Frame", resizedFrame);
            }
            else
            {
                std::cout << "No people detected. \n";
            }

            key = cv::waitKey(1);
            if (key == 'x')
                exit(0);
        }

        break;

    default:
        std::cout << "Invalid option \n";
        break;
    }

    StereoCamera::release();
}
