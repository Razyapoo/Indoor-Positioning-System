# Overview

This is a Main GUI, which allows to:
- Play / Pause / Seek synchronized Video and estimated data (UWB, Pixel-to-Real, Optical)
- Data Analysis
- Data visualization (distances, coordinates)
- Data export

The main components of the program are:
   - *GUI visualization*: responsible for data visualization
   - *Data Processor*: responsible for loading the UWB, Optical, PixelToReal data and working with them 
   - *Video Processor*: responsible for working with video stream (read, play, pause, seek, etc.)

Each component is performed simultaneously in a dedicated thread for better optimization.

**Requarnments**:
- Linux operating system
   - tested on Ubuntu 22.04 LTS
   - Monitor resolution: 1920x1080 or more.
- [eXtream Gradient Boost](https://github.com/dmlc/xgboost)
- [OpenCV v4.7.0](https://docs.opencv.org/4.x/d7/d9f/tutorial_linux_install.html)
- [Qt6 Framework v6.6.1](https://www.qt.io/download-qt-installer-oss?hsCtaTracking=99d9dd4f-5681-48d2-b096-470725510d34%7C074ddad0-fdef-4e53-8aa8-5e8a876d6ab4)
- [YoloV4](https://github.com/AlexeyAB/darknet) (we need only .weights and .cfg files)
    - [yolov4.weights](https://github.com/AlexeyAB/darknet/releases/download/darknet_yolo_v4_pre/yolov4.weights)
    - [yolov4.cfg](https://raw.githubusercontent.com/AlexeyAB/darknet/master/cfg/yolov4.cfg)

## Installation

To install the Main GUI application either use the [install.sh](/Implementation/install.sh) to automatically install all the requirement dependencies, or follow these steps:

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
   - **Install Qt6 Framework:**
      ```sh
      sudo apt install -y qt6-base-dev qt6-base-private-dev qt6-declarative-dev \
         qt6-declarative-private-dev qt6-tools-dev qt6-tools-private-dev qt6-scxml-dev \
         qt6-documentation-tools libqt6core5compat6-dev qt6-tools-dev-tools qt6-l10n-tools \
         qt6-shader-baker libqt6shadertools6-dev qt6-quick3d-dev qt6-quick3d-dev-tools \
         libqt6svg6-dev libqt6quicktimeline6-dev libqt6serialport6-dev libqt6charts6-dev qt6-multimedia-dev 
      
      export export CMAKE_PREFIX_PATH=/usr/lib/x86_64-linux-gnu/cmake/Qt6:$CMAKE_PREFIX_PATH
      ```

   - **Install XGBoost:**
      ```sh
      git clone --recursive https://github.com/dmlc/xgboost
      cd xgboost
      mkdir build && cd build
      cmake ..
      make -j4
      cd ../..
      XGBOOST_INSTALL_PATH=/opt/xgboost
      sudo mkdir -p $XGBOOST_INSTALL_PATH
      sudo mv xgboost/* $XGBOOST_INSTALL_PATH
      rm xgboost
      sudo ln -s /opt/xgboost/lib/libxgboost.so /opt/xgboost/lib/libxgboost.so.0
      export XGBOOST_INCLUDE_PATH=$XGBOOST_INSTALL_PATH/include
      export XGBOOST_LIB_PATH=$XGBOOST_INSTALL_PATH/lib
      ```

   - **Install OpenCV:**
      ```sh
      sudo apt install -y libopencv-contrib-dev libopencv-dev libopencv-core-dev libopencv-dnn-dev
      ```

   - **Download YoloV4 configuration files:**
      ```sh
      wget https://github.com/AlexeyAB/darknet/releases/download/darknet_yolo_v4_pre/yolov4.weights
      wget https://raw.githubusercontent.com/AlexeyAB/darknet/master/cfg/yolov4.cfg
      ```

   - **Set other environment variables**
      ```sh
      export LD_LIBRARY_PATH=$XGBOOST_LIB_PATH:$LD_LIBRARY_PATH
      export CPATH=$XGBOOST_INCLUDE_PATH:$CPATH
      export LIBRARY_PATH=$XGBOOST_LIB_PATH:$LIBRARY_PATH
      ```
   - **Make environment variables permanent**
      - To make all the above environment variables permanent:
         - write all exports into the ~/.bashrc 
         - run `source ~/.bashrc`

2. **Build and run the application:**
```sh
    # Assuming we are in IndoorPositioningSystem/ folder
    mkdir build && cd build
    cmake ..
    make -j4
    # Run the application
    ./IndoorPositioningSystem
   ```