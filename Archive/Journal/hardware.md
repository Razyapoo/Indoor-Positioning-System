# Journal

This is the journal to record updates and ideas, what didn't work, what was changed etc. Starting date is Oct 31, 2022 (middle point of the implementation, not beginning)

It doesn't record all changes, but the most important.

# What was done before

- trial run of YOLOv4 to detect people in the input video
- integration of focal length into YOLOv4 to compute the distance between people using 3D Euclidian distance
- trial run of ESP32 UWB chips

  - uploading existing code from official **[arduino-dw1000](https://github.com/thotro/arduino-dw1000)** library (examples/DW1000Ranging_ANCHOR/DW1000Ranging_ANCHOR.ino and examples/DW1000Ranging_TAG/DW1000Ranging_TAG.ino)
    - it works perfectly, but only for one tag and one anchor
    - To support more anchors, we have to use already implemented linked list of anchors:
      - integration of [link.cpp](https://github.com/playfultechnology/UWBRTLPS/blob/main/ESP32/UWB_Positioning/link.cpp) and [link.h](https://github.com/playfultechnology/UWBRTLPS/blob/main/ESP32/UWB_Positioning/link.h) to detect multiple anchors from a tag

- The main idea of the project is to implement synchronized system which uses several methods for distance measurement and then estimates final distance based on measured distances. For example it can use average.

  - this requires synchronization between each method

- First thing which comes to mind is to use some machine which will collect data from every method and then make final decision about the distance estimation. This machine can be a server.

- There are two ways to store the distances measured by UWB chips: in anchors and in tags. 

  - The former is not good desicion since two anchors must be synchronized as well to send the sync data to the server related to a particular tag, or server must know that received data from anchors belong to the same tag. Therefore we decided to implement the letter method, because each tag collects data related only to it.

- As the first step we desided to implement decentralized system. We have implemented the server which will get data from tags, but let tags operate on their own, i.e. they initiate the communication with anchors and collect the desired number of estimated distances from anchors; then they send measured distances to the server.

- Unfortunatelly, during experiments we have realized that tags overloaded the server, so the server showed distances with significant delay. And then after a while it showed all the received distances at once. This was a complete mess.

- As an improvement of the communication between server and tags we have added acknoledgements as a step of the communication between server and tags. This helped to overcome the problem.

- Unfortunatelly, after some tests, we have realized that the official implementation of the TWR protocol from the DW1000 library it happens that if we have a system with two tags and one anchor which are located at the same line, the tag in the middle overwrites data that are specific for the farther tag, therefore the farthest tag receives wrong estimated distances. Do not know exactly why, I think the library uses myDistantDevice->getDistance() in tags which reads distance from the anchor (distance is computed in the anchor). We desided not to overwrite the whole library and go with another existing solution.
  - new try with ESP32 UWB chips
    - source: [anchor](https://github.com/gsongsong/dw1000-positioning/blob/ee5e2c9e57f42ad23014cc470b6478d3fb0dbe57/anchorArduino/anchorArduino.ino) and [tag](https://github.com/gsongsong/dw1000-positioning/tree/ee5e2c9e57f42ad23014cc470b6478d3fb0dbe57/tagArduino) (changed) + added sender to server

    - The reason is that distance is measured on the anchor side, and this is a global variable, not specific to tag. Therefore, if we have in the system 2 or more anchors and 2 or more tags operating at the same time, tag might overwrite measured distance on the anchor side which belongs to another tag. This again leads to synchronization of data received from anchors on the server side.

# Oct 29, 2022

- Experiments at the school.

# Oct 31, 2022

- On the last experiment (Oct 29, 2022) there was a problem of the server overloading with tag requests. Also tags hit the signal from the anchors only on the distance of 2 m between anchors.
- Fixes:

  - there was a bug on hw level. To send data from tag to server it is needed first to establish the connection and then send data.
  - if tag didn't receive correct ack it get stuck. Fix: let tag scan again after a while (if there is no response), or in case of bad response.
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
  I have it also configured with 1 anchor and 3 tags. It is working synchronically. **But it is need to synchronize also 2nd anchor.** folder: v5

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

Another problem of decentralized system is that requests/data coming from tags are not evenly distributed, meaning that some data coming from some tags more frequently than from others. It even happen that one tag locks both anchors and do not allow other tags to communicate with anchors.

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

# Nov 25, 2022

- There are 2 basic libraries (first, 2022: https://github.com/jremington/UWB-Indoor-Localization_Arduino; second, official, 2019: https://github.com/thotro/arduino-dw1000).
  - The first lib provides setting of calibration parameter on anchors. It also provides autocalibration methods. But the bad side of this library is that anchors and tag are working best only in the case they are **facing each other**, for example when a person body , or some obstacle blocks the signal. Calibration (antenna delay) is set on anchor side. Each anchor must have own calibration parameter.
  - The second library doesn't provide methods to set the calibration parameter, but tags and anchors still see each other even if there are obstacles between them. Anchors and tags do not have to look directly to sensors of each other. It is working a lot better. The only thing is calibration parameter must to be set inside the library code. Each ESP32 chip has the same calibration parameter, and it is working good.
- Small experiment at school

# March 25, 2023

- Facing error: "python needs to be added to $Path" when trying to push code to ESP32:

  - solution: sed -i -e 's/=python /=python3 /g' /home/oskar/.arduino15/packages/esp32/hardware/esp32/1.0.6/platform.txt

# May 27, 2023

- (Why it did not work at some point? tags sent messages, but anchors did not receive them)

- Last implemented method v6.1 works perfect for 1/2 tag(s) and 2 anchors, but not so good for 3 tags and 2 anchors, because half of the received distances are from only one anchor. Why?

- To solve this problem, and based on the gained knowledge of how TWR protocol works, we decided to implement own solution. As the first step we decided to get rid of states, even thought they apper to be useful.

- As the first step I just focused on communication between one tag and one anchor. In own implementation I wanted to abstract from the mess of states and waiting states, therefore I desided to use the same logic as used in the https://github.com/gsongsong/dw1000-positioning and https://github.com/thotro/arduino-dw1000 implementations, but instead use separate function sendMessage and receiveMessage that will perform all the necessary steps to send and receive messages. So we can just send as a parameter the type of message to send or receive.

  - it used the logic that a tag sends first round of message exchange to explore which anchors are available in the area. Anchors then respond to ranging init (explore) message with own addresses, so the tag can collect these addresses in its own array for further communications. After exploration phase follows the ranging calculation phase in which tag starts communicate with each anchor found in the ranging init phase.
  - During the ranging calculation phase both anchor and tag have set busy flag which makes them anavailable from other interuptions. Each of them also remembers the counterpart's address, so they know that they communicate with the right counterpart.
  - !! The problem of abstraction was that we used if statement to check whether received message is expected. Because it checked first type of the message and if it was wrong it alert that the message is wrong and that it expect another message, and then stopped ranging.

    - therefore we decided to check received message explicitelly in the receive section under receivedAck variable
    - also added returns in each part of message checking (in receive section)

  - then I decided to get rid of ranging init state because it is unneccessary step. It is better to check in tag the array of already found anchors and if it is found just skip it.

    - I set a reply delay for each anchor different, so when tag sends poll message, anchors responds with different delays, so tag has a time to check whether it has already communicated with that anchor.

    - !!! Direct communication with anchor (without ranging init step) allowed to speed up the communication and bring the huge benefit, namely the tag is not stick with particular anchors. For example, when a person is walking fast, he may be already far from detected anchors in the ranging init phase, but tag must communicate with these anchors. It is better to have each pair of anchors set at the distance 7-10 meters from each other pair, so the tag can hit any arbitrary other anchor in the area and not stick with found in the ranging init phase. This will make the system and distance estimation iself more precise.
      Ranging init phase is used in both https://github.com/gsongsong/dw1000-positioning and https://github.com/thotro/arduino-dw1000 implementations.

  - Sometimes tag does not receive message from anchor (do not know why. Maybe it is because the issue in the implementation, or it may related to the tag).

    - reinit after 500 ms helps

  - Reply delay is set to replyDelay = (2*(myID-100)+1)*DEFAULT_REPLY_DELAY_TIME; where DEFAULT_REPLY_DELAY_TIME = 7 miliseconds

    - not good for distances father than 10m
    - replyDelay = (2*(myID-100)+2)*DEFAULT_REPLY_DELAY_TIME; is better

  - New implementation works very good. So far best from all tested implementations.

  - Problems during experiments:
    - Experiments was done in the dormitory
    - Setting: all 3 tags were close to each other (placed on chair), all 2 also were close to each other placed on chair
    - At the distances farther than 10 meters sometimes one of the tags cannot find second anchor. This may be beacuse of the signal propagation, since the hole is tight and long.
    - The estimated distance is correct within 2-3 meters. But at farthest distances from 5-6 meters there is constant shift +20..30 cm

# June 3, 2023 Experiment

- Anchor baseline: 1 m

# September 23, 2023

- During experiments I have occasionally changed options of the ESP32 board when uploading code.
- This fortunatelly shows, that when CPU frequency is set to 240 MHz chips generate results a lot faster.
- All chips must have same options set

Example: (so far best)

Board: "ESP32 Dev Module"
Upload Speed: 115200
CPU Frequency: 240 MHz (WiFi/BT)
Flash Frequency: 80 Mhz
Flash mode: QIO
Flash size: 4MB (32 Mb)
Partition Scheme: "Default 4MB with spiffs(1.2 MB APP/1.5MB SPIFFS)
Core Debug Level: "None"
PSRAM: "Disabled"
Port: "/dev/ttyUSB0"

- I have also changed a bit logic of the server such that now it handles cases when tag losts the connection, server detects it using timeout in select and then removes tag from the queue. Tag can be connected to the server again.

# September 24, 2023 - Experiments at university

- Distances between papers (od zdi opropti skrini):
  - 1.64 m
  - 1.26 m
  - all experiments assumed that tags where calibrated at 5m, and tags have presision +-5 (more +) cm at 5 m

1. Experiment "timestamp_ESP32_test_1_po_kalibraci.txt":

   - At this experiment all anchors where placed at the same line parallel to and 5m far from the anchor baseline.
   - During the experiment all anchors where static (were not moved).
   - anchor 102 was placed at the first walking line (same line as Tag 2), Anchor 101 was placed at the 2nd walking line
   - Using distances between anchors and ground truth papers on the floor we can manually perform calculations and check whether all distances received from tags are correct
   - TODO: try to compute the actual distances (based on geometry and ground truth distances)

2. Exepriment "timestamp_ESP32-Petr prekryl anchor 2":

   - Peter covered himself Tag 2
   - I can conclude that when tag is very close to a person, maybe within 5cm, or when it is touching a person, connection between tag and anchors is getting lost.
   - OBSERVATION: But when a person is not touching a tag and holds a tag at more than 5cm, but still covering a tag, signal is reaching the anchors, maybe because of signal propagation (reflection)? The only thing is that a distance is getting higher, around +30 cm, may it be that signal goes through the person, but because of the liquids inside a person it takes more time to reach beacons, or it cannot go though the person at all, and signal only reflects from the walls?

3. Experiment "timestamp_ESP32_presouvani_tag_2.txt":

   - During this experiments we have conclude that when tag is getting closer to anchor baseline, distance is lesser than the actual (max deviation is 20cm, but usually 15 cm), remember that tags were calibrated at 5m. Otherwise, when getting father from anchors, distance gets greater than the actual is (again max deviation is 20 cm, but most common is 15 cm).
   - Therefore I can conclude that calibration factor is precised only for the distance at which it was calibrated!!
   - also, at the end of the experiment I placed that tag before the wall, and it looks like the wall does not have reflexion (искажение)
     - this comes from the 8 m part of the experiment. I placed Tag 2, at the center (second) line, and swithced off other tags (before Tag 2 was at the first line and other tags were switcehd on). I though that tags placed at parallel lines (not necessary at the same distance from the baseline) will affect Tag 2 (which was moved), but it seems that they are not. And distance between tags more than 1 m is sufficient and they do not hide(cover) each other; for example when Tag 1 moved along the 2nd line is placed closer to the anchor baseline than Tag 2 moved along the 1st line, then it may block the Tag 2, such that anchor do not see Tag 2.

4. Experiment "timestamp_ESP32_experiment_s_otocenim_Petra_taky_se_nahralo_audio":

- During this experiment Peter was holding a tag in hands and was spinning in place
  - we have also recorded audio file with each step
- This experiments consists of several miniexperiments:

  - 1. Peter were facing himself the anchor bacesline and holding the Tag 2 such that antenna were facing anchors.
  - 2. Peter was still facing the anchor baseline, but this time he has rotated the tag, such that tag were directed in another, opposit direction
  - 3. Peter switched the direction, so that he was looking to the anchor baseline by back (looking in opposite direction), and holding the tag, so that it was facing the anchor baseline.
  - 4. Peter was still in the direction opposite to the anchor baseline, but this time, tag was also in the direction opposite to the anchor baseline.

  OBSERVATION: When tag and Peter were facing the anchor baseline, communication was smooth whithout drops in the connection. When tag was facing the anchor baseline but Peter was in the opposite direction, signal was good only when tag where at the distance less than 4-5 m, in further distances signal was getting lost. When Peter and a tag where directed in the opposite direction to the anchor baseline, signal were lost, and tag was able to reach anchors. When Peter was facing the anchor baseline and tag was directed in the direction opposite to the anchor baseline, communication was stable, but distance was greater than the actual due to the signal propagation. In all cases it holds that when a tag is very close or even touching a person, signal was lost. Only when both: tag and a person were facing the anchor baseline, connection was good (considering that tag was too close to the person)

- TODO: Think about the organization of files, naming conventions, file, directory hierarchies.

# September 30, 2023 - Experiments at university

Considering 2 anchors and 1 tag only, we have decided to perform two types of experiments:

1. Place tag at one place for cca 1 minut and measure the distances; for each meter (1m, 2m, 3m etc.) create a separate file containing measured distances.
2. Do not create separate files, but rather create one containing all the data, and use aruco markers in a way, that id of the marker will how the actual distance at which a tag is placed (id 1 corresponds to 1m, id 2 to 2m etc.).
   - markers can be easer places along side with tag, or right before the camera - second leads to the better accuracy of marker recognition

We did three experiments:

1. Separate measurements
2. Aruco markers
3. Tag-Anchor facing each other - was not recorded due to the bug in the program
   - Anchor 101 and Tag 2 calculations
   - With known distances between anchors and ground truth distance between Anchor 102 and Tag 2, we can compute the distance between Anchor 101 and Tag 2 and compare it we measured distance
   - Distance between 1 and 2 lines is 1.64 m
   - from 1 m to 5 m anchor and tag was facing each other, and result was better/closer to ground truth. But after 5m (starting from 4m), even at 5 m it didn't matter if tag and anchor were parallel or not
   - When tag is placed in 1 m from A101, it showed distance 1.82 m to A102 and ground truth is 1.92 m
   - This experiment also have separate files showing difference between parallel and facing placements

Questions:

- Does it matter when tag and anchor facing each other (should be rotated to face each other when placed on different lines)
  - YES, otherwise it adds some cm

TODO: From measured data create sketter plot showing the correlation/deviatoin from the groud truth value.

TODO: To sync aruco markers captured using camera and measurements from the anchors.

# 7 October, 2023 (Experiments)

Mention in the text about large behavioral model

1. During the first experiment I have noticed that when anchors are places close to the cabinet (шкаф), the distance appears to be smaller (4.96 instead of 5)

2. Tag 2 vs Anchor angle experiment:

- Popis: Tag 2 is communicating with anchors 101 and 102, Anchor 101 and Tag 2 are placed on the same line 1, andhor 102 is placed on the line 2 which is parallel to line 1. Tag 2 is moving along the line 1 only and at step 1 m. Therefore distance between anchor 101 and tag 2 must be precidsed.

  1. Anchor and tag are facing each other, placed at the angle such that they see each other.

     - naming convention of the file: distance\_{x}m_anchor_facing_tag.txt

  2. Anchor and tag are not facing each other, parallel placement

     - naming convention of the file: distance\_{x}m_anchor_is_parallel_to_tag.txt

3. one Tag ID=2, two anchors 101 and 102, stereo camera

At this experiment I did not focus on precision, but rather on camera and beacno synchronization
Therefore at the distance 5 m (calibration distance) beacons showed 4.90~4.96 (before was 4.97~5.02) why?
Camera calibration is also not good. At the distance 4m it was precised, but at the distance 6.46 it showed 6.2, at the distance 8.46 it showed 8m, why? bad calibration?

- distance between cameras 13.3 cm
- distance between anchors 1.67
- distance between camera baseline and anchor baseline is 1.46~1.47m

# 26 October, 2023

Changed the message format to json
Added time profiling

# 28 October, 2023

Small experiment at university

- This experiment is mainly focused on comparison of distance measured by beacons and the actual distance (ground truth). Ground truth is defined by the papers on the floor as well as using aruco markers. Unique ID of the aruco marker shows the ground truth distance.

- Server is multithreaded. One thread is responsible for communication with beacons (Server) and another is responsible for video recording.

- The outcome of this experiment must be scatter plot showing the shift of measured distances using beacons against the ground truth distances.

- Tag was placed at each meter from 1 to 9. Each meter has its own aruco marker assigned, for example 1m corresponds to Aruco marker with id 1, 2m - id 2, 3m - id 3 etc. 

- Distance between anchors is 131 cm.
- Only one tag (ID=2) was utilized during this experiment.

# 4 November, 2023

First of all, we should prove the theory that data collected from beacons can be used as a Golden Standard (GS) or Ground Truth (GT), i.e. they correspond to the actual GS data showing the true distance, such as aruco markers or papers on the floor.

To collect such data, we need to know that the data at the given time corresponds exactly to the GT data given by papers on the floor or Aruco markers. And the fewer preprocessing the recorded data involves the better and more precise the data analysis performed during the proof step is. There are several different methods how to record data from beacons: place a tag at different positions and use Aruco markers to show what the actual distance is.

As an another example we can use a predefined template/plan consisting of different stages where each stage will show where each tag should be placed and place tags accordingly (plan can be a simple draft on the paper). To show that this is the time to measure the data there are many different methods that can be used, such as the use of a pause/continue button to stop/start recording when the environment should be prepared, or when it is ready to be recorded.

Another example is to use some object which when appearing in the recorded video shows that this is the time to collect the data from beacons. This object can be anything such as simple white paper or some aruco marker.

There are two types of data processing:

1. Online
2. Offline

## Online

There can be a single program combining data collection (beacon data as well as data from video) and data processing including further data analysis. It can be a multithreaded program where one thread is running server which communicates with beacons, and another thread is running stereo vision camera with people and mutual distance detection. In this case there is no need for, or it is not mandatory to have a synchronization, because once we get data from server we can immidiately compare it with the data collected from video at the given time. Data analysis then can compare the distnaces collected via beacons and via video.

## Offline

Offline method can be split into several separate parts or modules: Data Acquisition, Data processing, Analysis of processed data. As our main next step is to prove that the data collected from beacons can be used as GS data, data acquisition step may consist of collecting positions of / distances between beacons as well as collecting video frames from video stream but without immidiate processing. Those collected data may be used by other researchers as the base which they can rely on and design their own methods for distance calculation.

As the next step, data from beacons that are making GS can be used as a reference system for several other methods used in video processing. For example, beacon data can be used to decide whether stereo vision method works good and computes the position of a person precisely. As another example, data collected from beacons may be used as annotations for machine learning model which then can predict the position of a person given a single camera video stream only.

And as the last but not least step the processed video data given by the evaluated model can be compared with GS data collected via beacons in data analysis step.

# 5 November, 2023

- Experiment at school. Set up: One tag ID = 2; two anchors.

- The first goal of this experiment in comparison to 28.10.23 experiemnt is to understand when tag was moved and when was stood at one place. For this purpose it is sufficient to use only one aruco marker just to show the moving stage. 
  - When aruco is shown - stationary stage
  - When aruco is hidden - moving stage

- Second goal is to:
  - have a manual plan of tag placement (at each meter it must be placed).
  - compare ground truth distance from plan to the actually measured data

# 12 November, 2023

- Experiment at school. 
- Setup: 3 tags, 2 anchors. Calibration distance is 5m. Max test distance is 12m.

Alignment: 
   Anchor 1<------2.087m------>Anchor 2
    (Orange)                    (White)
      |             |             |
      |<--1,0485m-->|<--1,0485m-->|
      |             |             |
     Tag 1         Tag 3         Tag 2
    (Green)        (Pink)       (Yellow)
    
Distances:
    - between anchors: 2.087m
    - Tag 1 <-> Tag 3: 1,0485m
    - Tag 2 <-> Tag 3: 1,0485m

- Two experiments were carried out: 

First experiemnt:
  - The goal is to measure the precision of the measurements. 
  - Tags were placed sequentailly every meter starting from 1 to 12 meters.
  - Calibration were perfomed for 5m
  - aruco is used to show the stationary and moving stages.

Second experiemnt:
  - Random placement of tags.
  - aruco is used to show the stationary and moving stages.
  - The goal is to detect some anomalies such as reflexion, tag overlay etc.
  - more in photo showng the plan of the experiment.

# 18 November, 2023

- Experiment at school
- Setup: 3 tags, 2 anchors. Calibration distance is 5m. Max test distance is 15m.

Alignment: 
  Anchor 1<-----2.5m----->Anchor 2
  (Orange)                (White)
    |           |           |
    |<--1,25m-->|<--1,25m-->|
    |           |           |
    Tag 1      Tag 3       Tag 2
   (Orange)    (Pink)      (White)

Distances:
    - between anchors: 2.5 m
    - Tag 1 <-> Tag 3: 1.25m
    - Tag 2 <-> Tag 3: 1.25m

- Two experiments were carried out:

- First experiment:

  - The goal of this experiment is to measure precision of the measurements. The differemce from the previous experiment (12.11.23) is that tags were moved more rapidly with less time between stages (movements).
  - aruco is used to show the stationary and moving stages.
  
- Second experiment: 

  - The goal of this experiemnt is to place tags at random places and analize their behavoir, detect some anomalies. 

  - The main difference from the prevoius experiments: aruco marker were shown during the whole experiment. So, now it cannot be used as a reference when tags were moved and when were stood. 

  # 20 November, 2023

  - Need to choose speed of moving, and how much time we need to stay on the place
  - Call Peter at saturday

  # 23 November, 2023

  During epxeriments at school I have realized that the answer from tag sometime take longer than usual, common time is 250 ms. The time is measured starting when the signal sent to the tag (Mer!) and till response with distances is reseived.

  - I have checked the time of beacon communication at the level of beacons and the whole process starting from discovering anchors and communication with all of them takes around 230-250 ms (need to correct again, because I have used arduino timestamps in the arduino terminal, and these timestamps are not so precise).
  
  - Therefore I can conclude that there is the time delay on the server side. There is no delay inside loops, but it looks like there is delay with the socket function "select".

  - TODO: to show where can be the problem. Show the time spent on beacon communication (direct, not including server), and overall time of the communication including server side.


# 25 November, 2023

- Experiment at school

- Before experiment:
  
  - There is a need to check the speed and reach. I have changed the timer for the blink from 1000 to 80 ms. Whould this change affect the speed and reach?

  - Also, it would be good to measure distances with tags calibrated at 1 m and at 12 m.
    - Do we need to measure such long distances?

- Experiment:
  - Distance between anchors: 250,5 cm
  - Distance from camera to anchor baseline is 160,5 cm
  
- First experiemnt:

  - Without aruco marker, moving faster. 
  - Tags are calibrated at 5 m
  - blink 80 ms
 
Previous experiments were focused mainly on the measurement of distance when tag is placed at the line of anchor. The distance between LOS (Line of sight) anchor and tag was 5m. An another anchor and the tag was not aligned on the same line and didn’t have direct line of sight - but they was parallel. Result of the experiment showed that first anchor and tag (aligned measurement) had precise 5m distance, but another anchor and tag had a bit higher distance than the actual was.

As the next step it was needed check the precision of distance measurement along the diagonal (tag and another anchor).

During previous experiments it was detected that some of the measurements take longer than the other. Due to this fact I have decided to change blink from 1000 to 80 ms. This didn’t affect the communication between beacons themselves and between tags and server, but improved time of measurement. 

This experiment is performed at the corridor where Peter is sitting (at 3th floor). 

There are 3 lines along which tags are moved. Two lines are aligned with anchors. //TODO: create plan of corridor and placement of beacons. 


The goal of the experiment is to check the quality and precision of the UWB beacons measurements. During all experiments system were consisting of 2 anchors and 1 tag: 

1. Check the quality of the first tag. 
2. Check the quality of the second tag 

- Setup for the first experiment: 
  - Anchor 101, Anchor 102, tag 1. 

  - step 1: Alignment with Anchor 101.
    - Objective: To evaluate the communication and distance measurement bertween Tag 1 and Anchor 101.
    - Process: Tag 1 is aligned with Anchor 101, ensuring they have a Line of Sight (LOS)
    - Observation: The measured distances between Tag 1 nad anchor 101 remain accurate and consistent throughout this step pf the experiment.

  - step 2: Alignment with Anchor 102 
    - Objective: To evaluate the communication and distance measurement between Tag 1 and Anchor 102.
    - Process: Tag 1 is aligned with Anchor 102 ensuring LOS.
    Initial observation: The distance measurements between Tag 1 and Anchor 102 appear to be accurate.
    Subsequent observation: After a period of communication, the measured distances between Tag 1 and Anchor 102 start decrease, indicating a potential issue or anomaly in the measurement or communication.

  - step 2.1: Alignment with Anchor 102
    - Additional tests: Upon reconnecting the tag to anchor 102 after a break (repeating this several times), the distance measurements still show a reduction.

  - Outcome of the step 2: The observed erro in measurements is about 40 cm, with the measurements showing stable behaviour (+-5cm fluctuation) and not exhibitig large jumps between vastly different values. 


- Overall outcome of the experiment: 
  - In repeated testsm sometimes the distances are correctly measured, but other times they do not. 
  - Hypotheses for the issue:
    - TWR Implementation problem: There might be an issue in the implementation of the Two-Way Ranging protocol, leading to ineffective communication when the beacons are overwhelmed. 
    - Manufacturer issue: Te problem might also be a manufacturing defect inside the beacons.
    
- Next steps:
  - Further investigation and checks are needed to isolate the case of the inconsistent distance measurements.


# 2 December, 2023

Experiment overview: 
  - All experiments are conudcted on the 1st floor of Mala Strana. The data collected is limited to tagging, with no video recordings involved.

Experiment 1: Review of Tag 1.

Tag 1 is calibrated on 5m.

Common Setup:
  - Distance between Anchors is 250,5 cm

Objective: 
  - Test the influence of the enviroment on distance estimation

This experiment comprises of multiple sub-experiments:

Sub-experiment 1.1:
  
  Setup details:
    
    Anchor placement:
      - Anchor 101: Placed on the Door Line.
      - Anchor 102: Placed on the Window Line.
    
    Tag placement:
      - Center

    Data collection: 
      - Data are collected from both anchors at the same ranging cycle.

  Result:
    Median and average values:
      Anchor 101:
        Median: 4.9404271
        Average: 4.938610753198653
      Anchor 102:
        Median: 4.940427
        Average: 4.94042727946128
      - from Anchor 101 and Anchor 102 are 4.94. By Pythagorean theorem, they should equal to sqrt(5^2 + 1.2525^2) = 5.155.
    
    Minimum and Maximum values:
      Anchor 101:
        - Min: 4.8278251
        - Max: 4.9826531

      Anchor 102:
        - Min: 4.879434
        - Max: 4.996728

    - I can conclude that Tags have offset
    
  Conclusion:
    - The calculated values suggest that there is an offset in the tags. 

Sub-experiment 1.2:
  
  Setup details:
    
    Anchor placement:
      - Anchor 101: Placed on the Door Line.
      - Anchor 102: Placed on the Window Line.
    
    Tag placement:
      - Door line
      - Line Of Sight with Anchor 101
      - Parallel to Anchor 102  

    Data collection: 
      - Data are collected from both anchors at the same ranging cycle.

  Result:
   Median and average values:
      Anchor 101:
        Median: 4.8559761
        Average: 4.8574475549019605
      Anchor 102:
        Median: 5.569124
        Average: 5.568626788235295
      
    By Pythagorean theorem, distance between Tag 1 and Anchor 102 should be equal to sqrt(5^2 + 2.5050^2) = 5.5924.
    
    Minimum and Maximum values:
      Anchor 101:
        - Min: 4.8043661
        - Max: 4.9169681

      Anchor 102:
        - Min: 5.484672
        - Max: 5.630116

    - I can conclude that Tags have offset
    
  Conclusion:
    - Median and average distances to Anchor 101 suggest that there is an offset. Also, based on the previous measurements: when tag and anchor are places in parallel and non facing each other (has no LOS), measured distance between them must be higher than the actual is. This dact also signals that there is some offset in measurements.

 Sub-experiment 1.2:
  
  Setup details:
    
    Anchor placement:
      - Anchor 101: Placed on the Door Line.
      - Anchor 102: Placed on the Window Line.

# 9 December, 2023

- Experiemnt 1 Tag 1 Anchor 103

  - Schema is shown in the video

  - Tag 1 (which was working perfectly in previous tests) starts blinking. There is a difference in powerbank power. Tag has powerbank with 4/4 (full) power. Anchor has powerbank with 3/4 power 

- Experiemnt 2 Tag 1 Anchor 101

  - Schema is shown in the video

  - Tag 1 not Anchor 101 are not blinking. Both have 4/4 power.

- Experiment 3 Tag 1 Anchor 102 (not recorded)

  - Surprisingly, Anchor 102 worked good. 
    - But here is strange thing observed:
      - measurements where done when diagonal was 5.15 ~ (sqrt(25+(1.2525)^2)) (half diagonal)
      - first measurement was when Tag was placed at the center, and anchor was placed near window. Measured distance was 5.32
      - Tag - center, Anchor - Wall, distance: 5.22m
      - Tag - wall, anchor - center, distance: 5.34m
      - Tag - window, anchor - center, distance: 5.20m

    
      This can explain why fully diagonal test 5.59 ~ (sqrt(25+(2.5050)^2)) showed different values (Tag - wall, anchor - Window; Tag - window, Anchor - wall)

- Experiment 4 (Recorded).

  - Last experiment showed that there might be interferention caused by USB cable. 
  - Therefore, in this experiment, tag 1 and anchor 102 are rotated such that they facing each other by the corner of antenna, and cable is placed in the back part. Beacons are facing each other by open end (counterpart of where cable is connected).

  In all cases distance was ~5.4 m.

- Experiment 5 (cables)

  - Finally, I found issue. It is cable connection. OMG.

- Experiment 6 (Tag 2, Anchor 102)

- Testing all cases

- Including half-diagonal test


Pozorovani: Tags may start blinking, and thus showing values a bit shifted from Ground Truth values. Try to place Tag vertically to detect cable interference.

Diagonal tests show that there may be diagobal interference.



What to do next: Place beacons inside box and check.


# 16 December, 2023

- Distance between anchors: 250,5 cm
- Diagonal and straight tests


Experiment set 1 - Straight line of sight:
Anchor and Tag are placed at direct line of sight at each line (Near door, center, near window)
These set of experiments aims to check reflextions/distortions that might be caused by some conditions
Anchors are without changed antenna delay (default)
All (roury) are aligned using electric lasers.

- Experiment 1.1 (Straight test - Anchor 102, Tag 2)

- Tag 2 (Antenna delay - 16530. With this antenna delay, Anchor 102 and tag 2 were showing 5m at callibration stage)
- Anchor 102

At each line distance where correct 4.98-5.02

- Experiment 1.2 (Straight test - Anchor 102, Tag 1. With this antenna delay, Anchor 102 and tag 2 were showing 5m at callibration stage) 

- Tag 1 (Antenna delay - 16500)
- Anchor 102

First part of the experiment was OK, but at some point distances started detected wrong. May it be because of cable connection? - Yes (understand from next experiment)

- Experiment 1.3 (Straight test - Anchor 101, Tag 1) - After change of cable

Distance is about 5.20 - 5.30 at each line.

- Experiment 1.4 (Straight test - Anchor 101, Tag 2)

All cables seems to be deffected. Data are not stable, sometimes ranging between 5.15-5.25, sometimes between 4.70-4.80 even with calibration for 5m. See previous experiments 1.1, 1.2, 1.3 - in all tests distance were correct, or it was accidentally?

Need to check with new cables.

# 17 December, 2023

Antenna delay is set to 16515 for all tags. Experiments are performed using new cables from Alza.
All (roury) are aligned using electric laser (see photo). I draw lines on (roury) to be able to put beacons on them faster.

- Experiment 1.5 (Straight test - Anchor 102, Tag 3)

Result are correct at all lines

- Experiment 1.6 (Straight test - Achor 101, Tag 3)

Results are correct at all lines. It might happen that at some point tag starts showing +5 cm higher values. See beginnign of the recorded data and end (at those parts tag and anchor where placed on the line near Window - window line).

- Experiment 1.7 (Straight test - Anchor 101, Tag 2) - Repetition of unsuccessful yesterday's test.

All distances are about 5.07. Based on the fact that Anchors have default antenna delay and based on Experiment 1.6 (which has also measured distances ~5.07), there might be a need to calibrate antenna delay on Anchor 101. Because distances between tags and Anchor 102 are about 5.00.

- Experiment 1.8 (Straight test - Anchor 102, Tag 2) - Repetition of yesterday's test, but with another antenna delay.

No, there is no need to calibrate anchor 101. Now all distances between Tag 2 and Anchor 102 are around 5.07-5.10. In this experiment I have found that (roury - pipes?) on tag line are not aligned. 

- Experiment 1.9 (Straight test - Anchor 102, Tag 1) - Repetition of yesterday's test

Tag 1 shows ~5.00 at each line. I thought that after some while tag starts showing a bit higher values. At this experiment I have left Tag 1 to tun for a while (at center line). Experiment shows that even after some time of work, tag shows same distance. Measured distance at all lines where ~5.00.

- Experiment 1.10 (Straight test - Anchor 101, Tag 1) - Repetition of yesterday's test

This setup also showing distance ~5.07. There is need to a bit upper antenna delay.

- Experiment 1.11 (Straight test; Door line - Anchor 102, all tags)

This experiment showed that all tags show ~5.00 m measured distance. 

 - The only tag 3 shows ~4.90; but previous experiments 1.5-1.6 showed that after some time, measured distances get normalized, maybe some deffect inside chip.

Therefore, I may conclude that anchor 101 might require some calibration. Because all tags show distance 5.07 with this anchor.


- Experiment 1.12 (Straight test; Door line - Anchor 101, all tags)

This experiment show that Anchor 101 - Tag 1, Anchor 101 - Tag 3 distances are 5.00. Anchor 101 - Tag 2 is 5.07.

Ahcnor 102 - Tag 2 distance (end of the experiment) is 5.00 m.

Overall result of today:

  - The only thing is that tag 3 might increase distance in a time. Therefore, Tag 2 and Tag 3 show ~5.07 measured distance with Anchor 101. Therefore, there is a need to upper antenna delay on Anchor 101. The only thing is Anchor 101 - Tag 1 which is 5.00m. Strange.

What to do next: 1. Try diagonal test. 2. Put tags into boxes and test for interference and reflexion.


# 22 December, 2023

- Experiment in dormitory

Objective: To test different environment with calibration parameters for another environment (1st floor of MS). Antenna delay: 16515. 

- 

# 23 December, 2023

1st try: 

Experiment overview: 
  - All experiments are conudcted on the 3st floor of Kajetanka dormitory. Experiment involves recording of tag data as well as video recording 

Setup (As seen on camera):
  - Left Wall <--- 0.5 m ---> Left Anchor Line <--- 0.6 m ---> Central Anchor Line <--- 0.6 m ---> Right Anchor Line <--- 0.3 m ---> Right Wall
  - Anchor baseline <--- 1.28 m ---> Camera
  - Camera <--- Unknown ---> Right Wall

Tags are calibrated on 5m (1st floor in MS).

Disclamer: Because of the situation happened I decided to perfrom experiments at the dormitory. This has a benefit that I can test antenna calibration parameter calibrated at 1st floor of MS in different environments. Worser thing is that antenna was calibrated a bit wrong. At 5m GT distance it shows 5.05-5.12 (~5.07). The reason is that at the first run, tags show a bit lesser values/distances, and after a while, they start showing +5-10 cm. 

Experiment 1: Review of Tag 2 and Anchor 102 in dormitory.


Common Setup:
  - Distance between Anchors is 250,5 cm

Objective: 
  - Test wether calibrated antenna delay from 1st floor of MS works for other environments.

Observation:
  - The floor is not straight. Therefore, pipes might have some tilting. This might influence the precision of measurements.

Result:
  - Same behaviour as in MS observed: At the first, tag is showing average distance 4.98, but after a while it starts showing 5.07 m on average.


What to do next: 
- Check why tag 1 was showing distance 4.98-5.00 at 1st floor of MS, and now showing ~5.07 on average. Check with which anchor Tag 1 showed 4.98 in MS.
- Why after some time of run, tags start to show +5-10 cm? Do they need warm up stage?


Experiment 1.2: Review of Tag 1 and Anchor 102 in dormitory.

# 24 December, 2023

Change of setup:
  - Camera position is changed
  - Left Wall <--- 0.5 m ---> Left Anchor Line <--- 0.6 m ---> Central Anchor Line <--- 0.6 m ---> Right Anchor Line <--- 0.3 m ---> Right Wall
  - Anchor baseline <--- 1.3 m ---> Camera
  - Camera <--- 1.20m ---> Right Wall                 <--- Change from yesterday

Experiment 1.3: Review of Tag 3 and Anchor 102 in dormitory.


Results:
  - Overall values get a bit higher after some while of running.
  - There is some level of interference near Right Wall (Values get higher ~ +5 cm).

Experiment 1.4: Review of Tag 1 and Anchor 101 in dormitory.

Results:
  - Values are immidiately around ~5,07 m. (without warm up).
  - There is also some level of interference near Right Wall (+10 cm).

Experiment 1.5: Review of Tag 2 and Anchor 101 in dormitory.

Results:
  - Why values are around 5.15? Higher than between Anchor 101 and Tag 1.
  - This confirms previous result of experiments performed on 17. of December: Anchor 101 shows a bit higher values on average for Tag 2 and Tag 3.
  - Level of intereference on the Left Line is present.

Experiment 1.6: Review of Tag 3 and Anchor 101 in dormitory.

Results:
  - Average distance measurement is 5.07-5.10, same as for Tag 2 <--> Anchor 101. This confirms the fact that anchor 101 need to adjust antenna delay.
  - 

Experiment 1.6.1: Review of Tag 3 and Anchor 101. Long run test.

Objective:
  - To test the change in measurements in time.

Results:
  - This test confirms that distance measurements get higher in time for Tag 3.

Experiment 2: Interference of Right Wall for Tag 1 and Anchor 101

Result:
  - There is a light interference +3-5 cm on Right Wall Line.
  - Why with Anchor 102 all tags had higher interference on Right Wall Line?



2nd try (Determining which and for how much to adjust antenna delay):

Experiment overview: 
  - All experiments are conudcted on the 3st floor of Kajetanka dormitory. Experiment involves recording of tag data as well as video recording 

Setup (As seen on camera):
  - Left Wall <--- 0.6 m ---> Left Anchor Line <--- 0.5 m ---> Central Anchor Line <--- 0.5 m ---> Right Anchor Line <--- 0.4 m ---> Right Wall
  - Anchor baseline <--- 1.31 m ---> Camera
  - Camera <--- 1.2 m ---> Right Wall

Tags are calibrated on 5m (1st floor in MS).

Experiment 1.1: Anchor 101, Tag 1

Result:
  - Right Wall Line:
    - Min: 5.01
    - Max: 5.10 (once 5.13)
    - Avg: 5.05
  - Central Line:
    - Min: 5.04
    - Max: 5.16 (once 5.17)
    - Avg: 5.09
  - Left Wall Line:
    - Min: 5.03
    - Max: 5.15
    - Avg: 5.10
  - Right Wall Line (2nd):
    - Min: 5.05
    - Max: 5.13
    - Avg: 5.08
  - Central Line (2nd):
    - Min: 5.08
    - Max: 5.17
    - Avg: 5.11
  - Left Wall Line (2nd):
    - Min: 5.08 (once 5.07)
    - Max: 5.16 (once 5.17)
    - Avg: 5.10-11

Result:
  - I may conclude that after some time values get higher.


Experiment 1.2: Anchor 101, Tag 2

Result:
  - Right Wall Line:
    - Min: 5.05
    - Max: 5.14
    - Avg: 5.09
  - Central Line:
    - Min: 5.09
    - Max: 5.20
    - Avg: 5.15
  - Left Wall Line:
    - Min: 5.11 (sometimes 5.09)
    - Max: 5.17
    - Avg: 5.14
  - Right Wall Line (2nd):
    - Min: 5.09
    - Max: 5.17 (sometimes 5.19)
    - Avg: 5.12
  - Central Line (2nd):
    - Min: 5.12 (sometimes 5.10)
    - Max: 5.21
    - Avg: 5.17
  - Left Wall Line (2nd):
    - Min: 5.10
    - Max: 5.17 (sometimes 5.19)
    - Avg: 5.13

Result:
  - I may conclude that after some time values get higher. Looks like after first move of beacons.
  - After values get normalized. 1st (Central Line, Left Wall line) and 2nd runs have same meausrements on average.

Experiment 1.3: Anchor 101, Tag 3

Result:
  - Right Wall Line:
    - Min: 5.02 (once 5.00)
    - Max: 5.12 (once 5.15)
    - Avg: 5.07
  - Central Line:
    - Min: 5.08
    - Max: 5.15
    - Avg: 5.13
  - Left Wall Line:
    - Min: 5.07
    - Max: 5.14
    - Avg: 5.09
  - Right Wall Line (2nd):
    - Min: 5.07 (once 5.02)
    - Max: 5.12
    - Avg: 5.07-09
  - Central Line (2nd):
    - Min: 5.08
    - Max: 5.14 (once 5.17)
    - Avg: 5.10
  - Left Wall Line (2nd):
    - Min: 5.05
    - Max: 5.14 (once 5.17)
    - Avg: 5.10

Result:
  - This also confirms that after some time of run, values are getting higher


# 25 December, 2023

Experiment 1.4: Anchor 102, Tag 1

Result:
  - Right Wall Line:
    - Min: 4.91
    - Max: 5.02 (sometimes 5.03)
    - Avg: 4.98
  - Central Line:
    - Min: 4.99 (once 4.95)
    - Max: 5.09
    - Avg: 5.03
  - Left Wall Line:
    - Min: 4.99
    - Max: 5.09
    - Avg: 5.03
  - Right Wall Line (2nd):
    - Min: 4.96
    - Max: 5.07
    - Avg: 5.02
  - Central Line (2nd):
    - Min: 5.02 (sometimes 5.00) 
    - Max: 5.09 (sometimes 5.10)
    - Avg: 5.03 
  - Left Wall Line (2nd):
    - Min: 5.01 (sometimes 4.99)
    - Max: 5.08
    - Avg: 5.02

Result:
  - This test confirms that Tag 1 and Anchor 102 are calibrated almost correct and has lower values that Tag 1 and Anchor 101
  - Also it confirms that in a time values get higher (small change here)

Experiment 1.5: Anchor 102, Tag 2

Result:
  - Right Wall Line:
    - Min: 4.99
    - Max: 5.07
    - Avg: 5.02
  - Central Line:
    - Min: 5.00
    - Max: 5.11 (sometimes 5.13)
    - Avg: 5.07
  - Left Wall Line:
    - Min: 5.03
    - Max: 5.11 (sometimes 5.15)
    - Avg: 5.08
  - Right Wall Line (2nd):
    - Min: 4.98
    - Max: 5.10
    - Avg: 5.06
  - Central Line (2nd):
    - Min: 5.00
    - Max: 5.12 (sometimes 5.14)
    - Avg: 5.08 
  - Left Wall Line (2nd):
    - Min: 5.04
    - Max: 5.10
    - Avg: 5.09

Result:
  - All measurements are almost same. No value drifting in time observed
  - Maybe small correction of antenna delay needed

Experiment 1.6: Anchor 102, Tag 3

Result:
  - Right Wall Line:
    - Min: 4.88 (once 4.84)
    - Max: 5.01 (once 5.03)
    - Avg: 4.95
  - Central Line:
    - Min: 4.97
    - Max: 5.06
    - Avg: 5.02
  - Left Wall Line:
    - Min: 4.97 (sometiems 4.95) 
    - Max: 5.06
    - Avg: 5.02
  - Right Wall Line (2nd):
    - Min: 4.99 (somestimes 4.94)
    - Max: 5.04 (once 5.07)
    - Avg: 4.99-5.00
  - Central Line (2nd):
    - Min: 4.97
    - Max: 5.09
    - Avg: 5.03
  - Left Wall Line (2nd):
    - Min: 4.99
    - Max: 5.06
    - Avg: 5.02

Result:
  - Tag 3 requies warm up for sure.
  -


Overall result of experiments above:
  - Tag 1 has good calibration - 16515
  - Anchor 102 has good calibration - default: 16384
  - Tag 2 must be recalibrated - 16536
  - TAg 3 must be recalibrated since in a time it shows a bit higher values - 16526
  - Anchor 101 must be calibrated - 16390
  - 

Experiment 2 (after calibration)

Experiment 2.1: Anchor 101, Tag 1

Result:
  - Right Wall Line:
    - Min: 4.96
    - Max: 5.04
    - Avg: 4.99
  - Central Line:
    - Min: 5.01
    - Max: 5.09
    - Avg: 5.05
  - Left Wall Line:
    - Min: 5.00
    - Max: 5.08
    - Avg: 5.05
  - Right Wall Line (2nd):
    - Min: 4.98
    - Max: 5.06 (sometimes 5.08)
    - Avg: 5.02
  - Central Line (2nd):
    - Min: 5.02
    - Max: 5.12
    - Avg: 5.07
  - Left Wall Line (2nd):
    - Min: 4.98
    - Max: 5.08 (sometimes 5.10)
    - Avg: 5.03 

Result:
  - Sometimes values are higher. Is it when tag and anchor restarted or values change in time?
  - Should I change antenna delay also on Tag 1? Need to check with Anchor 102

Experiment 2.2: Anchor 101, Tag 2

Result:
  - Right Wall Line:
    - Min: 4.90
    - Max: 4.99
    - Avg: 4.95
  - Central Line:
    - Min: 4.96
    - Max: 5.02
    - Avg: 4.99
  - Left Wall Line:
    - Min: 4.94
    - Max: 5.01
    - Avg: 4.98
  - Right Wall Line (2nd):
    - Min: 4.92
    - Max: 5.00
    - Avg: 4.96
  - Central Line (2nd):
    - Min: 4.98
    - Max: 5.03
    - Avg: 5.00
  - Left Wall Line (2nd):
    - Min: 4.96 (once 4.94)
    - Max: 5.02
    - Avg: 4.99

Result:
  - Overall values are OK.
  - Left Wall has some level of interference. Values are lower than average among all lines.
  - Does this tag requires warm-up?

Experiment 2.3: Anchor 101, Tag 3

Result:
  - Right Wall Line:
    - Min: 4.85
    - Max: 4.95
    - Avg: 4.91
  - Central Line:
    - Min: 4.91
    - Max: 5.02
    - Avg: 4.98
  - Left Wall Line:
    - Min: 4.92
    - Max: 5.03
    - Avg: 4.97
  - Right Wall Line (2nd):
    - Min: 4.91 (once 4.88)
    - Max: 5.01
    - Avg: 4.95
  - Central Line (2nd):
    - Min: 4.97
    - Max: 5.03
    - Avg: 4.99
  - Left Wall Line (2nd):
    - Min: 4.96 (once 4.94)
    - Max: 5.03
    - Avg: 5.00

Result:
  - Left Wall again show some level of interference.
  - In a time value get higher.

Experiment 2.4: Anchor 102, Tag 1

Result:
  - Right Wall Line:
    - Min: 4.91
    - Max: 5.02
    - Avg: 4.95
  - Central Line:
    - Min: 4.98
    - Max: 5.09
    - Avg: 5.02
  - Left Wall Line:
    - Min: 4.97 (sometimes 4.95)
    - Max: 5.08
    - Avg: 5.03
  - Right Wall Line (2nd):
    - Min: 4.98
    - Max: 5.06
    - Avg: 5.00-5.02
  - Central Line (2nd):
    - Min: 5.01
    - Max: 5.11
    - Avg: 5.05 (get lower values in a time)
  - Left Wall Line (2nd):
    - Min: 4.96
    - Max: 5.06
    - Avg: 5.02
  - Central Line (3rd):
    - Min: 5.00
    - Max: 5.08
    - Avg: 5.04 (get lower values in a time)

Result:
  - May be an interference from walls?
  - Why values at center line are higher?
    - No, need to upper antenna delay, set same as on Tag 3.

Experiment 2.5: Anchor 102, Tag 2

Result:
  - Right Wall Line:
    - Min: 4.94 (at the beginning 4.92)
    - Max: 5.03
    - Avg: 4.97
  - Central Line:
    - Min: 4.95
    - Max: 5.03
    - Avg: 5.00
  - Left Wall Line:
    - Min: 4.97
    - Max: 5.02
    - Avg: 4.99-5.00
  - Right Wall Line (2nd):
    - Min: 4.96
    - Max: 5.03
    - Avg: 4.98-4.99
  - Central Line (2nd):
    - Min: 4.97
    - Max: 5.04
    - Avg: 4.99-5.00
  - Left Wall Line (2nd):
    - Min: 4.96
    - Max: 5.04
    - Avg: 4.99-5.00

Result:
  - OK
  -

Experiment 2.6: Anchor 102, Tag 3

Result:
  - Right Wall Line:
    - Min: 4.85
    - Max: 4.97
    - Avg: 4.90
  - Central Line:
    - Min: 4.92
    - Max: 5.02
    - Avg: 4.94
  - Left Wall Line:
    - Min: 4.92
    - Max: 5.01
    - Avg: 4.97
  - Right Wall Line (2nd):
    - Min: 4.94
    - Max: 5.01
    - Avg: 4.96
  - Central Line (2nd):
    - Min: 4.94
    - Max: 5.04
    - Avg: 4.99
  - Left Wall Line (2nd):
    - Min: 4.94
    - Max: 5.02
    - Avg: 4.99

Result:
  - Need warm up!
  -

Overall result:
  - Need to lower (a bit) antenna delay on Tag 3, to upper values.
  - Need to adjust antenna delay (same as on Tag 3?) on Tag 1.

# 26 December, 2023

Setup (As seen on camera):
  - Left Wall <--- 0.6 m ---> Left Anchor Line <--- 0.5 m ---> Central Anchor Line <--- 0.5 m ---> Right Anchor Line <--- 0.4 m ---> Right Wall
  - Anchor baseline <--- 1.31 m ---> Camera
  - Camera <--- 1.2 m ---> Right Wall
  - Warm up is 1-2 min

Experiment 3: Adjusting calibration for Tag 1 and Tag 3 - 16521

Experiment 3.1: Anchor 101, Tag 1 (After warm-up)
 
Result:
  - Right Wall Line:
    - Min: 4.95
    - Max: 5.02
    - Avg: 5.00
  - Central Line:
    - Min: 4.99
    - Max: 5.08
    - Avg: 5.02
  - Left Wall Line:
    - Min: 4.96 (once 4.93)
    - Max: 5.04
    - Avg: 5.01-5.02
  - Right Wall Line (2nd):
    - Min: 4.96
    - Max: 5.03
    - Avg: 4.99-5.01
  - Central Line (2nd):
    - Min: 4.99
    - Max: 5.07 (once 5.11)
    - Avg: 5.02-5.05
  - Left Wall Line (2nd):
    - Min: 4.98
    - Max: 5.06 (once 5.08)
    - Avg: 5.02

Result:
  - Overall is OK.
  - Why central line show a bit higher values?


Experiment 3.2.1: Anchor 101, Tag 3 (Warm-up & Long run)

Result:
  - Long run does not affect values once they are normalized.

Experiment 3.2: Anchor 101, Tag 3 (After warm-up)
 
Result:
  - Right Wall Line:
    - Min: 4.98
    - Max: 5.06
    - Avg: 5.02
  - Central Line:
    - Min: 5.00 (once 4.98-4.99)
    - Max: 5.06
    - Avg: 5.02-5.04
  - Left Wall Line:
    - Min: 4.96
    - Max: 5.06
    - Avg: 4.98-5.02
  - Right Wall Line (2nd):
    - Min: 4.97
    - Max: 5.04
    - Avg: 4.99-5.00
  - Central Line (2nd):
    - Min: 4.97
    - Max: 5.02-03
    - Avg: 5.08
  - Left Wall Line (2nd):
    - Min: 4.97
    - Max: 5.05
    - Avg: 4.99-5.01

Result:
  -

Experiment 3.3: Anchor 102, Tag 1 (After warm-up)
 
Result:
  - Right Wall Line:
    - Min: 4.96
    - Max: 5.04
    - Avg: 4.99-5.02
  - Central Line:
    - Min: 4.98
    - Max: 5.04
    - Avg: 4.99-5.02
  - Left Wall Line:
    - Min: 4.97
    - Max: 5.04
    - Avg: 4.98-5.02
  - Right Wall Line (2nd):
    - Min: 4.95
    - Max: 5.02
    - Avg: 4.98-5.00
  - Central Line (2nd):
    - Min: 4.98
    - Max: 5.04
    - Avg: 5.01-5.02
  - Left Wall Line (2nd):
    - Min: 4.97
    - Max: 5.03
    - Avg: 5.00

Result:
  - Tag 1 also requires warm-up!
  - all tags and anchors require warm-up!

Experiment 3.4: Anchor 102, Tag 3 (After warm-up)
 
Result:
  - Right Wall Line:
    - Min: 4.94
    - Max: 5.02
    - Avg: 4.97-4.99
  - Central Line:
    - Min: 4.96
    - Max: 5.03
    - Avg: 4.99-5.01
  - Left Wall Line:
    - Min: 4.96
    - Max: 5.03
    - Avg: 4.99-5.00
  - Right Wall Line (2nd):
    - Min: 4.94
    - Max: 5.03
    - Avg: 4.99
  - Central Line (2nd):
    - Min: 4.97
    - Max: 5.05
    - Avg: 5.00-5.02
  - Left Wall Line (2nd):
    - Min: 4.96
    - Max: 5.03
    - Avg: 4.99-5.00

Result:
  -

Experiment 4: 

Objective:

  - Boards are now enclosed into boxes.
  - Test of influence of box.

Experiment 4.1: Anchor 101, Tag 1
 
Result:
  - Right Wall Line:
    - Min: 4.97
    - Max: 5.03
    - Avg: 4.99-5.00
  - Central Line:
    - Min: 4.98
    - Max: 5.07
    - Avg: 5.02-5.03
  - Left Wall Line:
    - Min: 4.97
    - Max: 5.04
    - Avg: 5.00-5.01
  - Right Wall Line (2nd):
    - Min: 4.96
    - Max: 5.03
    - Avg: 4.99-5.01
  - Central Line (2nd):
    - Min: 4.98
    - Max: 5.06 (mosty 5.04)
    - Avg: 5.01-5.03
  - Left Wall Line (2nd):
    - Min: 4.98
    - Max: 5.04
    - Avg: 4.99-5.02

Result:
  - Looks like no interference is caused by boxes

Experiment 4.2: Anchor 101, Tag 2
 
Result:
  - Right Wall Line:
    - Min: 4.95
    - Max: 5.01
    - Avg: 4.98-99
  - Central Line:
    - Min: 4.97
    - Max: 5.02
    - Avg: 4.99-5.01
  - Left Wall Line:
    - Min: 4.97
    - Max: 5.02
    - Avg: 4.99-5.00
  - Right Wall Line (2nd):
    - Min: 4.95
    - Max: 4.98 (once 5.01)
    - Avg: 5.97
  - Central Line (2nd):
    - Min: 4.97
    - Max: 5.03
    - Avg: 4.99-5.00
  - Left Wall Line (2nd):
    - Min: 4.97
    - Max: 5.02
    - Avg: 4.99-5.00

Result:
  - Does box affects chips?

Experiment 4.3: Anchor 101, Tag 3
 
Result:
  - Right Wall Line:
    - Min: 4.92
    - Max: 5.01
    - Avg: 4.96
  - Central Line:
    - Min: 4.98
    - Max: 5.02
    - Avg: 4.99-5.01
  - Left Wall Line:
    - Min: 4.98
    - Max: 5.03
    - Avg: 4.99-5.01
  - Right Wall Line (2nd):
    - Min: 4.96 (somestimes 4.94)
    - Max: 5.02
    - Avg: 4.98-4.99
  - Central Line (2nd):
    - Min: 4.98
    - Max: 5.04
    - Avg: 5.02
  - Left Wall Line (2nd):
    - Min: 4.98
    - Max: 5.04
    - Avg: 5.01

Result:
  -

Experiment 4.4: Anchor 102, Tag 1
 
Result:
  - Right Wall Line:
    - Min: 4.93
    - Max: 5.00
    - Avg: 4.97
  - Central Line:
    - Min: 4.97
    - Max: 5.02
    - Avg: 4.99
  - Left Wall Line:
    - Min: 4.97
    - Max: 5.02
    - Avg: 4.99-5.00
  - Right Wall Line (2nd):
    - Min: 4.96
    - Max: 5.01
    - Avg: 4.97
  - Central Line (2nd):
    - Min: 4.97
    - Max: 5.02
    - Avg: 4.99-5.00
  - Left Wall Line (2nd):
    - Min: 4.97
    - Max: 5.03
    - Avg: 4.99-5.01

Result:
  -

Experiment 4.5: Anchor 102, Tag 2
 
Result:
  - Right Wall Line:
    - Min: 4.90
    - Max: 4.95
    - Avg: 4.94
  - Central Line:
    - Min: 4.94
    - Max: 4.98
    - Avg: 4.96
  - Left Wall Line:
    - Min: 4.96
    - Max: 5.00
    - Avg: 4.97
  - Right Wall Line (2nd):
    - Min: 4.94 (once 4.90)
    - Max: 4.97 (once 4.99)
    - Avg: 4.94-4.97
  - Central Line (2nd):
    - Min: 4.96 (sometimes 4.94)
    - Max: 5.01
    - Avg: 4.97-4.99
  - Left Wall Line (2nd):
    - Min: 4.96 (once 4.94)
    - Max: 5.00
    - Avg: 4.97-4.99

Result:
  -

Experiment 4.6: Anchor 102, Tag 3
 
Result:
  - Right Wall Line:
    - Min: 4.91
    - Max: 4.96
    - Avg: 4.94
  - Central Line:
    - Min: 4.97
    - Max: 5.02
    - Avg: 4.99-5.00
  - Left Wall Line:
    - Min: 4.96
    - Max: 5.04
    - Avg: 4.99-5.00
  - Right Wall Line (2nd):
    - Min: 4.95
    - Max: 5.02
    - Avg: 4.97-99
  - Central Line (2nd):
    - Min: 4.97
    - Max: 5.02
    - Avg: 4.99-5.00
  - Left Wall Line (2nd):
    - Min: 4.98 (sometimes 4.97)
    - Max: 5.03 (sometimes 5.04)
    - Avg: 4.99-5.00

Result:
  - At some point values get higher, and than again lower. Is it true? Looks like no



Overall result:
  - Need to lower antenna delay at Anchor 102. 

# 27 December, 2023

Experiment 5: Diagoal test

Objective:
  - Test different azimuth angles: tag and anchor are parallel or facing each other. Do we need to place anchors at some angle along azimuth? 

Setup:
  - Placement of HW is same as yesterday.
  - Tags and anchors are inside boxes.
  - Anchor 101 AD: 16392
  - Anchor 102 AD: 16380
  - Left Wall <--- 0.6 m ---> Left Anchor Line <--- 0.5 m ---> Central Anchor Line <--- 0.5 m ---> Right Anchor Line <--- 0.4 m ---> Right Wall
  - Anchor baseline <--- 1.31 m ---> Camera
  - Camera <--- 1.2 m ---> Right Wall
  - Warm up is 1-2 min
  - Small Diagonal is sqrt(25+0.25) = 5.024938
  - Large Diagonal is sqrt(25+1) = 5.099
  - Tests are performed at 5m distance because this is the average size of the room

Experiment 5.0: Anchor 101, Tag 1 - test of precision
 
Result:
  - Right Wall Line:
    - Min: 4.97
    - Max: 5.02
    - Avg: 4.99 - 5.00
  - Central Line:
    - Min: 4.98 (once 4.96)
    - Max: 5.03 (once 5.05)
    - Avg: 5.00-5.01
  - Left Wall Line:
    - Min: 4.98
    - Max: 5.02
    - Avg: 5.00-5.01
  
Experiment 5.1: Anchor 101, Tag 1
 
Result:
  - Tag Right Wall Line - Anchor Right Wall Line:
    - Min: 4.97 (sometimes 4.94)
    - Max: 5.03
    - Avg: 4.98-5.00
  - Tag Right Wall Line - Anchor Central Wall Line:
    - Min: 5.02
    - Max: 5.09
    - Avg: 5.04-5.05
  - Tag Right Wall Line - Anchor Left Wall Line:
    - Min: 5.10 (sometimes 5.09)
    - Max: 5.18
    - Avg: 5.14
  - Tag Central Line - Anchor Right Wall Line:
    - Min: 5.07
    - Max: 5.12
    - Avg: 5.09-5.10
  - Tag Central Line - Anchor Central Wall Line:
    - Min: 5.00 (once 4.98)
    - Max: 5.06
    - Avg: 5.02
  - Tag Central Line - Anchor Left Wall Line:
    - Min: 5.03
    - Max: 5.09
    - Avg: 5.06
  - Tag Left Wall Line - Anchor Right Wall Line:
    - Min: 5.14
    - Max: 5.22
    - Avg: 5.17
  - Tag Left Wall Line - Anchor Central Wall Line:
    - Min: 5.03
    - Max: 5.11
    - Avg: 5.05-5.07
  - Tag Left Wall Line - Anchor Left Wall Line:
    - Min: 5.00
    - Max: 5.07
    - Avg: 5.02

Result:
  - 


Experiment 5.2: Anchor 101, Tag 2
 
Result:
  - Tag Right Wall Line - Anchor Right Wall Line:
    - Min: 4.96
    - Max: 4.99 (once 5.01)
    - Avg: 4.96-99
  - Tag Right Wall Line - Anchor Central Wall Line:
    - Min: 5.03 (sometimes 5.02)
    - Max: 5.07
    - Avg: 5.06
  - Tag Right Wall Line - Anchor Left Wall Line:
    - Min: 5.09 (sometimes 5.08)
    - Max: 5.15 (sometimes 5.16)
    - Avg: 5.11-13
  - Tag Central Line - Anchor Right Wall Line:
    - Min: 5.04
    - Max: 5.10
    - Avg: 5.05-07
  - Tag Central Line - Anchor Central Wall Line:
    - Min: 4.97
    - Max: 5.03
    - Avg: 4.99-5.02
  - Tag Central Line - Anchor Left Wall Line:
    - Min: 5.00
    - Max: 5.05
    - Avg: 5.02-03
  - Tag Left Wall Line - Anchor Right Wall Line:
    - Min: 5.12
    - Max: 5.17
    - Avg: 5.13-16
  - Tag Left Wall Line - Anchor Central Wall Line:
    - Min: 5.02
    - Max: 5.08
    - Avg: 5.05
  - Tag Left Wall Line - Anchor Left Wall Line:
    - Min: 4.97
    - Max: 5.02
    - Avg: 5.00-01

Result:
  - 

Experiment 5.3: Anchor 101, Tag 3
 
Result:
  - Tag Right Wall Line - Anchor Right Wall Line:
    - Min: 4.97
    - Max: 5.02
    - Avg: 4.99-5.01
  - Tag Right Wall Line - Anchor Central Wall Line:
    - Min: 5.04
    - Max: 5.08
    - Avg: 5.07
  - Tag Right Wall Line - Anchor Left Wall Line:
    - Min: 5.11
    - Max: 5.17
    - Avg: 5.13-5.15
  - Tag Central Line - Anchor Right Wall Line:
    - Min: 5.05
    - Max: 5.11 (once 5.13)
    - Avg: 5.07-09
  - Tag Central Line - Anchor Central Wall Line:
    - Min: 5.00
    - Max: 5.05
    - Avg: 5.02
  - Tag Central Line - Anchor Left Wall Line:
    - Min: 5.03 (sometimes 5.02)
    - Max: 5.07 (sometimes 5.08)
    - Avg: 5.04-5.05
  - Tag Left Wall Line - Anchor Right Wall Line:
    - Min: 5.15 (sometimes 5.14)
    - Max: 5.21
    - Avg: 5.17
  - Tag Left Wall Line - Anchor Central Wall Line:
    - Min: 5.05
    - Max: 5.12
    - Avg: 5.08
  - Tag Left Wall Line - Anchor Left Wall Line:
    - Min: 5.00 (sometimes 4.98)
    - Max: 5.05 (sometimes 5.06)
    - Avg: 5.02-5.03

Result:
  - 

Experiment 5.4: Anchor 102, Tag 1
 
Result:
  - Tag Right Wall Line - Anchor Right Wall Line:
    - Min: 4.98 (once 4.96)
    - Max: 5.02
    - Avg: 4.99-5.02
  - Tag Right Wall Line - Anchor Central Wall Line:
    - Min: 5.04
    - Max: 5.09
    - Avg: 5.07
  - Tag Right Wall Line - Anchor Left Wall Line:
    - Min: 5.13
    - Max: 5.17
    - Avg: 5.15
  - Tag Central Line - Anchor Right Wall Line:
    - Min: 5.07
    - Max: 5.12 (once 5.14)
    - Avg: 5.09
  - Tag Central Line - Anchor Central Wall Line:
    - Min: 4.99
    - Max: 5.08
    - Avg: 5.02-06
  - Tag Central Line - Anchor Left Wall Line:
    - Min: 5.04
    - Max: 5.12 (sometimes 5.14)
    - Avg: 5.08-09
  - Tag Left Wall Line - Anchor Right Wall Line:
    - Min: 5.14
    - Max: 5.21 (sometimes 5.22)
    - Avg: 5.17
  - Tag Left Wall Line - Anchor Central Wall Line:
    - Min: 5.04
    - Max: 5.12
    - Avg: 5.07-09
  - Tag Left Wall Line - Anchor Left Wall Line:
    - Min: 5.02 (sometimes 5.01)
    - Max: 5.08
    - Avg: 5.03-04

Result:
  - 

Experiment 5.5: Anchor 102, Tag 2
 
Result:
  - Tag Right Wall Line - Anchor Right Wall Line:
    - Min: 4.95
    - Max: 5.02
    - Avg: 4.97-5.01
  - Tag Right Wall Line - Anchor Central Wall Line:
    - Min: 5.03
    - Max: 5.08
    - Avg: 5.05-06
  - Tag Right Wall Line - Anchor Left Wall Line:
    - Min: 5.09
    - Max: 5.13
    - Avg: 5.11-12
  - Tag Central Line - Anchor Right Wall Line:
    - Min: 5.05 (once 5.04)
    - Max: 5.10
    - Avg: 5.06-07
  - Tag Central Line - Anchor Central Wall Line:
    - Min: 4.98
    - Max: 5.04
    - Avg: 5.00-5.01
  - Tag Central Line - Anchor Left Wall Line:
    - Min: 5.02
    - Max: 5.09
    - Avg: 5.02-5.05
  - Tag Left Wall Line - Anchor Right Wall Line:
    - Min: 5.14
    - Max: 5.18
    - Avg: 5.15
  - Tag Left Wall Line - Anchor Central Wall Line:
    - Min: 5.03 (sometimes 5.02)
    - Max: 5.08
    - Avg: 5.06
  - Tag Left Wall Line - Anchor Left Wall Line:
    - Min: 4.98
    - Max: 5.03
    - Avg: 5.00-5.01

Result:
  - 

Experiment 5.6: Anchor 102, Tag 3
 
Result:
  - Tag Right Wall Line - Anchor Right Wall Line:
    - Min: 4.97
    - Max: 5.01
    - Avg: 4.97-5.00
  - Tag Right Wall Line - Anchor Central Wall Line:
    - Min: 5.03
    - Max: 5.09
    - Avg: 5.06
  - Tag Right Wall Line - Anchor Left Wall Line:
    - Min: 5.10
    - Max: 5.17
    - Avg: 5.13-14
  - Tag Central Line - Anchor Right Wall Line:
    - Min: 5.07
    - Max: 5.12
    - Avg: 5.08-09
  - Tag Central Line - Anchor Central Wall Line:
    - Min: 4.99
    - Max: 5.05
    - Avg: 5.02-04
  - Tag Central Line - Anchor Left Wall Line:
    - Min: 5.03
    - Max: 5.07 (once 5.10)
    - Avg: 5.05
  - Tag Left Wall Line - Anchor Right Wall Line:
    - Min: 5.17
    - Max: 5.21
    - Avg: 5.19-20
  - Tag Left Wall Line - Anchor Central Wall Line:
    - Min: 5.06
    - Max: 5.12
    - Avg: 5.08
  - Tag Left Wall Line - Anchor Left Wall Line:
    - Min: 5.00 (once 4.99)
    - Max: 5.07
    - Avg: 5.02-5.05

Result:
  - 

Analysis of diagonal test:

  - Observations from today's diagonal-parallel test and the december 9th experiment shows that reveal the influence of the USB connection on measurements. Notably, when anchor and tag see each other from USB connection side, values are registered lower in comparison to scenarios observed from the opposite side. In this context, USB connection side is for example when tag is plased at the window line and anchor is placed at the central line (as per experiment on December 9th), and opposite side is for example when tag is placed at central line and anchor is at the window line.   
    - At today's experiment and at experiment on December 9th I have used different USB cables
    - The reason can be is that usb cable might enchance UWB signal. Or the reason can be inside the UWB ESP32 itself.

  - Even the smallest measured value exceeds the true distance. Therefore, there is need to rotate anchors to have direct line-of-sight with tags.
    - TODO: rotate anchors to have direct line-of-sight with tags.


# 29 December, 2023

Experiment 6: Rotated diagonal test. Anchor 101, Tag 1

Objective:
  - Add rotation along azimuth for anchor and tag. Check results.
 
Result:
  - Tag Right Wall Line - Anchor Right Wall Line:
    - Min: 4.96
    - Max: 5.06
    - Avg: 5.00-5.02
  - Tag Right Wall Line - Anchor Central Wall Line:
    - Min: 5.03 (sometimes 5.02)
    - Max: 5.09 (sometimes 5.12)
    - Avg: 5.05
  - Tag Right Wall Line - Anchor Left Wall Line:
    - Min: 5.08
    - Max: 5.17
    - Avg: 5.14
  - Tag Central Line - Anchor Right Wall Line:
    - Min: 5.06 (once 5.03)
    - Max: 5.11
    - Avg: 5.08
  - Tag Central Line - Anchor Central Wall Line:
    - Min: 5.02 (once 5.00)
    - Max: 5.05 (once 5.06)
    - Avg: 5.02-5.04
  - Tag Central Line - Anchor Left Wall Line:
    - Min: 5.00
    - Max: 5.09
    - Avg: 5.02-5.07 (jumping)
  - Tag Left Wall Line - Anchor Right Wall Line:
    - Min: 5.10
    - Max: 5.16
    - Avg: 5.11-5.13
  - Tag Left Wall Line - Anchor Central Wall Line:
    - Min: 5.03
    - Max: 5.10 (once 5.12)
    - Avg: 5.05
  - Tag Left Wall Line - Anchor Left Wall Line:
    - Min: 4.98
    - Max: 5.06
    - Avg: 5.02

Result:
  - 

-
Overall result shows that rotation improves measurements by 1-2 cm, but still values are high comparing to ground truth distances.

Experiment 7: 2 Anchors and 3 Tags at once.

Antenna delay setup:
  - Tag 1 and Tag 3: 16521
  - Tag 2: 16536
  - Anchor 101: 16392
  - Anchor 102: 16380

Result:
  Experiment 7.1:
  - Tag 1 Left Wall Line - Tag 2 Central Line - Tag 3 Right Wall Line. 
        Anchor 101 Left Wall Line - Anchor 102 Right Wall Line.
    
    Tag 1: 
        Anchor 101:
            Median: 5.048338 
            Mean: 5.045450538461539
            Min: 4.987345
            Max: 5.114023
      
        Anchor 102:
            Median: 5.198474 
            Mean: 5.199977987179487
            Min: 5.156249
            Max: 5.254776


    Tag 2: 
        Anchor 101:
            Median: 5.00142 
            Mean: 5.002803576923077
            Min: 4.973269
            Max: 5.05303
      
        Anchor 102:
            Median: 5.0413 
            Mean: 5.041540820512821
            Min: 4.987345
            Max: 5.085872


    Tag 3: 
        Anchor 101:
            Median: 5.106985 
            Mean: 5.109992410256411
            Min: 5.067105
            Max: 5.146865
      
        Anchor 102:
            Median: 4.987345 
            Mean: 4.987585461538463
            Min: 4.949811
            Max: 5.024879 
        

  Experiment 7.2:
  - Tag 1 Left Wall Line - Tag 3 Central Line - Tag 2 Right Wall Line. 
        Anchor 101 Left Wall Line - Anchor 102 Right Wall Line.

    Tag 1: 
        Anchor 101:
            Median: 5.05303 
            Mean: 5.053517025974026
            Min: 5.010804
            Max: 5.090564
      
        Anchor 102:
            Median: 5.207858 
            Mean: 5.209015493506493
            Min: 5.16094
            Max: 5.245392


    Tag 2: 
        Anchor 101:
            Median: 5.085872 
            Mean: 5.087415157894737
            Min: 5.057721
            Max: 5.142173
      
        Anchor 102:
            Median: 4.968578 
            Mean: 4.970923684210526
            Min: 4.935736
            Max: 5.010804


    Tag 3: 
        Anchor 101:
            Median: 5.05303 
            Mean: 5.052782592105262
            Min: 5.015495
            Max: 5.104639
      
        Anchor 102:
            Median: 5.095255 
            Mean: 5.095502328947369
            Min: 5.057721
            Max: 5.123406
  
  Intermediate observation: 

    - Tag 1 start showing higher values for both Anchors in comparison to Experiment 7.1.

  Experiment 7.3:
  - Tag 2 Left Wall Line - Tag 1 Central Line - Tag 3 Right Wall Line. 
        Anchor 101 Left Wall Line - Anchor 102 Right Wall Line.
    
    Tag 1: 
        Anchor 101:
            Median: 5.095255 
            Mean: 5.096390483870967
            Min: 5.057721
            Max: 5.128098
      
        Anchor 102:
            Median: 5.128098 
            Mean: 5.127795145161289
            Min: 5.090564
            Max: 5.165632


    Tag 2: 
        Anchor 101:
            Median: 4.996728 
            Mean: 4.994760870967743
            Min: 4.968578
            Max: 5.034263
      
        Anchor 102:
            Median: 5.156249 
            Mean: 5.156778096774192
            Min: 5.123406
            Max: 5.193782


    Tag 3: 
        Anchor 101:
            Median: 5.13279 
            Mean: 5.131957225806452
            Min: 5.085872
            Max: 5.165632
      
        Anchor 102:
            Median: 5.010804 
            Mean: 5.011636080645161
            Min: 4.992037
            Max: 5.034263
  
  Experiment 7.4:
  - Tag 2 Left Wall Line - Tag 3 Central Line - Tag 1 Right Wall Line. 
        Anchor 101 Left Wall Line - Anchor 102 Right Wall Line.
    
 Tag 1: 
        Anchor 101:
            Median: 5.170324 
            Mean: 5.171475309090909
            Min: 5.128098
            Max: 5.203166
      
        Anchor 102:
            Median: 5.029571 
            Mean: 5.0302532181818185
            Min: 4.987345
            Max: 5.062413


    Tag 2: 
        Anchor 101:
            Median: 5.00142 
            Mean: 5.001842837837838
            Min: 4.977962
            Max: 5.029571
      
        Anchor 102:
            Median: 5.16094 
            Mean: 5.1610247477477476
            Min: 5.123406
            Max: 5.207858


    Tag 3: 
        Anchor 101:
            Median: 5.067105 
            Mean: 5.064991369369369
            Min: 5.024879
            Max: 5.095255
      
        Anchor 102:
            Median: 5.104639 
            Mean: 5.105188468468468
            Min: 5.067105
            Max: 5.146865
  
  Experiment 7.5:
  - Tag 3 Left Wall Line - Tag 1 Central Line - Tag 2 Right Wall Line. 
        Anchor 101 Left Wall Line - Anchor 102 Right Wall Line.
    
    Tag 1: 
        Anchor 101:
            Median: 5.104639 
            Mean: 5.105386956521739
            Min: 5.062413
            Max: 5.16094
      
        Anchor 102:
            Median: 5.128098 
            Mean: 5.125921963768116
            Min: 5.085872
            Max: 5.16094


    Tag 2: 
        Anchor 101:
            Median: 5.109331 
            Mean: 5.108956760869565
            Min: 5.071796
            Max: 5.156249
      
        Anchor 102:
            Median: 4.996728 
            Mean: 4.995572449275363
            Min: 4.959194
            Max: 5.024879


    Tag 3: 
        Anchor 101:
            Median: 5.029571 
            Mean: 5.031916644927536
            Min: 4.992037
            Max: 5.071796
      
        Anchor 102:
            Median: 5.203166 
            Mean: 5.205035913043478
            Min: 5.165632
            Max: 5.250083
  
  Experiment 7.6:
  - Tag 3 Left Wall Line - Tag 2 Central Line - Tag 1 Right Wall Line. 
        Anchor 101 Left Wall Line - Anchor 102 Right Wall Line.
    
    Tag 1: 
        Anchor 101:
            Median: 5.179707 
            Mean: 5.177055260869564
            Min: 5.123406
            Max: 5.217241
      
        Anchor 102:
            Median: 5.034263 
            Mean: 5.036353489130435
            Min: 4.992037
            Max: 5.067105


    Tag 2: 
        Anchor 101:
            Median: 5.038954 
            Mean: 5.038648358695652
            Min: 4.996728
            Max: 5.08118
      
        Anchor 102:
            Median: 5.085872 
            Mean: 5.085514826086956
            Min: 5.057721
            Max: 5.118714


    Tag 3: 
        Anchor 101:
            Median: 5.034263 
            Mean: 5.031866797872341
            Min: 4.992037
            Max: 5.071796
      
        Anchor 102:
            Median: 5.207858 
            Mean: 5.209504861702128
            Min: 5.175015
            Max: 5.254776

  Experiment 8.1:
  - Tag 1 Left Wall Line - Tag 2 Central Line - Tag 3 Right Wall Line. 
        Anchor 102 Right Wall Line - Anchor 101 Left Wall Line.
    
    Tag 1: 
        Anchor 101:
            Median: 5.226625 
            Mean: 5.228520821917808
            Min: 5.184399
            Max: 5.273542
      
        Anchor 102:
            Median: 5.057721 
            Mean: 5.059938589041096
            Min: 5.010804
            Max: 5.099947


    Tag 2: 
        Anchor 101:
            Median: 5.076488 
            Mean: 5.0758134109589035
            Min: 5.029571
            Max: 5.114023
      
        Anchor 102:
            Median: 5.05303 
            Mean: 5.050073109589041
            Min: 5.00142
            Max: 5.095255


    Tag 3: 
        Anchor 101:
            Median: 5.010804 
            Mean: 5.0131657655172415
            Min: 4.973269
            Max: 5.057721
      
        Anchor 102:
            Median: 5.137481 
            Mean: 5.136769531034483
            Min: 5.085872
            Max: 5.175015
  
  Experiment 8.2:
  - Tag 1 Left Wall Line - Tag 3 Central Line - Tag 2 Right Wall Line. 
        Anchor 102 Right Wall Line - Anchor 101 Left Wall Line.
    
    Tag 1: 
        Anchor 101:
            Median: 5.226625 
            Mean: 5.2271703875969
            Min: 5.193782
            Max: 5.278234
      
        Anchor 102:
            Median: 5.062413 
            Mean: 5.060885472868216
            Min: 4.996728
            Max: 5.104639


    Tag 2: 
        Anchor 101:
            Median: 4.982653 
            Mean: 4.98432623255814
            Min: 4.954503
            Max: 5.024879
      
        Anchor 102:
            Median: 5.13279 
            Mean: 5.131007426356589
            Min: 5.08118
            Max: 5.184399


    Tag 3: 
        Anchor 101:
            Median: 5.109331 
            Mean: 5.109803573643411
            Min: 5.062413
            Max: 5.156249
      
        Anchor 102:
            Median: 5.057721 
            Mean: 5.058448689922481
            Min: 5.020187
            Max: 5.095255
  
  Experiment 8.3:
  - Tag 2 Left Wall Line - Tag 1 Central Line - Tag 3 Right Wall Line. 
        Anchor 102 Right Wall Line - Anchor 101 Left Wall Line.
    
    Tag 1: 
        Anchor 101:
            Median: 5.137481 
            Mean: 5.135514948529412
            Min: 5.090564
            Max: 5.231317
      
        Anchor 102:
            Median: 5.104639 
            Mean: 5.104086963235295
            Min: 5.057721
            Max: 5.142173


    Tag 2: 
        Anchor 101:
            Median: 5.175015 
            Mean: 5.175610582089552
            Min: 5.142173
            Max: 5.21255
      
        Anchor 102:
            Median: 5.015495 
            Mean: 5.017000985074627
            Min: 4.982653
            Max: 5.057721


    Tag 3: 
        Anchor 101:
            Median: 5.006112 
            Mean: 5.008352708955225
            Min: 4.973269
            Max: 5.048338
      
        Anchor 102:
            Median: 5.128098 
            Mean: 5.129638410447761
            Min: 5.076488
            Max: 5.184399
  
  Experiment 8.4:
  - Tag 2 Left Wall Line - Tag 3 Central Line - Tag 1 Right Wall Line. 
        Anchor 102 Right Wall Line - Anchor 101 Left Wall Line.
    
    Tag 1: 
        Anchor 101:
            Median: 5.043646 
            Mean: 5.044502782608695
            Min: 4.992037
            Max: 5.08118
      
        Anchor 102:
            Median: 5.170324 
            Mean: 5.171466026086956
            Min: 5.137481
            Max: 5.221933


    Tag 2: 
        Anchor 101:
            Median: 5.170324 
            Mean: 5.172426905172414
            Min: 5.123406
            Max: 5.207858
      
        Anchor 102:
            Median: 5.015495 
            Mean: 5.016951482758621
            Min: 4.992037
            Max: 5.048338


    Tag 3: 
        Anchor 101:
            Median: 5.106985 
            Mean: 5.10595602631579
            Min: 5.071796
            Max: 5.146865
      
        Anchor 102:
            Median: 5.057721 
            Mean: 5.058626684210527
            Min: 5.010804
            Max: 5.099947
  
  Experiment 8.5:
  - Tag 3 Left Wall Line - Tag 1 Central Line - Tag 2 Right Wall Line. 
        Anchor 102 Right Wall Line - Anchor 101 Left Wall Line.
    
    Tag 1: 
        Anchor 101:
            Median: 5.13279 
            Mean: 5.133247284552845
            Min: 5.08118
            Max: 5.184399
      
        Anchor 102:
            Median: 5.095255 
            Mean: 5.093729569105691
            Min: 5.05303
            Max: 5.128098


    Tag 2: 
        Anchor 101:
            Median: 4.977962 
            Mean: 4.978533520325203
            Min: 4.935736
            Max: 5.015495
      
        Anchor 102:
            Median: 5.114023 
            Mean: 5.113488536585366
            Min: 5.071796
            Max: 5.16094


    Tag 3: 
        Anchor 101:
            Median: 5.217241 
            Mean: 5.2148000487804875
            Min: 5.165632
            Max: 5.259467
      
        Anchor 102:
            Median: 5.038954 
            Mean: 5.036818235772357
            Min: 4.996728
            Max: 5.067105
  
  Experiment 8.6:
  - Tag 3 Left Wall Line - Tag 2 Central Line - Tag 1 Right Wall Line. 
        Anchor 102 Right Wall Line - Anchor 101 Left Wall Line.
    
    Tag 1: 
        Anchor 101:
            Median: 5.048338 
            Mean: 5.050342846153845
            Min: 5.010804
            Max: 5.099947
      
        Anchor 102:
            Median: 5.165632 
            Mean: 5.163506547008547
            Min: 5.13279
            Max: 5.207858


    Tag 2: 
        Anchor 101:
            Median: 5.085872 
            Mean: 5.0840921637931045
            Min: 5.034263
            Max: 5.118714
      
        Anchor 102:
            Median: 5.048338 
            Mean: 5.047286163793103
            Min: 5.00142
            Max: 5.090564


    Tag 3: 
        Anchor 101:
            Median: 5.21255 
            Mean: 5.214955589743589
            Min: 5.175015
            Max: 5.254776
      
        Anchor 102:
            Median: 5.038954 
            Mean: 5.037991880341881
            Min: 5.00142
            Max: 5.08118
  

  Overall results of Experiments 7 and 8:
    - Tags have good diagonal measurements from cable connection side. Otherwise diagonal measurements are higher than ground truth.
      - Note: Tag 2 shows a bit lower values on average compare to Tag 1 and Tag 3 in direct test.

Experiment 9: Movements

Experiment 9.1: Experiment 9.1 - From Anchors, by 1 m together


Experiment 9.2: Experiment 9.2 - To Anchors, by 1 m each

Results:

  - When Two Tags are on the same line towards Anchors, and a person is between them, then tag does not see anchor.
    - Everything is stopped
    - TODO: need to investigate more.
    - Maybe when more than 3 UWB chips are placed at the same line.
  
  - But when tag is overlapping with another tag only or a person only, it works OK.


  # 6 January, 2024

Experiments: Distances

Setup:
  Camera distance:
  - 155 cm from the anchor baseline
  - 71 cm from the right wall

  Antenna delay setup:
  - Tag 1: 16528 - changed
  - Anchor 101: 16392
  - Anchor 102: 16380
  - Tag and Anchor are placed at the central line


Objective: 
  - Trending in distance measurements. How values is changed along with distance.

Experiment 1: Anchor 101, Tag 1. Dynamic test. Focus on precision using electric laser.

Result: //TODO
  - 1m
    - Min: 
    - Max: 
    - Avg: 
  - 2m:
    - Min: 
    - Max: 
    - Avg: 
  - 3m:
    - Min: 
    - Max: 
    - Avg: 
  - 4m:
    - Min: 
    - Max: 
    - Avg: 
  - 5m:
    - Min: 
    - Max: 
    - Avg: 
  - 6m:
    - Min: 
    - Max: 
    - Avg: 
  - 7m:
    - Min: 
    - Max: 
    - Avg: 
  - 8m:
    - Min: 
    - Max: 
    - Avg: 
  - 9m:
    - Min: 
    - Max: 
    - Avg: 
  - 10m:
    - Min: 
    - Max: 
    - Avg: 

Experiment 2: Anchor 101 and 102, Tag 1. Dynamic test. Focus on precision using electric laser. //TODO


# 7-8 January, 2024

The entire system, comprising 2 anchors and 3 tags demonstrates good functionality, but not so good performance (can be improved).

In order to improve performance, debug part (Serial.print) can be removed in the next experiments.

During the upload process, the board type was changed from ESP Dev to ESP Wrover.

Result: This modification resulted in sustainable speed improvement, with the speed boost of at least 2 times.

Time measurement showed that new takes around 145 ms from sending request till receiving measurement.

However, while uploading code to Tag 1, the board was malfunctioned, such that it is no more recognizable through the USB port. 

Repair of USB connection pins is required.

ESP Dev mode has same speed. Maybe faster?

# 11 January, 2024

Prozkoumat frekvenci, jaky ma vliv

# 14 January, 2024

The primary aim at this stage is to consolidate the individual components of the project into one unified system. Components are: 1. UWB Server, 2. Stereo vision distance estimator, 3. Data Analysis.

Development in python: Initial development of the GUI was done in the pythonm due to its ease of use and extensive library support.

Issues with python: Duting the development phase I have encountered challenges with Stereo Vision implementation and decided to switch to C++.

GUI in C++: for the development of the GUI in c++ I have decided to go with Qt framework.

Current status: 
  - As an initial step I have implemented a simple video player using opencv library, which shows synchronized recorded UWB data and Video Frames. 
  - Application also supports trackbar, so the user can easily choose the part of the video he wants to work with. There is also possibility to stop and play video.
  - However, opencv is not optimized for good user experience: video is playing very slowly, due to update of the trackbar.

Next steps: Try Qt library and make video player. 

Observation of an issue: After creating Qt Video Player, I faced an issue with opening .avi and .mp4 video files. I got issues: [h264_cuvid @ 0x5555568aed80] Codec h264_cuvid is not supported. Player error: QMediaPlayer::FormatError and [hevc_cuvid @ 0x5590f55eecc0] Codec hevc_cuvid is not supported, respectively. 

Solution: Installing libnvidia-decode-535, x265 and libx265-199 libraries helped.

# 16 January, 2024

Simple video player in Qt framework is implemented. However, standard Qt player does not allow to access and work with each frame. Integration of the OpenCV library is required.

After integrating opnecv library into Qt player, I have realized that performance issues are still present - same as when I have implemented video player with only OpenCV library. Therefore, some optimizations might be needed.

The first optimization is to update QImage and not recreate it every time. As another improvement, it would be better to have a separate thread for frame processing using OpenCV library.  

# 20 January, 2024

Multithreaded Qt application is implemented. As a first step, separate thread for GUI and for OpenCV is created. OpenCV reads file and triggers GUI to update frames. However, this setup has disadvantage that OpenCV reads frames without delay, so video plays very fast. 

In order to solve this, I have added a timer, which ticks every 40 ms (for 25 fps video), and signals OpenCV library to prepare frame. This approach has disadvantage when OpenCV might require more time to process frame, and therefor GUI will wait for frame to be prepared by OpenCV.

This can be solved in two ways: 1. In GUI use a pool of frames, which is filled by OpenCV. Then just read frames from pool every 40ms. 2. Use locked shared variable "frame". When reading frame by GUI, lock it, read, and signal OpenCV to prepare next frame.

The downside of the 2. approach is that it speeds up the workflow just for a small amount of time. Although it is simplier that 1. approach, it might not help in case of complex, computationally expensive operations on OpenCV side. 

Therefore, based on the previous experience on working with threads I decided to go with pool. 

# 26 January, 2024

Issue:

Multithreaded pool is implemented correctly. However, I faced several issues when trying to implement slider to be able to go the the desired position of the video.

What was tried:
1. camera.set(cv::CAP_PROP_POS_FRAMES, position)
2. camera.set(cv::CAP_PROP_POS_MSEC, position)

1. option crashes when position is set to non-existing position in camera (VideoCapture). Since frame pool is implemented, it is possible to see position of each frame, using camera.get(cv::CAP_PROP_POS_FRAMES).

2. option works bad, since get method on Linux return random value (random time) due to ffmpeg: https://github.com/opencv/opencv/issues/23472.

Also Multithreading works bad with OpenCV


Solution: 

Handling multithreading in coorect way helped. VideoProcessor::processvideo function was called directly in VideoProcessor thread after seeking the right frame. This led to thread conflicts. Introducing IndoorPositioningSystem::requestProvessVideo signal slot to run VideoProcessor::processVideo helped. This signal is emitted when frameTimer has started and videoProcessor thread become free to perform further work.


Next step: Add synchronization between video frames and uwb data.

Solution: 

Since player is showing frames in 25 fps, I decided to run another thread to find uwb data before they will be displayed. Maybe this does not make sence for small data, but it make sence for GBs of data. As a further improvement I decided to implement logarithmic search because data are sorted by timestamps.




# 3 February, 2024

Experiment 1: 2 Anchors standing on rours and 3 Tags with People walking.
From 3m to 15 m

Antenna delay setup:
  - Tag 1: 16528
  - Tag 2: 16536
  - Tag 3: 16521
  - Anchor 101: 16392
  - Anchor 102: 16380


Anchor baseline: 2,5 m
Camera to anchor baseline: 2,14 m
camera height: height of roura

Petr drzel tag primo u bricha, ja jsem drzel trochu dal

Experiment 2: Stejny setup, vsichni drzime tag trochu dal od bricha. Jdeme po 1 metru kazdy spolehlive, spolu. Stojime po 1 vterine.

Experiment 3: Chodime po 1 metru, ale kazdy chodi nahodne. Na konci ja jdu bez zastavek

Co pridat: Vyber usek a automatickou analyzu pro usek. Bud pridat zoom na video, anebo export obrazku, aby se to dalo exportovat

//TODO Issue: video are recorded very fast, why? there are missing frames. This happened (maybe) after I have installed Qt library. Before video was recorded ok (considering same settings for opencv).

# 4 February, 2024

Antenna delay setup:
  - Tag 1: 16528
  - Tag 2: 16536
  - Tag 3: 16521
  - Anchor 101: 16392
  - Anchor 102: 16380


Anchor baseline: 2,5 m
Camera to anchor baseline: 2,08 m
Camera to anchors width (perpendicular): 1,25m 
camera to anchors actual: 2.08^2 + 1,25^2
camera height: 63,5 m

Experiment 1: 2 Anchors standing on rours and 3 Tags on rours

This experiment is axiliary. from 5 to 3 meters.

Experiment 2: 2 Anchors standing on rours and 3 Tags on rours.
From 3m to 15 m

This experiments aims to proof precision of UWB chips.



Epxeriment 3:
Changed anchor ID: 100-103. This affects timer delay -> communication speed must be larger. - DOES NOT WORK!!! -> returned to rage 101-104. Formula (2*(myID-100)+1)*DEFAULT_REPLY_DELAY_TIME does not work
Lowered reset timeout from 500 ms to 200 ms

Setup:

Anchor baseline: 2,5 m
Camera to anchor baseline: 3m
Camera to anchors width (perpendicular): 1,25m 
camera height: 1,062 m 


# 8 February, 2024

Meeting with teacher.

- Depth test is now sufficient. We need to perform width/angle test.
- First of all we need to develop methods within ideal environment. Once everything works, we can test interference.
- Use cases: 1. Only video. 2. Video + UWB
- Where to put camera when recording video + collecting UWB data? Anchor baseline vs. 3 m behind baseline, such that everything is visible (inlcuding anchors)
- On sunday we can perform angle test, on saturday I can perform depth test and collect data from each 0.5 m.
- As next step we can compute position of person and then try to train neural network.
- Need to add analysis and range selection to GUI.

# 10 February, 2024

looks like I forgot to set correct antenna delay for Anchor 102

After changing reset timeout on the anchor side to 200ms. The issue is observed with Tag. Tag starts smelling. // TODO - is that true? Is it because of timeout on the anchor side? Why tag was burned.

Changing hard reset on anchor from 200 back to 500 ms resolved issue.

Anchor baseline: 2,5 m
Camera to anchor baseline: 2,08 m
Camera to anchors width (perpendicular): 1,25m 
camera to anchors actual: 2.08^2 + 1,25^2
camera height: 63,5 cm

Check experiments folder for experiment data.

# 11 February, 2024

I have tried to run a system with 3 anchors. Anchors were placed on lamps cca 2,5 m high. It works OK even when only 2 anchors were placed on lamps and 1 anchor was placed on the roura.

Then I tried a system with 4 anchors, but unfortunatelly 4th anchor gives strange numbers. Is it due to replyDelay (too long?).

According to this, I have tried to change replyDelay. Working state is when each anchor has a different replyDelay. I have tried to only delay anchor's response to tag and then use same replyDelay value for all anchors, but it DOES NOT WORK!! I also tried to use different replyDelay, but lower value for each anchor. Also does not help. Only works the way which is implemented in main library.

This is not true. Not only the way from library works. See below (16 February)

So I decided t stick only with 3 anchors, which is useful for position estimation.

# 16 February, 2024

Analysis of the past experiments: 

1. After lowering the reset timeout on the anchor side to 200 ms, tags were burned twice. It happened when tag got the value from one anchor and lost connection with another anchor. 
  - In the first experiment in Rotunda tag lost the connection with anchor and stucked in infinite loop (maybe). Tag was placed at visible distance from anchors.
  - In the second experiment near S8, I was standing at 1 meter from the Anchor baseline. Distance between anchors was 2.5 m. One of the anchors lost the connection with tag, and tag were burned. 

  After resetting the reset timeout on the anchor back to the previous 500 ms solved the issue. I have tried same scenario as in second experiment. It was ok. Tag lost the connection, but was not burned. 
  
  Maybe this was happened occasionally, because Timout was changed on the Anchor side, but Tag was burned.

  2. Different anchors use different replyDelay when sending poll and range signals. This delays must be different. Same value does not work. Even more, I think when anchors send signal at the same time, signal from one anchor gets blocked by another signal and gets lost. Tag cannot see the signal sent by anchor. This happens even with different replyDelay values, but when signals get collided.

  I have observed that difference between replyDelay on anchors must be at least 12 ms, otherwise communication does not work good. Either gets stucked or distances arriving randomly (to server) with signifficant delay. Another thing is that maximum difference must be 50 ms. Otherwise one of the anchors shows strange distances like 4500.00 or even negative values like -5000.00.

  replyDelay does have to be like in library. Difference between each pair of anchors (replyDealys) must be at least 12-14ms and at most 50 ms. Difference between two following replyDelays must be 12-14 ms. - Not True

  I have managed to run all 4 anchors with the following setup:

  Anchor 101 - 17 ms
  Anchor 102 - 20 ms
  Anchor 103 - 23 ms
  Anchor 104 - 26 ms

# 17 February, 2024 - test in school with Stepan and Peter

Experiments at school - looks like I forgot to set correct antenna delay for Anchor 102

Anchor baseline: 2,5 m
Camera to anchor baseline: 2,08 m
Camera to anchors width (perpendicular): 1,25m 
camera to anchors actual: 2.08^2 + 1,25^2
camera height: 63,5 cm

Observation: 
  During experimets strange phenomenon was observed. Tag 3 with mac address 70:B8:F6:D8:F6:60 (which was burned) does not receive signal near server (HW server which is placed in the wall). Another Tag 1 with mac address "70:B8:F6:D8:F6:48" (which was also burned), however, works OK. Maybe this is specific to ESP32 board, or it is because the board was burned. 
  
  This also happened when anchor line was far away. When anchors are placed closer (Experiment 7), signal is strong enough to be received and handled (on both tags).

For best anchor - tag communication, anchor baselines must be placed close to each other as shown in Experiment 12 and 13. At such positions, the problem with angle of vision of antenna is resolved. Experiment 13 shown that a bit larger distance between anchor baselines also works and sufficient. Such placement of anchors requires less anchors to be used to cover the whole space.

# 18 February, 2024 - stereo vision test in School

Environment settings:

- Anchor baseline: 2,5 m
- camera baseline: 19,8 cm
- Anchor and camera baselines are aligned - same line
- All video and parameters are saved
- camera height: 63.5 cm
- Baseline is origin/start of coordiate system, next visible (in video) papers are at 2 m from baseline.


# 19 February, 2024 - Monday, Zoom

1. The goal of diploma is to train the model to estimate the position inside the room.
2. Then position estimation can be tested with Stereo Vision method.


# 24 February, 2024 - Experiment for neural network

Setup (New tags):

Anchor baseline: 2,5 m
Camera to anchor baseline: 2,08 m
Camera to anchors width (perpendicular): 1,25m 
camera to anchors actual: 2.08^2 + 1,25^2
camera height: 1,06 m

Antenna delay setup (New tags):
  - Tag 1: 16526
  - Tag 2: 16532
  - Tag 3: 16524
  - Anchor 101: 16392 Reply delay: 17000
  - Anchor 102: 16384 Reply delay: 20000
  - Anchor 103: 16372 Reply delay: 23000
  - Anchor 104: 16392 Reply delay: 26000


Experiment 1: 

All of us are walking same direction same tempo. Going both directions. Walking along papers.

Experiment 2: 

Same as Experiment 1, but walking near papers

Experiment 3:

Each of us has own Area: Tag 1 - line along left papers and line near to it from internal side; Tag 2 - central line, lines from both sides; Tag 3 - right line and line from internal part near to right line 

Experiment 4: 

Recorded for model train - Tag 3 only

Experiment 5:

Cross walking. Everyone goes as he likes. Crossing lines.

Experiment 6:

Cross walking. Similar to 5, but going outside the area. Opening doors + sitting.



//TODO:

1. Complete GUI for analysis
2. Compute poistion using UWB data only
3. Tain model for camera using corrected UWB measurements
4. Complete implementation of other methods

Methods:

1. (Camera only) Focal length
2. (Camera only) HEometrical method 2. Using floor tiles
3. (UWB + Camera) Calculate position using UWB measurements as distance from camera 
  - given camera and anchor permanent, it is possible to compute distance from camera

4. (UWB + Camera) Train model


#  2 March, 2023. Experiment in school

Experiment 5: Free walking + chair

Chair is placed 4 meters from anchor baseline

Experiment 8: Anchors are at the height of Peter

Setup:
  - Anchor baseline (same for 101-102 and 103-104 pairs): 
  - Anchor height: 180 cm (Peter's height)
  - Camera height: 110 cm (roura)
  - Camera is aligned with baseline of anchors 101 and 102:
  - Tag holding height is same as anchor height: Height of Peter.

  Experiment 9: Anchors and Tags are at the height 180 cm

  Setup: 
  - Anchor baseline: 125 cm
  - Anchor 101 to Camera: 65,3 cm
  - Anchor 102 to Camera: 60,6 cm

  Position of anchors: 101 102
                       
                       104 103

  Anchor height: 180 cm
  Camera height: 180 cm
  Right wall to Anchor 102: 70,5 cm
  Left wall to Anchor 101: 228,9 cm
  Height of Mohamad: 180 cm
  Height of other guy: 170 cm
  My height: 176 cm
  Anchors 101 and 102 are aligned with camera





Where to use project: 

1. Use it isnide house (smart home) or other buildings to localize perople and make operating thing only in the area where person is located. For example to turn on the light, hitting etc. This will save money, a LOT!!

2. To localize the robot, for example at wharehouses or restaurents, which will bring staff to customer etc.






TODO:

 1. Calculate the distance from wall to first anchor (for anchor/tag visualization)
 2. Add offset to x-coordinate (done) and y-coordinate(TODO) in formula calculation. Conversion to world/camera coordinate system
 2. Record new data with 3/6 anchors

 # 13 March, 2024

Trying to integrate object detection. 

Best perfomrance, but less accurate if YOLOv4-tiny. Better to use normal YOLOv4, but this has large impact on performance. However usage of CUDA helped to improve performance. 

To use more recent YOLO, like v8, there is a need to convert .pt to .onnx first. This can be done using python script. Necessary libraries: torch, ultralytics
There is also another way how to make yolov8.pt to be used in c++: https://pytorch.org/tutorials/advanced/cpp_export.html

