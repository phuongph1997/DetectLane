cmake_minimum_required(VERSION 3.10)
project(LamLaiDetectLane)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(OpenCV REQUIRED)

add_executable(${PROJECT_NAME} main.cpp )

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})