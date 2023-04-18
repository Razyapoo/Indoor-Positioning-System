cmake_minimum_required(VERSION 3.1)
project(StereoOffline)

# Set OpenCV_DIR to the path where OpenCVConfig.cmake is located
# set(OpenCV_DIR /usr/local/include/opencv4/)

# Find the OpenCV package
find_package(OpenCV REQUIRED)

# Include OpenCV headers
# include_directories(${OpenCV_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

# Add your source files
add_executable(StereoOffline main.cpp Disparity.cpp HumanDetector.cpp VideoManager.cpp)

# Link against OpenCV
target_link_libraries(StereoOffline ${OpenCV_LIBS})
