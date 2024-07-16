
#include "Calibrator.h"
#include <string>
#include <map>
#include <fstream>

/*********************************************** Calibrator (main) ***********************************************
 * Camera Intrinsic Calibration
****************************************************************************************************************/

int main(int argc, char **argv)
{
    // Command line options
    std::map<std::string, std::string> options;

    uint8_t key;

    // Default options
    options["input_camera"] = "2";
    options["intrinsic_file_path"] = "parameters/intrinsic_parameters.xml";
    options["image_path"] = "images";

    // Parse options
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

    char input;
    cv::Mat frame, resizedFrame;
    // Init Camera
    Camera::initCamera(std::stoi(options["input_camera"]));

    // Init Calibrator
    Calibrator::intrinsicFilePath = options["intrinsic_file_path"];
    Calibrator::initCameraCalibration();
}
