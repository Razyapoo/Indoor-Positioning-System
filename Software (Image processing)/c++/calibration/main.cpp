
#include "StereoCalibrator.hpp"
#include "VideoManager.hpp"
#include <string>
#include <map>
#include <fstream>

int main(int argc, char **argv)
{
    std::map<std::string, std::string> options;
    bool skipCalibration = false;
    bool videoNotStream = false;
    bool recordNotLoad = false;
    uint8_t key;

    options["input_camera_left"] = "4";
    options["input_camera_right"] = "2";

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

    std::cout << "Press \"s + Enter\" to start video streaming. This will turn on the online method" << std::endl;
    std::cout << "Press \"r + Enter\" to start video recording. This will turn on the offline method" << std::endl;
    std::cout << "Press \"l + Enter\" to load recorded video. This will turn on the offline method" << std::endl;

    std::string input;
    std::getline(std::cin, input);
    if (input == "s")
    {
        videoNotStream = false;
    }
    else if (input == "r")
    {
        videoNotStream = true;
        recordNotLoad = true;
    }
    else if (input == "l")
    {
        videoNotStream = true;
        recordNotLoad = false;
    }
    else
    {
        std::cout << "Invalid option" << std::endl;
        return -1;
    }

    if (videoNotStream)
    {
        if (recordNotLoad)
        {
            // Using webcameras
            StereoCamera::initStereoCamera(std::stoi(options["input_camera_left"]), std::stoi(options["input_camera_right"]));
            VideoManager::videoRecorder();
        }
    }
    else
    {
        // Web camera
        // StereoCamera stereoCamera = StereoCamera(std::stoi(options["input_camera_left"]), std::stoi(options["input_camera_right"]));

        // IP Camera
        // StereoCamera::initStereoCamera(options["input_camera_left"], options["input_camera_right"]);
        // StereoCamera stereoCamera = StereoCamera(options["input_camera_left"], options["input_camera_right"]);
        if (videoNotStream)
        {
            StereoCalibrator::videoAsSource = true;
            StereoCalibrator::leftVideoSource = cv::VideoCapture("video_from_left_camera.avi");
            StereoCalibrator::rightVideoSource = cv::VideoCapture("video_from_right_camera.avi");
        }
        else
        {
            StereoCalibrator::videoAsSource = false;
            StereoCamera::initStereoCamera(std::stoi(options["input_camera_left"]), std::stoi(options["input_camera_right"]));
        }

        if (!skipCalibration)
        {
            StereoCalibrator::intrinsicFilePath = options["intrinsic_file_path"];
            StereoCalibrator::extrinsicFilePath = options["extrinsic_file_path"];
            StereoCalibrator::rectificationFilePath = options["rectification_file_path"];

            StereoCalibrator::initCameraCalibration();
        }

        Disparity::createTrackBars();
        Disparity::computeDepth(options["intrinsic_file_path"], options["extrinsic_file_path"], options["rectification_file_path"]);
    }

    StereoCamera::release();
}