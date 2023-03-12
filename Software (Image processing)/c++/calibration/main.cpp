
#include "StereoCalibrator.h"
#include <string>
#include <map>
#include <fstream>

int main(int argc, char** argv) {
    std::map<std::string, std::string> options;
    bool skipCalibration = false;

    options["input_camera_left"] = "2";
    options["input_camera_right"] = "4";

    // options["input_camera_left"] = "rtsp://admin:Nera1998&@192.168.1.11:554/";
    // options["input_camera_right"] = "rtsp://admin:Nera1998&@192.168.1.12:554/";

    options["intrinsic_file_path"] = "parameters/intrinsic_parameters.xml";
    options["extrinsic_file_path"] = "parameters/extrinsic_parameters.xml";
    options["rectification_file_path"] = "parameters/rectification_parameters.xml";

    options["image_path"] = "images";

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg.substr(0,2) == "--") {
            std::string option = arg.substr(2);
            if (options.find(option) != options.end()) {
                if (i + 1 < argc) {
                    options[option] = argv[i + 1];
                    i++;
                } else {
                    std::cerr << "Error: " << option << " option requires a value" << std::endl;
                    return 1; 
                }
            } else {
                std::cerr << "Error: " << option << " option is unknown" << std::endl;
                return 1; 
            }
        } else {
            std::cerr << "Error: " << arg << " is invalid argument" << std::endl;
            return 1; 
        }
    }

    std::ifstream intrinsicParametersFile(options["intrinsic_file_path"]);
    std::ifstream extrinsicParametersFile(options["extrinsic_file_path"]);
    std::ifstream rectificationParametersFile(options["rectification_file_path"]);

    if (intrinsicParametersFile && extrinsicParametersFile && rectificationParametersFile) {
        std::cout << "All necessary files with parameters exist. Would you like to load existing parameters and skip calibration stage? (Y/n): " << std::endl;
        std::string input;
        std::getline(std::cin, input);
        if (input == "Y") skipCalibration = true;
        else if (input == "n") skipCalibration = false;
        else {
            std::cout << "Invalid option" << std::endl;
            exit(0); 
        }
    }

    // Web camera
    StereoCamera stereoCamera = StereoCamera(std::stoi(options["input_camera_left"]), std::stoi(options["input_camera_right"]));

    // IP Camera
    // StereoCamera stereoCamera = StereoCamera(options["input_camera_left"], options["input_camera_right"]);
    
    if (!skipCalibration) {
        StereoCalibrator stereoCalibrator = StereoCalibrator(stereoCamera);
        stereoCalibrator.initCameraCalibration(options["intrinsic_file_path"], options["extrinsic_file_path"], options["rectification_file_path"]);
    }

    Disparity disparity = Disparity(stereoCamera);
    Disparity::createTrackBars(disparity);
    disparity.computeDepth(options["intrinsic_file_path"], options["extrinsic_file_path"], options["rectification_file_path"]);
}