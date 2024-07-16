# Implementation

## Purpose
Contains source code and implementation files.

## Structure
```
.
├── ESP32 UWB                                # Firmware for ESP32 UWB devices
│   ├── anchorArduino                        # Firmware for Anchor
│   └── tagArduino                           # Firmware for Tag
├── IndoorPositioningSystem                  # Main GUI
├── Camera Intrinsic Calibration (Optical)   # Intrinsic camera calibration
├── ├── intrinsic_parameters.xml             # Parameters prepared to be used in GUI for Optical method
│   └── Calibrator                           # Source code for calibration 
├── PixelToReal                              # Source code and data for PixelToReal training
└── Server                                   # Centralized server for recording UWB and video data
```