# Smart extensions to regular cameras in the industrial environment

## Author
**Name:** Oskar Razyapov\
**Email:** oskarrazyapov@gmail.com  

## Abstract
> The purpose of this work is to obtain an extensive set of videos with automatically annotated locations of people. Using the collected data, we have developed a Pixel-to-Real machine learning model, which aims to extend static Closed-circuit television (CCTV) cameras to allow for localization of people and measuring distance between the them, solely based on the input video data. To gather the datasets, we have developed an Ultra-wideband (UWB) system, which allows for precise people localization. By synchronizing UWB and video data, we generated automatically annotated datasets, which were used for training the Pixel-to-Real model. To facilitate the data acquisition, we have developed a GUI, which allows to synchronize, visualize, and analyze video and UWB data for fast and easy system calibration. Additionally, it facilitates an export of the data for the model training. To demonstrate the accuracy of the developed model, we have developed the Optical method for people localization based on the height of people, and compared it with Pixel-to-Real model. We also performed a comprehensive evaluation of UWB, Pixel-to-Real, and Optical methods against Ground Truth people positions. Our methodology ensures the repeatability of the experiments, which allows us to support the future research and development in people localization.


**Keywords:** Repeatable Experiments, CCTV Cameras, Localization, Ultra-Wide band, Machine learning

## Institute
**Institute Name:** Charles University\
**Department:** Department of Software Engineering

## Documentation

The entire application (system) consists of several separate parts (smaller applications) each located in its own folder within this repository. Each part has its own installation guide available in the README.md file found in its respective folder. Detailed instructions on how to use the sepcific part of the application can be found in the [Wiki](https://github.com/Razyapoo/Master-Thesis/wiki).
  
## Workflow of the system

- **Working with video and UWB**
    1. Video and UWB data are collected using the [Server](./Server/).
        - The server [records](./Implementation/Server/VideoManager.h) video data and [communicates](./Implementation/Server/Server.h) with [ESP32 UWB devices](./ESP32%20UWB/), collecting data from both sources.
        - After the data is collected, they should be formatted for opening in GUI, as described in [Recorded Experiments](../Recorded%20Experiments/).
    2. After correction, the collected data can be opened in [Indoor Positioning System (GUI)](./Implementation/IndoorPositioningSystem/).

- **Camera calibration for Optical method**

    1. The camera calibration (calculation of intrinsic camera parameters) is performed using [Calibrator](./Implementation/Camera%20Intrinsic%20Calibration%20(Optical)/Calibrator/).

    2. After calibration, the parameters can be opened in [Indoor Positioning System (GUI)](./Implementation/IndoorPositioningSystem/) to enable the people localization using Optical method

- **Pixel-to-Real model training**
    
    1. The model capable of localizing people is trained using [`train_PixeltoReal_model.py`](./Implementation/PixelToReal/train_PixeltoReal_model.py)

    2. After the model is trained, it can be opened in [Indoor Positioning System (GUI)](./Implementation/IndoorPositioningSystem/) to enable the people localization using PixelToReal model

## Project structure
```
.
├── Archive                                      # Contains archived documents and papers
│   ├── Documentation                            # Documentation related to the project
│   │   ├── Images for wiki                      # Images for wiki 
│   │   ├── Main Documentation                   # Thesis
│   │   ├── Technical Documentation              # Detailed technical documentation
│   │   └── User documentation                   # User guides and manuals
│   ├── Journal                                  # Notes collected during experiments and development of the project
│   └── Papers                                   # Research papers and articles (Motivation for the project)
├── Data for Indoor Positioning System (GUI)     # Data files prepared to open in the IPS (GUI) application
│   ├── Ground truth data                        # Ground truth distances and coordinates for experiments (used for correction of UWB measurements)
│   │   ├── Experiment 109                       # Ground truth data for Experiment 109
│   │   ├── Experiment 113                       # Ground truth data for Experiment 113
│   │   ├── Experiment 118                       # Ground truth data for Experiment 118
│   │   └── Experiment 124                       # Ground truth data for Experiment 124
│   ├── Test - Experiment 113                    # Test data for PixelToReal model from Experiment 113
│   ├── Test - Experiment 118                    # Test data for PixelToReal model from Experiment 118
│   ├── Test - Experiment 124                    # Test data for PixelToReal model from Experiment 124
│   └── Train - Experiment 109                   # Train data for PixelToReal model from Experiment 109
├── Implementation                               # Source code files
│   ├── ESP32 UWB                                # Firmware for ESP32 UWB devices
│   │   ├── anchorArduino                        # Firmware for Anchor
│   │   └── tagArduino                           # Firmware for Tag
│   ├── IndoorPositioningSystem                  # Main GUI
│   ├── Camera Intrinsic Calibration (Optical)   # Intrinsic camera calibration
│   │   └── Calibrator                           # Source code for calibration 
│   ├── PixelToReal                              # Source code and data for the PixelToReal model training
│   └── Server                                   # Centralized server for recording UWB and video data
├── PixelToReal, Optical and UWB evaluation      # Evaluation data and results
│   ├── Data for evaluation                      # Data prepared for evaluating the system (export from GUI)
│   │   ├── For Experiment 109                   # Data from the Experiment 109, to be evaluated
│   │   ├── For Experiment 113                   # Data from the Experiment 113, to be evaluated
│   │   ├── For Experiment 118                   # Data from the Experiment 118, to be evaluated
│   │   └── For Experiment 124                   # Data from the Experiment 124, to be evaluated
│   └── Relusts of evaluation (Plots, Statistics) # Evaluation results, including plots and statistics
│       ├── Plots                                # Plot files for evaluation results
│       └── Statistics                           # Statistical analysis of evaluation results
└── Recorded Experiments                         # Raw data and recordings from experiments (Not prepared to open in GUI)
```
