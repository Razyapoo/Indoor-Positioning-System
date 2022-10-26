<h1 id="Annotations">How to annotate data</h1>

<h2 id="NN-matching">Neural network matching (based on [1])</h2>

[1] is the main basic paper used as motivation.

- How to check if detection is correct (without annotations)?
  - Run multiple NNs to detect people on the input video/image. The coincidence of several NNs can serve as an indicator that a person really exists.
  - Experiment shows that coincidence of 3 out of 5 NNs is better.
  - We can use this paper to say that 3 NNs is sufficient to use and provide our experiment using 3 NNs.

<h3>Usage of multiple cameras</h3>

How to use multiple cameras? How to synchronize them? 

Solutions:
- Use audio signal as starting point. 

<h2 id="controlled-experiment">Controlled experiment</h2>

- There are annotations, e.g. point on the floor showing the distance.


<h1>Why to use HW sensors?</h1>

1. There might be the case when it is not allowed to use cameras. For example:
  - Hospitals
  - Hotel private rooms
  - Private residence
  - Dressing rooms
  - Public restrooms (toilet)
  - locker rooms
  - private rooms in factories and in industry (new technologies, games)

2. Using sensors we can explicitly choose who to track. For example:
  - Nurses
  - Patients \
but not to track doctors or patients who is not allowed to be tracked (some special disease) or it does not make sense.

3. Combination of different sensors (position sensors + wearable sensors).
For example, combinations of position sensors and pressure monitors. What is the pressure of the person and where is he located.

<h1>Why to combine image detection (SW) and sensors (HW)</h1>

1. In the case where it is allowed to track people using security cameras (cctv), the combination may be used to improve the distance (position) detection. 
2. In the case where it is not allowed, cctv cameras may be used just for HW calibration purposes.
3. HW sensors are not always precise or there might be could be an obstacle, such as body or foil.

<h1>Experiments</h1>

We are working with physical data. The main purpose of the Master Thesis is to do experiments and show their results. We do only controlled experiments. We do not know what is ground truth but we want to know it.

We will make a table containing many different methods with their precision level. 

Main type of experiments:
   - only video
   - only sensors (+ video for calibration)
   - Video + sensors

Tips:

1. Try placing anchors in different places and at different heights. 
   - Anchors can be placed along or perpendicular to the tags

2. Do more experiments:
   - Museum
   - Rotunda
   - 3 anchors and 2 tags (triangulation)

3. Try to do:
   - offline experiments: process collected data offline
   - online experiments: process collected data online

Using sensors we can configure video detection algorithm, and otherwise using video we can configure and calibrate sensors.
 
<h2>Difficulties that may happen during experiment</h2>

- Long distance such that NNs cannot recognize objects
- Obstacles that make difficult to detect person (such as auto, chair etc.)
- How to determine real height of a person? (using door height?)

- How to compute the distance between people?
    - Work in 3D space and compute the Euclidean distance between points (mine formula is not working)
    - measure the heigh and positions of other different objects in the image and use them to determine positions of the people

<h1>Result of Master Thesis</h1>

The result of the Master Thesis will consist of many tables containing comparisons of several methods, their precision and error values.



<h1 id="references">References</h1>

- [ProcediaClanek](../../Papers/ProcediaClanek.pdf)
- 