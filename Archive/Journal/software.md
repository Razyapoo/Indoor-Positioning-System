# How to compute the distance between two object 

## 1 method 

- Draw the grid on the floor (1m x 1m). Then detect on what cell a person is located. Compute location of a person inside a cell: we know the offset from the boundaries of the cell in pixels and we know the number of pixels of the boundary and we know that it is 1m in real world. Then we can compute the proportion and then compute the actual real distance.  
- https://answers.opencv.org/question/177732/how-to-measure-distance-between-2-objects-in-a-video-edited/
- https://dsp.stackexchange.com/questions/46399/how-to-measure-distance-between-two-3d-objects-using-cameras
- https://github.com/livinlargeinvenus/YOLOv4_Social_Distance
- Bird Eye view: 
  - https://medium.com/@birla.deepak26/social-distancing-ai-using-python-deep-learning-c26b20c9aa4c
    - https://github.com/deepak112/Social-Distancing-AI
  - https://www.ncbi.nlm.nih.gov/pmc/articles/PMC8249827/
  - https://github.com/basileroth75/covid-social-distancing-detection
  - https://github.com/mmaaz60/social_distancing
  - https://github.com/Vatsalparsaniya/Social-Distance-Surveillance

Best: 
- https://github.com/JohnBetaCode/Social-Distancing-Analyser
- https://github.com/secondlevel/Social-distance-detector
  - https://github.com/secondlevel/Social-distance-detector/blob/main/presentation.pdf
- https://github.com/kannsh/Development-of-Social-Distancing-Alert-System
- https://github.com/YassirMatrane/EnforcingSocialDistancingThroughComputerVision
- https://github.com/mmaaz60/social_distancing


Image transformations and camera calibration
- https://medium.com/analytics-vidhya/camera-calibration-theory-and-implementation-b253dad449fb
- https://www.mathworks.com/help/vision/ug/camera-calibration.html
- https://docs.opencv.org/4.5.3/dc/dbb/tutorial_py_calibration.html
- Homography matrix: https://docs.opencv.org/3.4/d9/dab/tutorial_homography.html
- Video course: https://www.youtube.com/watch?v=S-UHiFsn-GI&list=PL2zRqk16wsdoCCLpou-dGo7QQNks1Ppzo&index=1
- https://learnopencv.com/tag/camera-matrix/
- Camera parameters: https://www.cse.unr.edu/~bebis/CS791E/Notes/CameraParameters.pdf
- https://www.appsloveworld.com/opencv/100/5/to-calculate-world-coordinates-from-screen-coordinates-with-opencv

Why we cannot reconstruct 3D from 2D using single camera: 
- we don't know the depth
- https://www.quora.com/How-should-I-do-back-projections-from-2D-images-to-3D
- https://stackoverflow.com/questions/39394785/opencv-get-3d-coordinates-from-2d
- https://www.fdxlabs.com/calculate-x-y-z-real-world-coordinates-from-a-single-camera-using-opencv/
# How to track person

- Merge frames of both cameras into one frame: real-time video stitching
- Recognize a person
- Only detect and then track + his identification using tagID.
  - https://github.com/JunweiLiang/Object_Detection_Tracking
- object tracking:
  - https://opencv.org/multiple-object-tracking-in-realtime/
  - https://github.com/topics/multi-camera-tracking

# Stereo Vision

- depth calculation: https://github.com/LearnTechWithUs/Stereo-Vision
- very useful lib: https://github.com/erget/StereoVision
- 3D world coordinates https://github.com/mixr26/stereo_vision
- https://www.youtube.com/watch?v=sW4CVI51jDY
- Very good calibration: https://github.com/DLuensch/StereoVision-ADCensus
- https://www.youtube.com/watch?v=t3LOey68Xpg&list=PLkmvobsnE0GHMmTF7GTzJnCISue1L9fJn&index=13


# Back Projection

- http://chenlab.ece.cornell.edu/people/adarsh/publications/BackProjection.pdf

# Extrinsic parameters

- https://stackoverflow.com/questions/55220229/extrinsic-matrix-computation-with-opencv?rq=1

# Distance estimation using a single camera

- https://www.youtube.com/watch?v=iYAQL1bPQk8&list=PLkmvobsnE0GHMmTF7GTzJnCISue1L9fJn&index=21

# Tkinter Multithreading

- [Displaying a video feed with OpenCV and Tkinter](https://pyimagesearch.com/2016/05/30/displaying-a-video-feed-with-opencv-and-tkinter/)
- [OpenCV-with-tkinter-and-threading](https://github.com/informramiz/OpenCV-with-tkinter-and-threading/blob/master/GUI.py)
- [Increasing webcam FPS with Python and OpenCV](https://pyimagesearch.com/2015/12/21/increasing-webcam-fps-with-python-and-opencv/)