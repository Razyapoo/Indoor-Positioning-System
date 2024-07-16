# Smart extensions to regular cameras in the industrial environment

## Author
**Name:** Oskar Razyapov\
**Email:** oskarrazyapov@gmail.com  

## Abstract



**Keywords:** Repeatable Experiments, CCTV Cameras, Localization, Ultra-Wide band, Machine learning

## Institute
**Institute Name:** Charles University\
**Department:** Department of Software Engineering

## Project structure
```
.
├── Archive                                      # Contains archived documents and papers
│   ├── Documentation                            # Documentation related to the project
│   │   ├── Main Documentation                   # Thesis
│   │   ├── Technical Documentation              # Detailed technical documentation
│   │   └── User documentation                   # User guides and manuals
│   ├── Journal                                  # Journal entries related to the project
│   └── Papers                                   # Research papers and articles (Motivation for the project)
├── Data for Indoor Positioning System (GUI)     # Data files prepared to be opened in the Indoor Positioning System (GUI) application
│   ├── Ground truth distances                   # Ground truth distance data for experiments (used for correction of UWB measurements)
│   │   ├── Experiment 109                       # Prepared data for Experiment 109
│   │   ├── Experiment 113                       # Prepared data for Experiment 113
│   │   ├── Experiment 118                       # Prepared data for Experiment 118
│   │   └── Experiment 124                       # Prepared data for Experiment 124
│   ├── Test - Experiment 113                    # Test data for PixelToReal model from Experiment 113
│   ├── Test - Experiment 118                    # Test data for PixelToReal model from Experiment 118
│   ├── Test - Experiment 124                    # Test data for PixelToReal model from Experiment 124
│   └── Train - Experiment 109                   # Train data for PixelToReal model from Experiment 124
├── Implementation                               # Source code and implementation files
│   ├── ESP32 UWB                                # Firmware for ESP32 UWB devices
│   │   ├── anchorArduino                        # Firmware for Anchor
│   │   └── tagArduino                           # Firmware for Tag
│   ├── IndoorPositioningSystem                  # Main GUI
│   ├── Camera Intrinsic Calibration (Optical)   # Intrinsic camera calibration
│   ├── ├── intrinsic_parameters.xml             # Parameters prepared to be used in GUI for Optical method
│   │   └── Calibrator                           # Source code for calibration 
│   ├── PixelToReal                              # Source code and data for PixelToReal training
│   └── Server                                   # Centralized server for recording UWB and video data
├── PixelToReal, Optical and UWB evaluation      # Evaluation data and results
│   ├── Data for evaluation                      # Data used for evaluating the system (export from GUI)
│   │   ├── For Experiment 109                   # Evaluation data for Experiment 109
│   │   ├── For Experiment 113                   # Evaluation data for Experiment 113
│   │   ├── For Experiment 118                   # Evaluation data for Experiment 118
│   │   └── For Experiment 124                   # Evaluation data for Experiment 124
│   └── Relusts of evaluation (Plots, Statistics) # Evaluation results, including plots and statistics
│       ├── Plots                                # Plot files for evaluation results
│       └── Statistics                           # Statistical analysis of evaluation results
└── Recorded Experiments                         # Raw data and recordings from experiments (Not prepared to be opened in GUI)
```