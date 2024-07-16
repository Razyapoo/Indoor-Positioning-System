# Recorded Experiments

Contains raw data and recordings from experiments.

Each experiment consists of the following files (produced by Server):
- `video.avi` - video recording of the experiment.
- `video_timestamps.txt` - index file to read the video frame-by-frame.
- `UWB_timestamps.txt` - UWB measurements.

During the experiments, I made notes in a journal. These notes are available in the following directory: [Journal](../Archive/Journal/)

These data are **not prepared** to open in IPS (GUI) because `UWB_timestamps.txt` contains an incompatible data format:
- **Format used (for informative purposes, per record ID)**:
  ```
  { Record ID } { Timestamp } { Tag ID } { Anchor 1 ID } { Anchor 1 Distance } { Anchor 2 ID } { Anchor 2 Distance } 
  Request time: { timestamp } 
  Response time: { timestamp } 
  Overall time of the request (response time - request time): { measurement_time }```

- **Correct format to open in GUI (per record ID)**:
  ```{ Record ID } { Timestamp } { Tag ID } { Anchor 1 ID } { Anchor 1 Distance } { Anchor 2 ID } { Anchor 2 Distance } { measurement_time }```

## Structure
```
.
├── Data for evaluation                      # Data prepared for evaluating the system (export from GUI)
│   ├── For Experiment 109                   # Evaluation data for Experiment 109
│   ├── For Experiment 113                   # Evaluation data for Experiment 113
│   ├── For Experiment 118                   # Evaluation data for Experiment 118
│   └── For Experiment 124                   # Evaluation data for Experiment 124
└── Relusts of evaluation (Plots, Statistics) # Evaluation results, including plots and statistics
    ├── Plots                                # Plot files for evaluation results
    └── Statistics                           # Statistical analysis of evaluation results
```