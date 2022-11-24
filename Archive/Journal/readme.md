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

# Nov 16, 2022

- Decentralized
- Tests are done in the dormitory. 2 anchors x 3 tags.
- 2 anchors x 1 tag is working best. This use-case is useful for the camera calibration purposes
- 2 anchors x 2 tags working good. But the one needs to be located in front of and perpendicular to anchors. 
- 2 anchors x 3 tags is also working, but only if anchors are located within 3-4 meters and places perpendicular to tags. But we still get data at least from 1 anchor that is useful for the calibration purposes.  

# Nov 17, 2022

- Centralized version v6
- Working better?

# Nov 18, 2022

- trying to implement centralized version, but using implemented library (all additional files are in v6). But it is not working, i.e. one tag is working super clean, but when it is 2 tag, it is now working, hitting only 1 tag and sometimes range is 0.00. Why?
- I thought it would be faster to get data while using library, but it is not. It is better to continue with my own implementation.


# Nov 19, 2022 Tests at university. 1 floor

- **v6 (centralized)**
  - **1 test** 
    - Anchors are placed in opposite sides. Distance between anchors is 3,4 m.
    - 1 tag is working good max at distance
      - at the distance 17 m it can hit 1 anchor
      - at the distance 14 m it can hit 2 anchors continuously
    - 2 tag work fine, but there are some unnecessary pings of server, why?
    - 3 tag are working good. 
    - Result: Sometimes there are some pings from tag to server that are unnecessary and not allowing tag to start communication with anchors. It is happen very often that tags disconnect from server, and in a while reconnect again. Why? 
    - Basically, it is working good with all 1, 2, 3 tags switched on. Working unexpectedly good. 

    - **Test for distance** was done with the following setting: 
      - 1st tag was placed at the distance ~10 m from anchors
      - 2nd and 3rd tags were placed an my hands at the distance ~15 m
      - Everything was working fine. All tags managed to send data. 
      - **Problems**: there was some issues unfortunately:
        - not always incoming package contains data from both anchors, but it happened rarely
        - the main problem here is that server prints incoming request signals instead of incoming packages, why?

    - So far, it is best setting.

  - **2 test** 
    - Anchors placed in one line along the whole
    - working a bit worth that the case when anchors placed on opposite sides.

  - **3 test** (with obstacles)
  - 1 tag and 2 anchors 
  - Obstacles such as the wall, any part of person (hand, leg, wrist), laptop, lower part, chair seat completely blocks signal.
  - chair backrest is not blocking.
  - when it is in the pocket, it hits both anchors at the distance 5 m and lower, and only 1 anchor at the distance 8~9 m

- v5.1 (decentralized)
  - **1 test**
  - anchors are placed on the opposite sides
  - it is working good as well in all cases (1, 2 and 3 tags switched on).
  - 1st tag is placed at the distance 5.5 m, 2nd and 3rd tags are placed at the distance 7 m (from anchors).
  - Since anchors are placed on the opposite sides, distance from a tag to each anchor is almost same.

Overall results: 
- v6 centralized. signal outgoing from uwb chip on tag must come directly to the uwb chip on anchor, therefore anchors must be placed at different level than tags. Lower or higher? Does n't matter, the goal is that signal should target uwb chip directly.
The best is when anchors are placed in the opposite direction. 

- v5.1 decentralized. Working good, but data coming less frequently than in v6. Sometimes can happen deadlocks. But thanks to the delay in tags it resolves automatically.


# Nov 24, 2022

Centralized v6

- A lot of 3's were coming because tag wasn't receive ack from the server indication that the server received distance. 
  - Supplementing such condition helped
- I didn't find where is the problem with anchors: some times they got frozen and did't receive messages from tags. 
  - Reinit helped, but it happened after a second in the case when anchor didn't received or send something. And because of that, tags sometimes didn't see that anchor. 
  - Reinit after 100 ms helped. Now it is working good.
- There was hard reset in tags. 
  - The problem was in interrupt pins in Tag. When tag had a lot of interrupts, interrupt flags got overflowed. And this happened only when tag received messages that are not for it. 
    - Also after request sent to the server, in the case server was used by some other tag, first tag had gotten wrong ack, but still checked received and sent packages, but instead it was better to start ping server again. Adding "return" to such placed helped.
- Now tags and anchors work good.
