# Journal

This is the journal to record updates and ideas, what didn't work, what was changed etc. Starting date is Oct 31, 2022 (middle point of the implementation, not beginning)

It doesn't record all changes, but the most important.

# What was done before

- trial run of YOLOv4 to detect people in the input video
- integration of focal length into YOLOv4 to compute the distance between people using 3D Euclidian distance
- trial run of ESP32 UWB chips
  - uploading existing code from official **[arduino-dw1000](https://github.com/thotro/arduino-dw1000)** library (examples/DW1000Ranging_ANCHOR/DW1000Ranging_ANCHOR.ino and examples/DW1000Ranging_TAG/DW1000Ranging_TAG.ino)
  - integration of [link.cpp](https://github.com/playfultechnology/UWBRTLPS/blob/main/ESP32/UWB_Positioning/link.cpp) and [link.h](https://github.com/playfultechnology/UWBRTLPS/blob/main/ESP32/UWB_Positioning/link.h) to detect multiple anchors from tags (there was problems)
- new try with ESP32 UWB chips
  - source: [anchor](https://github.com/gsongsong/dw1000-positioning/blob/ee5e2c9e57f42ad23014cc470b6478d3fb0dbe57/anchorArduino/anchorArduino.ino) and [tag](https://github.com/gsongsong/dw1000-positioning/tree/ee5e2c9e57f42ad23014cc470b6478d3fb0dbe57/tagArduino) (changed) + added sender to server

# Oct 29, 2022

- Experiments at the school.

# Oct 31, 2022

- On the last experiment (Oct 29, 2022) there was a problem of the server overloading with tag requests. Also tags hit the signal from the anchors only on the distance of 2 m between anchors.
- Fixes:
  - there was a bug on hw level. To send data from tag to server it is needed first to establish the connection and then send data.
  - if tag didn't receive correct ack it get stuck. Fix: left tag scan again after a while (if there is no response), or in case of bad response.
  - Delay parameter was set to small value. When tag has computed the distance, it sent to the server ack to establish the connection. Tag didn't get the ack from server and started to compute new value. 

- Updates:
  - state diagram for tag
# Nov 1, 2022

- Updates:
  - state diagram for anchor
