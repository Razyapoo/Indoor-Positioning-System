# Data for Indoor Positioning System (GUI)

Contains data that are **ready** to open in Indoor Positioning System (GUI)
- The "Ground truth data" **should not** be opened in GUI
    - These are Ground Truth distances for UWB measurements correction in Data Analysis window in GUI!!!
    - This folder also contains Ground Truth Coordinates used for methods' evaluation (UWB, Pixel-to-Real, Optical)

- Indoor Positioning System (GUI) opens as a project / package a folder consisting of:
    - `UWB_timestamps.txt` - UWB measurements.
    - `video_timestamps.txt` - index file to read the video frame-by-frame.
    - `video (.avi, .mp4)` - video recording in either .avi format or .mp4

- Coordinates of anchors
    - Input the following coordinates of the anchors when opening a new video package
    - Origin should be checked as so, when anchor positions are entered in Indoor Positioning System GUI
    - Each experiment have different coordinates of Origin of UWB coordinate system!!    

    - Experiment 109
        - Anchor 101 { x: 0.627, y: 2.08} (Origin)
        - Anchor 102 { x: 3.127, y: 2.08}

    - Experiment 113
        - Anchor 101 { x: 3.612, y: 2.08}
        - Anchor 102 { x: 1.112, y: 2.08} (Origin)

    - Experiment 118
        - Anchor 101 { x: 3.127, y: 2.08}
        - Anchor 102 { x: 0.627, y: 2.08} (Origin)
        - Anchor 103 { x: 0.627, y: 17.08}
        - Anchor 104 { x: 3.127, y: 17.08}

    - Experiment 124
        - Anchor 101 { x: 3.612, y: 2.08}
        - Anchor 102 { x: 1.112, y: 2.08} (Origin)
        - Anchor 103 { x: 1.112, y: 8.08}
        - Anchor 104 { x: 3.612, y: 8.08}

## Structure
```
.
├── Data for Indoor Positioning System (GUI)     # Data files prepared to open in the Indoor Positioning System (GUI) application
│   ├── Ground truth data                        # Ground truth distances and coordinates for experiments (used for correction of UWB measurements)
│   │   ├── Experiment 109                       # Ground truth data for Experiment 109
│   │   ├── Experiment 113                       # Ground truth data for Experiment 113
│   │   ├── Experiment 118                       # Ground truth data for Experiment 118
│   │   └── Experiment 124                       # Ground truth data for Experiment 124
│   ├── Test - Experiment 113                    # Test data for PixelToReal model from Experiment 113
│   ├── Test - Experiment 118                    # Test data for PixelToReal model from Experiment 118
│   ├── Test - Experiment 124                    # Test data for PixelToReal model from Experiment 124
│   └── Train - Experiment 109                   # Train data for PixelToReal model from Experiment 124
```
