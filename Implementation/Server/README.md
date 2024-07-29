# Server

The implementation of the Server, which simulates a real CCTV camera system, allowing to collect video and UWB data. 
It is podsible to Pause / Continue / Stop recording of data streams

**Responsibilities:**
- *Video recording*: handles video recording
- *(Centralized) Server for UWB*: communicates with UWB tags and collects distance measurements from them
- *Activity watchdog*: monitors tag responses and detects if the communication between an anchor and tag is blocked

Each work (responsibility) is performed simultaneously in a dedicated thread for better optimization

> [!Warning]
> The video and UWB data streams are not yet synchornized; synchronization is performed later in Indoor Positioning System (GUI)

**Output:**
  - `video.avi`: Video recording
  - `video_timestamps.txt`: Index file containing frames' timestamps
  - `UWB_timestamps.txt`: UWB measurements together with their timestamps

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
      # Assuming we are in Server/ folder
      mkdir build && cd build
      cmake ..
      make
      # Run the application
      ./Server
      ```

  - This will automatically start the UWB server, the watchdog to track activity of tags, and the video recording.

## Structure of the folder
```
.
├── build
├── Camera.cpp               # Accessing Camera 
├── Camera.h
├── CMakeLists.txt           # Building the Server
├── README.md
├── Server.cpp               # UWB Server + Activity Watchdog
├── Server.h
├── Server_Multithreaded.cpp # Main part initiating all workers
├── SharedData.h             # Communication between workers (threads)
├── VideoManager.cpp         # Recording video stream
└── VideoManager.h

```