
#include "Calibrator.hpp"
#include <string>
#include <map>
#include <fstream>

int main(int argc, char **argv)
{
    std::map<std::string, std::string> options;

    uint8_t key;

    options["input_camera"] = "2";

    options["intrinsic_file_path"] = "parameters/intrinsic_parameters.xml";

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


    char input;
    cv::Mat frame, resizedFrame;


    Camera::initCamera(std::stoi(options["input_camera"]));

    Calibrator::intrinsicFilePath = options["intrinsic_file_path"];
    Calibrator::initCameraCalibration();
    
}
