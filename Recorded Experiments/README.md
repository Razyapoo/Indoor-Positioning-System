# Recorded Experiments

## Purpose
Contains raw data and recordings from experiments:

Each experiment consists of the following files (produced by Server):
- video.avi - video recording of the experiment
- video_timestamps.txt - index file to read video fram-by-frame
- UWB_timestamps.txt - UWB measurements

These data are **not prepared** to be opened in IPS (GUI) because of UWB_timestamps.txt contains incompatible data format:
    - Format used (for informative purpose):
        "Record ID" "Timestamp" "Tag ID" "Anchor 1 ID" "Anchor 1 Distance" "Anchor 2 ID" "Anchor 2 Distance"
        **Request time: { timestamp }\ Response time: { timestamp }\ Overall time of the request (response time - request time): { measurement_time }**
    - Correct format (ready to be opened in GUI):
        "Record ID" "Timestamp" "Tag ID" "Anchor 1 ID" "Anchor 1 Distance" "Anchor 2 ID" "Anchor 2 Distance" "**measurement_time**"

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