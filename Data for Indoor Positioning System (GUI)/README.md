# Data for Video Player

## Purpose
Contains data that are ready to be opened in Indoor Positioning System (GUI)\
- Data in "Recorded experiments" **are not ready** to be opened\
- The "Ground truth distances" **should not** be opened in GUI\
    - They should be used for data correction in Data Analysis window in GUI!!!

## Structure
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
