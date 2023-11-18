cmake_minimum_required(VERSION 3.1)
project(explore_slam)

## Find catkin macros and libraries
find_package(catkin REQUIRED COMPONENTS
  actionlib
  actionlib_msgs
  costmap_2d
  geometry_msgs
  map_msgs
  move_base_msgs
  nav_msgs
  roscpp
  std_msgs
  tf
  visualization_msgs
)

###################################
## catkin specific configuration ##
###################################
catkin_package(
  CATKIN_DEPENDS
    actionlib
    actionlib_msgs
    costmap_2d
    geometry_msgs
    map_msgs
    move_base_msgs
    nav_msgs
    roscpp
    std_msgs
    tf
    visualization_msgs
)

###########
## Build ##
###########
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

## Specify additional locations of header files
include_directories(
  ${catkin_INCLUDE_DIRS}
)

add_executable(explore src/explore.cpp src/costmap_client.cpp src/frontier_search.cpp)
#add_executable(costmap_client src/costmap_client.cpp)
#add_executable(frontier_search src/frontier_search.cpp)

add_dependencies(explore ${explore_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
#add_dependencies(costmap_client ${explore_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
#add_dependencies(frontier_search ${explore_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

target_link_libraries(explore
  ${catkin_LIBRARIES}
)

#target_link_libraries(costmap_client
#  ${catkin_LIBRARIES}
#)

#target_link_libraries(frontier_search
#  ${catkin_LIBRARIES}
#)

#############
## Install ##
#############

# install nodes
#install(TARGETS explore_slam
#  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
#)

# install roslaunch files
#install(DIRECTORY launch/
#  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/launch
#)

#############
## Testing ##
#############
#if(CATKIN_ENABLE_TESTING)
#  find_package(roslaunch REQUIRED)

  # test all launch files
#  roslaunch_add_file_check(launch)
#endif()
