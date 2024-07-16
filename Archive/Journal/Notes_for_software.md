# How to compute the distance between two object

## 1 method

- Draw the grid on the floor (1m x 1m). Then detect on what cell a person is located. Compute location of a person inside a cell: we know the offset from the boundaries of the cell in pixels and we know the number of pixels of the boundary and we know that it is 1m in real world. Then we can compute the proportion and then compute the actual real distance.
- https://answers.opencv.org/question/177732/how-to-measure-distance-between-2-objects-in-a-video-edited/
- https://dsp.stackexchange.com/questions/46399/how-to-measure-distance-between-two-3d-objects-using-cameras

## Bird Eye view
  - https://www.ncbi.nlm.nih.gov/pmc/articles/PMC8249827/

## Camera calibration
- Camera parameters: https://www.cse.unr.edu/~bebis/CS791E/Notes/CameraParameters.pdf
- https://docs.opencv.org/4.5.3/dc/dbb/tutorial_py_calibration.html
- Homography matrix: https://docs.opencv.org/3.4/d9/dab/tutorial_homography.html
- Video course: https://www.youtube.com/watch?v=S-UHiFsn-GI&list=PL2zRqk16wsdoCCLpou-dGo7QQNks1Ppzo&index=1
- https://learnopencv.com/tag/camera-matrix/

# Extrinsic parameters

- https://stackoverflow.com/questions/55220229/extrinsic-matrix-computation-with-opencv?rq=1

# Distance estimation using a single camera

- https://www.youtube.com/watch?v=iYAQL1bPQk8&list=PLkmvobsnE0GHMmTF7GTzJnCISue1L9fJn&index=21

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

# Experiments

## May 31, 2023

- Observations:

  - There was wrong order of height and width parameters that is used as tuple representing the size of the chessboard `const cv::Size StereoCalibrator::chessboardSize`

    - must be:
      `uint8_t StereoCalibrator::chessboardHeight = 6, StereoCalibrator::chessboardWidth = 9; const cv::Size StereoCalibrator::chessboardSize = cv::Size(StereoCalibrator::chessboardHeight, StereoCalibrator::chessboardWidth);`
    - and the order of creating array of 3D points. Correct:

      for (auto i = 0; i < chessboardWidth; i++)
      for (auto j = 0; j < chessboardHeight; j++)
      objectPoint.push*back(cv::Point3f((float)j * squareSize, (float)i \_ squareSize, 0.0f));

  - Presence of a good light conditions is very important! When it is evening, results are bad
  - mono calibration best flags: cv::CALIB_CB_ADAPTIVE_THRESH + cv::CALIB_CB_NORMALIZE_IMAGE + cv::CALIB_CB_FILTER_QUADS
  

## June 3, 2023 Experiments in school

- Observations:

  1. Light conditions affect both calibration phase and runtime

  - rotation of the camera for micromilimeters may affect result

  4. Even with perfect calibration, during runtime lights from widnow may affect distance estimation

  6. During tests in dormitory and in school I have realized that the maximal estimated distance is around 9~9,5 meters. Is it true?

  - in the dormitory I had bad light conditions
  - in school there is a big wall behind at distance 10 m. May be that is the reason?


# April 2, 2024

- Testing trained model: XGbooster. Even when trained on 1, 2, and 3 lines (7 lines in total starting and ending by papers).

On 3,5 meters model shows x = 2,29; y = 3,7; reference values are x = 2,25; y = 3,5
Then it shows ( 2.42333 ,  1.95776 ) - predicted. When reference are (2.25, 2.0)

- At the beginning, I tried to train a model based on the first frame of the standing segment. But it showed that it is not a good choice, because I am still moving at this point.

- Then I decided to use middle frame to train a model. This helped. 

- But as further improvement, I need to try to fit model with 3 frames per segment: first, middle and last.

- Prediction also depends on the detection box, therefore on the detection model, how it is precised. More data is used to train a model, more flexible model becomes , and is able predict unknown coordinates.

- I have also observed that it is sufficient to train a model on the left half of the coridor, so it can predict y coordinated almost successfully on the right side of the corridor (middle paper line is deliminator). Unfortunatelly, when training on the half of coridor, there is no sufficient amount of data for x-axis to be successfully predicted.
	For example reference point is (3.08; 6.5); predicted is (2.17; 6,24); or reference point is (3.08 , 5.00), while predicted is ( 2.21943 ,  5.07038 ) 

- Need also to try to place camera above, to see the scene from the above. This way pixel coordinates will chage more impressivly.
