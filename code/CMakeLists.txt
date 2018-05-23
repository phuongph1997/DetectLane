cmake_minimum_required(VERSION 3.10)
project(DetectLane)

set(CMAKE_CXX_STANDARD 11)
set(Source_file main.cpp)
find_package(OpenCV 3.1 REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
add_executable(DetectLane main.cpp)
target_link_libraries(DetectLane ${OpenCV_LIBS})