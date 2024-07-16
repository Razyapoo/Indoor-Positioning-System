# Smart extensions to regular cameras in the industrial environment

## Author
**Name:** Oskar Razyapov 
**Email:** oskarrazyapov@gmail.com  

## Abstract



**Keywords:** Repeatable Experiments, CCTV Cameras, Localization, Ultra-Wide band, Machine learning

## Institute
**Institute Name:** Charles University
**Department:** Department of Software Engineering

## Project structure
```bash
.
├── Archive
│   ├── Documentation
│   │   ├── Main Documentation
│   │   ├── Technical Documentation
│   │   └── User documentation
│   ├── Journal
│   └── Papers
├── Data for Video Player
│   ├── Ground truth distances for correction of UWB distances (data shifts)
│   │   ├── Experiment 109
│   │   ├── Experiment 113
│   │   ├── Experiment 118
│   │   └── Experiment 124
│   ├── Test - Experiment 113
│   ├── Test - Experiment 118
│   ├── Test - Experiment 118 - after correction
│   ├── Test - Experiment 124
│   └── Train - Experiment 109
├── Implementation
│   ├── ESP32 UWB
│   │   ├── anchorArduino
│   │   └── tagArduino
│   ├── GUI - Video Player
│   │   └── IndoorPositioningSystem
│   ├── Optical - Camera Intrinsic Calibration
│   │   └── Calibrator
│   ├── PixelToReal
│   └── UWB + Video data recorder (Server)
│       └── build
├── PixelToReal, Optical and UWB evaluation
│   ├── Data for evaluation
│   │   ├── For Experiment 109
│   │   ├── For Experiment 113
│   │   ├── For Experiment 118
│   │   └── For Experiment 124
│   └── Relusts of evaluation (Plots, Statistics)
│       ├── Plots
│       └── Statistics
└── Recorded Experiments
```