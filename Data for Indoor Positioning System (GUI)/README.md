# Data for Indoor Positioning System (GUI)

## Purpose
Contains data that are **ready** to open in Indoor Positioning System (GUI)
- The "Ground truth distances" **should not** be opened in as a project in GUI
    - These are Ground Truth distances for data correction in Data Analysis window in GUI!!!

- Indoor Positioning System (GUI) opens as a project a folder consisting of:
    - "UWB_timestamps.txt" - UWB data
    - "video_timestamps.txt" - index file for video
    - video (.avi, .mp4) - original video have .avi format; for sending the project to SIS sample video were changed to .mp4

## Structure
```
.
├── Data for Indoor Positioning System (GUI)     # Data files prepared to open in the Indoor Positioning System (GUI) application
│   ├── Ground truth distances                   # Ground truth distance data for experiments (used for correction of UWB measurements)
│   │   ├── Experiment 109                       # Prepared data for Experiment 109
│   │   ├── Experiment 113                       # Prepared data for Experiment 113
│   │   ├── Experiment 118                       # Prepared data for Experiment 118
│   │   └── Experiment 124                       # Prepared data for Experiment 124
│   ├── Test - Experiment 113                    # Test data for PixelToReal model from Experiment 113
│   ├── Test - Experiment 118                    # Test data for PixelToReal model from Experiment 118
│   ├── Test - Experiment 124                    # Test data for PixelToReal model from Experiment 124
│   └── Train - Experiment 109                   # Train data for PixelToReal model from Experiment 124
```