# ESP32 UWB

Contains the implementation of the Asymmetric Double-Sided Two-Way Ranging protocol.

# Requirements

- [Arduino IDE](https://www.arduino.cc/en/software)
- [DW1000 lib](https://github.com/Makerfabs/Makerfabs-ESP32-UWB/blob/main/mf_DW1000.zip)

# How to upload the firmware

The code for both anchors and tags is uploaded in the same way:

## Set up the Arduino IDE

- Download and install the Arduino IDE:
    - Download the Arduino IDE from the official Arduino [website](https://www.arduino.cc/en/software).
    - It is recommended to install 1.8.18 version, as this version was used during the project's development. A newer version of 2.x is not guaranteed to work.
    - Follow the installation instructions from the [tutorial](https://docs.arduino.cc/software/ide-v1/tutorials/Linux/).

## Install the ESP32 Board package

1. Open Preferences in Arduino IDE:
    - Got to `File` -> `Preferences`.

2. Add ESP32 Board URL:
    - In the `Additional Boards Manager URLs` field, add the following URL:
        ```https://dl.espressif.com/dl/package_esp32_index.json```
    - If there are other URLs already present, separate them with a comma.

3. Open the Boards Manager and Install the ESP32 Package:

    - Go to `Tools` -> `Board` -> `Boards Manager`.
    - Search for the `esp32` package in a search bar
    - Click `Install`

## Install the ESP32 UWB library

1. Download the ESP32 UWB library from the Makerfabs GitHub [repository](https://github.com/Makerfabs/Makerfabs-ESP32-UWB/blob/main/mf_DW1000.zip).

2. Extract the downloaded ZIP file to Arduino libraries directory ("path/to/arduino"/libraries).

## Select the ESP32 UWB board and port

1. Select the ESP32 Board:
    - Go to `Tools` -> `Board` and select the `ESP32 Arduino` -> `ESP32 Dev Module`

2. Select the port:
    - Connect the ESP32 UWB board to the computer using USB
    - Go to `Tools` -> `Port` and select the working port. 

## Upload the firmware

1. Upload the Sketch:
    - Click the `Upload` button (the right-pointing arrow) in the Arduino IDE toolbar.
    - This will automatically upload the firmware to the ESP32 board and show the installation progress.


## Structure
```
.
├── anchorArduino               # Firmware for Anchor
│   ├── anchorArduino.ino
│   └── arduino.h
├── README.md
└── tagArduino                  # Firmware for Tag
    ├── arduino.h
    └── tagArduino.ino
```