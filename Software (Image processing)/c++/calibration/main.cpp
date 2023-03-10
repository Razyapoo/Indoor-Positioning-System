
#include "StereoCalibrator.h"
#include <string>
#include <map>

int main(int argc, char** argv) {
    std::map<std::string, std::string> options;

    options["input_camera_left"] = "2";
    options["input_camera_right"] = "4";
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

    StereoCamera stereoCamera = StereoCamera(std::stoi(options["input_camera_left"]), std::stoi(options["input_camera_right"]));

    StereoCalibrator stereoCalibrator = StereoCalibrator(stereoCamera);
    stereoCalibrator.initCameraCalibration(options["intrinsic_file_path"], options["extrinsic_file_path"], options["rectification_file_path"]);

    Disparity disparity = Disparity(stereoCamera);
    disparity.computeDepth(options["intrinsic_file_path"], options["extrinsic_file_path"], options["rectification_file_path"]);
}