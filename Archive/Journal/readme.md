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
  - Did some tests:
    - Note: Anchors sometimes lost receive and sent flags. It is needed to hard reset them.
    - Note: delay and PONG_TIMEOUT_MS in the anchor's IDLE_STATE is needed to escape deadlock when all tags send PING to the anchor. Do we need STATE_PENDING_PONG? Sometimes tags are hard reset. Why? Does STATE_PENDING_PONG help? Looks like yes, but I did only one test; need to do more tests.

- Next step: 
  - to implement centralized system with server as initiator. This way we will escape problems with overloading the server and anchors.

# Nov 2, 2022

- There is a problem with centralized system - it waits for the new connections and only waits for incoming requests from sockets.
- In my opinion, there exist 2 ways how to solve this problem. 1 way is to connect tags to the server at the beginning and then just communicate with them. 2 way is to write multithreaded server
  where 1 thread will be responsible for the new connections and another thread will be used for communication.  

# Nov 7, 2022

- There is no reason to implement centralized system, since tags still need to respond to server and there can be some delay. Can it interrupt another calculation, request a write request while calculating another tag?
- Set up local network. A lot better now. 
- Trying to implement in anchors history of tag connections (first draft). Is it good working?

# Nov 8, 2022

- Note: In many systems, where all devices have the same transmitter and receiver antenna delays, it is
unnecessary to calibrate the transmitter antenna delay. The transmitter antenna delay may be set to zero
and a combined receiver and transmitter delay value may be used for the receiver antenna delay without
impairing the ranging performance of the system. source: https://www.qorvo.com/products/d/da007967

# Nov 9, 2022

- standard Ranging library does not check if msg was sent to the certain (current) tag, therefore, when we have multiple tags, one of them can receive data that was specialized for another tag 

# Nov 12, 2022

- I have implemented own anchor-tag ranging system based on https://github.com/gsongsong/dw1000-positioning and https://www.decawave.com/wp-content/uploads/2018/10/APS013_The-Implementation-of-Two-Way-Ranging-with-the-DW1000_v2.3.pdf.
I have it also configured with 1 anchor and 3 tags. It is working synchronically. **But it is need to synchronize also 2nd anchor.**  folder: v5