# Server (Centralized)

 Initiates simultaneous work of:
- Video Manager
- (UWB) Server
- Activity watchdog separately

Each work is initiated in a dedicated thread for better optimization

**!!! IMPORTANT !!!**
- These data are not yet synchornized; synchronization is performed later in Indoor Positioning System (GUI)

**Responsibilities:**
     - *Video processing*: handles video recording
     - *Server*: communicates with tags and collects distance measurements from them
     - *Activity watchdog*: monitors tag responses and detects if communication between anchor and tag is blocked

**Output:**
- Separatelly: 
    - `video.avi`
    - `video_timestamps.txt`
    - `UWB_timestamps.txt`

## Structure
```
.
├── build
├── Camera.cpp               # Camera
├── Camera.h
├── CMakeLists.txt
├── README.md
├── Server.cpp               # UWB Server + Activity Watchdog
├── Server.h
├── Server_Multithreaded.cpp # Main
├── SharedData.h             # Communication between threads 
├── VideoManager.cpp         # Video Manager
└── VideoManager.h

```