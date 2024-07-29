#!/bin/bash

while true; do
    read -p "Make sure you have the system updated and upgrated. Proceed with installation of libraries? (y/n): " yn
    case $yn in
        [Yy]* ) break;;
        [Nn]* ) echo "Exiting installation. Please run the script again after performing update and upgrade."; exit;;
        * ) echo "Please answer yes or no.";;
    esac
done

# Update and install basic dependencies
sudo apt-get install -y build-essential cmake g++ wget unzip git ninja-build \
    pkg-config libgtk-3-dev libeigen3-dev \
    libavcodec-dev libavformat-dev libswscale-dev libv4l-dev \
    libxvidcore-dev libx264-dev libjpeg-dev libpng-dev libtiff-dev \
    openexr libatlas-base-dev \
    libtbb2 libtbb-dev libxcb-xinerama0 libxcb-cursor0

# Install Qt6 Framework at the beginning to check credentials
echo "Installing Qt6..."
sudo apt install -y qt6-base-dev qt6-base-private-dev qt6-declarative-dev \
    qt6-declarative-private-dev qt6-tools-dev qt6-tools-private-dev qt6-scxml-dev \
    qt6-documentation-tools libqt6core5compat6-dev qt6-tools-dev-tools qt6-l10n-tools \
    qt6-shader-baker libqt6shadertools6-dev qt6-quick3d-dev qt6-quick3d-dev-tools \
    libqt6svg6-dev libqt6quicktimeline6-dev libqt6serialport6-dev libqt6charts6-dev qt6-multimedia-dev 
echo "Qt6 is installed"

# Install XGBoost

## Train model
echo "Installing XGBoost..."
pip install xgboost numpy==1.26.1 pandas==2.1.2 scipy==1.11.4 bottleneck==1.3.4

## Install library for running the model
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
echo "XGBoost is installed to $XGBOOST_INSTALL_PATH"


# Install OpenCV
echo "Installing OpenCV..."
sudo apt install -y libopencv-contrib-dev libopencv-dev libopencv-core-dev libopencv-dnn-dev
echo "OpenCV is installed"

# Download YoloV4 configuration files
echo "Downloading YoloV4 configuration files..."
mkdir weights && cd weights
wget https://github.com/AlexeyAB/darknet/releases/download/darknet_yolo_v4_pre/yolov4.weights
wget https://raw.githubusercontent.com/AlexeyAB/darknet/master/cfg/yolov4.cfg
cd ..

{
  echo "# Qt"
  echo "export CMAKE_PREFIX_PATH=/usr/lib/x86_64-linux-gnu/cmake/Qt6:\$CMAKE_PREFIX_PATH"
  echo "# XGBoost"
  echo "export XGBOOST_INCLUDE_PATH=$XGBOOST_INSTALL_PATH/include"
  echo "export XGBOOST_LIB_PATH=$XGBOOST_INSTALL_PATH/lib"
  echo "# Library Paths"
  echo "export LD_LIBRARY_PATH=\$XGBOOST_LIB_PATH:$LD_LIBRARY_PATH"
  echo "export CPATH=\$XGBOOST_INCLUDE_PATH:$CPATH"
  echo "export LIBRARY_PATH=\$XGBOOST_LIB_PATH:$LIBRARY_PATH"
} >> $HOME/.bashrc

source $HOME/.bashrc

echo "Installation completed successfully!"
