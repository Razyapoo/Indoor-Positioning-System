cmake_minimum_required(VERSION 3.1)
project(StereoCalibration)

# Set OpenCV_DIR to the path where OpenCVConfig.cmake is located
# set(OpenCV_DIR /usr/local/include/opencv4/)

# Find the OpenCV package
find_package(OpenCV REQUIRED)

# Include OpenCV headers
# include_directories(${OpenCV_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

# Add your source files
add_executable(StereoCalibration main.cpp Disparity.cpp StereoCalibrator.cpp StereoCamera.cpp VideoManager.cpp)

# Link against OpenCV
target_link_libraries(StereoCalibration ${OpenCV_LIBS})
