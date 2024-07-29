# Camera Intrinsic Calibration

The implementation of camera intrinsic calibration.

The result intrinsic parameters of a camera are:
  - Focal length
  - Optical center
  - Distortion coefficients

These parameters are used to calculate people coordinates using Optical method and undistort the image for better people detection (in Indoor Positioning System (GUI)).

**Output**

  `intrinsic_parameters_to_be_used_in_GUI.xml` - prepared intrinsic parameters to use in Indoor Positioning System (GUI). XML format!!!

## Requirements

- **Linux operating system**
  - Tested on Ubuntu 22.04 LTS
- **OpenCV v4.7.0**

## Installation

Use either the [install.sh](/Implementation/install.sh) to automatically install all the requirement dependencies, or follow these steps:
1. **Install the required dependencies:** 
  - Install basic dependencies (optional):
    ```sh
    sudo apt-get update && upgrade
    sudo apt-get install -y build-essential cmake g++ wget unzip git ninja-build \
      pkg-config libgtk-3-dev libeigen3-dev \
      libavcodec-dev libavformat-dev libswscale-dev libv4l-dev \
      libxvidcore-dev libx264-dev libjpeg-dev libpng-dev libtiff-dev \
      openexr libatlas-base-dev \
      libtbb2 libtbb-dev libxcb-xinerama0 libxcb-cursor0
    ```

  - **Install OpenCV:**
    ```sh
    sudo apt install -y libopencv-contrib-dev libopencv-dev libopencv-core-dev libopencv-dnn-dev
    ```
    
2. **Build and run the application:**
  ```sh
    # Assuming we are in Camera Intrinsic Calibration (Optical)/ folder
    mkdir build && cd build
    cmake ..
    make -j4
    # Run the application
    ./Calibration
  ```

## Usage

Detailed instructions on how to use the camera calibration can be found in [Wiki](https://github.com/Razyapoo/Master-Thesis/wiki/Camera-Intrinsic-Calibration)

## Structure of the folder
```
.
├── Calibrator                                  
│   ├── build                                   # Build folder for cmake
│   ├── Calibrator.cpp                          # Perfroms calibration
│   ├── Calibrator.h                             
│   ├── Camera.cpp                              # Provides access to the camera
│   ├── Camera.h                                
│   ├── CMakeLists.txt                          
│   └── main.cpp                                # Init Calibrator and Camera
├── intrinsic_parameters_to_be_used_in_GUI.xml  # Intrinsic Parameters ready to use in IPS (GUI)
└── README.md
```
