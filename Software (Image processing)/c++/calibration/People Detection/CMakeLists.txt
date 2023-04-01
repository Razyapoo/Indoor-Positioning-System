cmake_minimum_required(VERSION 3.5)
project(HumanDetection)

# Set OpenCV_DIR to the path where OpenCVConfig.cmake is located
#set(OpenCV_DIR /usr/local/include/opencv4/)

# Find the OpenCV package
find_package(OpenCV REQUIRED)
if(OPENCV_FOUND)
    message(STATUS "OPENCV Found")
else()
    message(STATUS "OPENCV Not Found")
endif()

# Check if CUDA is available
find_package(CUDA REQUIRED)
if(CUDA_FOUND)
    message(STATUS "CUDA Found")
else()
    message(STATUS "CUDA Not Found")
endif()

# Include OpenCV headers
#include_directories(${OpenCV_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

# Add your source files
add_executable(HumanDetection human_detection.cpp StereoCamera.cpp)

# Link against OpenCV
target_link_libraries(HumanDetection ${OpenCV_LIBS} ${CUDA_LIBRARIES})
