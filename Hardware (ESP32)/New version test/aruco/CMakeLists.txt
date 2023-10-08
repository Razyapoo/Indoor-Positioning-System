cmake_minimum_required(VERSION 3.10)

project(Aruco)

# Find the OpenCV package
find_package(OpenCV REQUIRED)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")


add_executable(Aruco aruco.cpp)

# Link against OpenCV
target_link_libraries(Aruco ${OpenCV_LIBS})
