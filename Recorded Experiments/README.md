# Recorded Experiments

Each experiment consists of the following files (produced by the [Server](/Implementation/Server/Server_Multithreaded.cpp)):
  - `video.avi`: Video recording
  - `video_timestamps.txt`: Index file containing frames' timestamps
  - `UWB_timestamps.txt`: UWB measurements together with their timestamps

These data are **not prepared** to be openned in Indoor Positioning System (GUI) because `UWB_timestamps.txt` contains an incompatible data format:
- **Format used in these recordings (per record ID)**:
  ```
  { Record ID } { Timestamp } { Tag ID } { Anchor 1 ID } { Anchor 1 Distance } { Anchor 2 ID } { Anchor 2 Distance } 
  Request time: { timestamp } 
  Response time: { timestamp } 
  Overall time of the request (response time - request time): { measurement_time }
  ```

- **Correct format to open in GUI (per record ID)**:
  ```
  { Record ID } { Timestamp } { Tag ID } { Anchor 1 ID } { Anchor 1 Distance } { Anchor 2 ID } { Anchor 2 Distance } { measurement_time }
  ```

During the experiments, I have been writing notes into the journal, which is available in the following directory: [Journal](../Archive/Journal/)