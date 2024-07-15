cmake_minimum_required(VERSION 3.1)
project(Calibration)

# Find the OpenCV package
find_package(OpenCV REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

# Add your source files
add_executable(Calibration main.cpp Calibrator.cpp Camera.cpp)

# Link against OpenCV
target_link_libraries(Calibration ${OpenCV_LIBS})
