cmake_minimum_required(VERSION 3.0.2)
project(patrol)

set (OpenCV_DIR /usr/lib/x86_64-linux-gnu/cmake/opencv4)

## Compile as C++11, supported in ROS Kinetic and newer
#add_compile_options(-std=c++14)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  roscpp
  OpenCV
  pcl_ros
  
)

# the following is used to install the nodes so that rosrun can find them
catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES motion_from_camera
#  CATKIN_DEPENDS opencv
#  DEPENDS system_lib
)

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)

## Add cmake target dependencies of the executable
## same as for the library above



## Specify libraries to link a library or executable target against

