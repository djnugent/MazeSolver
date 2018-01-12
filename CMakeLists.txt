# Specify the minimum version for CMake
cmake_minimum_required(VERSION 2.8)

# Project's name
project(mazesolver)

# Set the output folder where your program will be created
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# Use opencv
FIND_PACKAGE( OpenCV REQUIRED )

# The following folder will be included
include_directories("${PROJECT_SOURCE_DIR}/src")
include_directories("${PROJECT_SOURCE_DIR}/include")
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable(mazesolver "${PROJECT_SOURCE_DIR}/src/mazesolver.cpp")

# Link opencv
target_link_libraries( mazesolver ${OpenCV_LIBS} )
