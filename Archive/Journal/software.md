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
- USEFULL!!! camera calibration and triangulation: https://temugebatpurev.wordpress.com/2021/02/02/stereo-camera-calibration-and-triangulation-with-opencv-and-python/
- USEFUL!!! depth perception: https://learnopencv.com/depth-perception-using-stereo-camera-python-c/
- !!!VERY USEFUL!!! https://learnopencv.com/introduction-to-epipolar-geometry-and-stereo-vision/
- https://learnopencv.com/depth-perception-using-stereo-camera-python-c/

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


# Compilation of c++ with opencv

- need to install opencv
- need to configure program in CMakeLists
- install xorg to be able to open windows when running program from shell
- 


# March 03, 2023

- C++ version of stereo calibration is implemented. However with IP cameras it works worse than with webcameras. Frames are not synchronized. 
- In order to solve it, threaded version need to be implemented. Each camera will use a separate thread.

# March 18, 2023

- Manage to get more or less accurate results (depth estimation) with webcam and c++ setup
- Next step is to use YOLOv4 to detect people on the frames:
  - Need to install YOLOv4 from https://github.com/AlexeyAB/darknet and download weights https://github.com/AlexeyAB/darknet#pre-trained-models.
  - Intallation of the Darknet (YOLOv4) is perfomed locally, so there is need to add a path to darknet.h file in the CMakeLists.txt of the target project
  - For better performance of YOLOv4 there is need to install CUDA support for GPU. The name of my laptop is ASUS Zenbook UX530UQ and it has the Nvidia GeForce 940MX GPU with CUDA support. Current OS is Ubuntu 22.04 and version of nvidia driver is 525
    - First, make sure nvidia driver is installed by running the command: nvidia-detector. If not istalled, run "sudo apt install nvidia-driver-<version>"
    - Steps to install CUDA:
      - Run the command: sudo apt install nvidia-cuda-toolkit
      - To check whether CUDA has been installed, run "nvcc --version".
      - Run sudo apt update | sudo apt upgrade
      - Add path to cuda lib to ~/.bashrc file as follows: 
          - export PATH=/usr/lib/cuda/bin:$PATH
          - export LD_LIBRARY_PATH=/usr/lib/cuda/lib64:$LD_LIBRARY_PATH

    - Steps to install Darknet for YOLOv4 suppport: 
      - git clone https://github.com/AlexeyAB/darknet.git
      - cd darknet
      - "make" (this will install support for CUDA)
        - If there is no support for CUDA use: "make CPU"
    
    - Steps to install CUDNN (doc: https://docs.nvidia.com/deeplearning/cudnn/install-guide/index.html): 
      - go to https://developer.nvidia.com/rdp/cudnn-archive and download .deb package
      - run: sudo dpkg -i cudnn-local-repo-ubuntu2204-8.8.1.3_1.0-1_amd64.deb
      - run: sudo cp /var/cudnn-local-repo-ubuntu2204-8.8.1.3/cudnn-local-A917C751-keyring.gpg /usr/share/keyrings/
      - run: sudo apt-get install libcudnn8
      - sudo apt-get install libcudnn8-dev
      - sudo apt-get install libcudnn8-samples