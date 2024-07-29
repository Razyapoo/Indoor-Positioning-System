# Overview

Contains source code and implementation files.

**Requirements**:
- Linux operating system
    - tested on Ubuntu 22.04 LTS
- [eXtream Gradient Boost](https://github.com/dmlc/xgboost)
- [OpenCV v4.7.0](https://docs.opencv.org/4.x/d7/d9f/tutorial_linux_install.html)
- [Qt6 Framework v6.6.1](https://www.qt.io/download-qt-installer-oss?hsCtaTracking=99d9dd4f-5681-48d2-b096-470725510d34%7C074ddad0-fdef-4e53-8aa8-5e8a876d6ab4)
- [YoloV4](https://github.com/AlexeyAB/darknet) (we need only .weights and .cfg files)
    - [yolov4.weights](https://github.com/AlexeyAB/darknet/releases/download/darknet_yolo_v4_pre/yolov4.weights)
    - [yolov4.cfg](https://raw.githubusercontent.com/AlexeyAB/darknet/master/cfg/yolov4.cfg)

To automatically install all the above requirements, please use the [install.sh](/Implementation/install.sh) script. 

## Structure
```
.
├── ESP32 UWB                                # Firmware for ESP32 UWB devices
│   ├── anchorArduino                        # Firmware for Anchor
│   └── tagArduino                           # Firmware for Tag
├── IndoorPositioningSystem                  # Main GUI
├── Camera Intrinsic Calibration (Optical)   # Intrinsic camera calibration
│   └── Calibrator                           # Source code for calibration 
├── PixelToReal                              # Source code and data for PixelToReal training
└── Server                                   # Centralized server for UWB and video data recording
```