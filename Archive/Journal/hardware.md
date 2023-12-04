# Journal

This is the journal to record updates and ideas, what didn't work, what was changed etc. Starting date is Oct 31, 2022 (middle point of the implementation, not beginning)

It doesn't record all changes, but the most important.

# What was done before

- trial run of YOLOv4 to detect people in the input video
- integration of focal length into YOLOv4 to compute the distance between people using 3D Euclidian distance
- trial run of ESP32 UWB chips

  - uploading existing code from official **[arduino-dw1000](https://github.com/thotro/arduino-dw1000)** library (examples/DW1000Ranging_ANCHOR/DW1000Ranging_ANCHOR.ino and examples/DW1000Ranging_TAG/DW1000Ranging_TAG.ino)
    - it works perfectly just for one tag and one anchor
    - To support more anchors, we have used already implemented linked list of anchors:
      - integration of [link.cpp](https://github.com/playfultechnology/UWBRTLPS/blob/main/ESP32/UWB_Positioning/link.cpp) and [link.h](https://github.com/playfultechnology/UWBRTLPS/blob/main/ESP32/UWB_Positioning/link.h) to detect multiple anchors from a tag

- The main idea of the project is to implement synchronized system which uses several methods for distance estimation and then estimates the final distance based on those estimated distances. For example it can use average.

  - this requires synchronization between each method

- First thing which comes to mind is to use some machine which will collect data from every method and then make final decision about the distance estimation. This machine can be a server.

- There are two ways to store the distances measured by UWB chips: in anchors and in tags. The former is not good desicion since two anchors must be synchronized as well to send the sync data to the server related to a particular tag. Therefore we decided to implement the letter method, because each tag collects data related only to it.

- As the first step we desided to implement decentralized system. We have implemented the server which will get data from tags, but let tags operate on their own. So, when they will collect the desired number of estimated distances from anchors, they will send the distance to the server.

- Unfortunatelly, during experiments we have realized that tags overloaded the server, so the servee showed distances with significant delay. And then after a while it showed all the received distances at once. This was a complete mess.

- As an improvement of the communication between server and tags we have added acknoledgements as a step of the communication between server and tags. This helped to overcome the problem.

- Unfortunatelly, after some tests, we have realized that the official implementation of the TWR protocol from the DW1000 library it happens that if we have a set up where two tags and one anchor are located at the same line, the tag in the middle overwrites data that are specific for the farther tag, therefore the farthest tag receives wrong estimated distances. Do not know exactly why, I think the library uses myDistantDevice->getDistance() in tags which reads distance from the anchor (distance is computed in the anchor). We desided not to overwrite the whole library and go with another existing solution.
  - new try with ESP32 UWB chips
    - source: [anchor](https://github.com/gsongsong/dw1000-positioning/blob/ee5e2c9e57f42ad23014cc470b6478d3fb0dbe57/anchorArduino/anchorArduino.ino) and [tag](https://github.com/gsongsong/dw1000-positioning/tree/ee5e2c9e57f42ad23014cc470b6478d3fb0dbe57/tagArduino) (changed) + added sender to server

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

  - Decide speed of moving, decide how many time we need to stay on the place
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

- After the experiment:
  - Distance between anchors: 250,5 cm
  - Distance from camera to anchor baseline is 160,5 cm
  


- First experiemnt:

  - Without aruco marker, moving faster. 
  - Tags are calibrated at 5 m
  - blink 80 ms