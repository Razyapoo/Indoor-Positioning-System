# Camera Intrinsic Calibration

Contains the implementation of the calibrator for performing camera intrinsic calibration.

The calibrator determines the intrinsic parameters of a camera:
  - Focal length
  - Optical center
  - Distortion coefficients

These parameters are used to calculate people coordinates using Optical method and undistort the image for better people detection.

`intrinsic_parameters_to_be_used_in_GUI.xml` - prepared intrinsic parameters to use in Indoor Positioning System (GUI)

**Process**:
- The calibration process uses a chessboard pattern to gather multiple views of the pattern from different angles.
- The implementation includes functions to detect the corners of the chessboard, compute the intrinsic parameters, and save the calibration results.

**Requarnments**:
- [OpenCV v4.7.0](https://docs.opencv.org/4.x/d7/d9f/tutorial_linux_install.html)

## Structure
```
.
├── Calibrator                                  
│   ├── build                                   # Build folder for cmake
│   ├── Calibrator.cpp                          # Perfroms calibration
│   ├── Calibrator.h                             
│   ├── Camera.cpp                              # Provides access to cameras
│   ├── Camera.h                                
│   ├── CMakeLists.txt                          
│   └── main.cpp                                # Init Calibrator and Camera
├── intrinsic_parameters_to_be_used_in_GUI.xml  # Intrinsic Parameters ready to use in IPS (GUI)
└── README.md
```
