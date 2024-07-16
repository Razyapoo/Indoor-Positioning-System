# Implementation

This is a Main GUI which allows for:
- Play / Pause / Seek synchronized Video and estimated data (UWB, Pixel-to-Real, Optical)
- Data Analysis
- Data visualization (distances, coordinates)
- Data export

**Requarnments**:
- Linux operating system
    - tested on Ubuntu 22.04.4 LTS
- [eXtream Gradient Boost](https://xgboost.readthedocs.io/en/stable/install.html)
- [OpenCV v4.7.0](https://docs.opencv.org/4.x/d7/d9f/tutorial_linux_install.html)
- [Qt6 Framework v6.6.1](https://www.qt.io/download-qt-installer-oss?hsCtaTracking=99d9dd4f-5681-48d2-b096-470725510d34%7C074ddad0-fdef-4e53-8aa8-5e8a876d6ab4)
- [YoloV4](https://github.com/AlexeyAB/darknet) 
    - [yolov4-tiny.weights](https://github.com/AlexeyAB/darknet/releases/download/darknet_yolo_v4_pre/yolov4-tiny.weights)
    - [yolov4-tiny.cfg](https://raw.githubusercontent.com/AlexeyAB/darknet/master/cfg/yolov4-tiny.cfg)

**Expected input files in GUI**:

In the toolbar when GUI is opened:
- "File->Open video and UWB data":
    - One of the experiments available in `Master Thesis/Data for Indoor Positioning System (GUI)/`
    - As a single data stream:
        - `video.avi` or `video.mp4`
        - `video_timestamps.txt`
        - `UWB_timestamps.txt`

- "File->Load human detection model weights":
    - It was necessary to download them from GitHub
        - [yolov4-tiny.weights](https://github.com/AlexeyAB/darknet/releases/download/darknet_yolo_v4_pre/yolov4-tiny.weights)
        - [yolov4-tiny.cfg](https://raw.githubusercontent.com/AlexeyAB/darknet/master/cfg/yolov4-tiny.cfg)
    - GUI loads them as a pair (together)

- "File->Load pixel to real model params":
    - from `Master Thesis/Implementation/PixelToReal/`
        - `PixeltoReal_model_to_be_used_in_GUI.json`

- "File->Load Camera intrinsic params":
    - from `Master Thesis/Implementation/Camera Intrinsic Calibration (Optical)/`
        - `intrinsic_parameters_to_be_used_in_GUI.xml`
