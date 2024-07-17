# Implementation

Contains source code and implementation files.

!!! IMPORTANT !!!:
- People positions are calculated in Indoor Positioning System (GUI) for all methods
    - Tags only estimate distances (ESP32 UWB) 
    - Calibrator only gives calibration parameters (Camera Intrinsic Calibration)

**Requarnments**:
- Linux operating system
    - tested on Ubuntu 22.04.4 LTS
- [eXtream Gradient Boost](https://xgboost.readthedocs.io/en/stable/install.html)
- [OpenCV v4.7.0](https://docs.opencv.org/4.x/d7/d9f/tutorial_linux_install.html)
- [Qt6 Framework v6.6.1](https://www.qt.io/download-qt-installer-oss?hsCtaTracking=99d9dd4f-5681-48d2-b096-470725510d34%7C074ddad0-fdef-4e53-8aa8-5e8a876d6ab4)
- [YoloV4](https://github.com/AlexeyAB/darknet) 
    - [yolov4-tiny.weights](https://github.com/AlexeyAB/darknet/releases/download/darknet_yolo_v4_pre/yolov4-tiny.weights)
    - [yolov4-tiny.cfg](https://raw.githubusercontent.com/AlexeyAB/darknet/master/cfg/yolov4-tiny.cfg)
    
## Structure
```
.
├── ESP32 UWB                                # Firmware for ESP32 UWB devices
│   ├── anchorArduino                        # Firmware for Anchor
│   └── tagArduino                           # Firmware for Tag
├── IndoorPositioningSystem                  # Main GUI
├── Camera Intrinsic Calibration (Optical)   # Intrinsic camera calibration
├── ├── intrinsic_parameters.xml             # Parameters prepared for using in GUI for Optical method
│   └── Calibrator                           # Source code for calibration 
├── PixelToReal                              # Source code and data for PixelToReal training
└── Server                                   # Centralized server for recording UWB and video data
```