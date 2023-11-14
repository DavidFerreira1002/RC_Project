# Install script for directory: /home/david/Projects/RC_Project/src/random_walk

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/david/Projects/RC_Project/install")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "/home/david/Projects/RC_Project/build/random_walk/catkin_generated/installspace/random_walk.pc")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/random_walk/cmake" TYPE FILE FILES
    "/home/david/Projects/RC_Project/build/random_walk/catkin_generated/installspace/random_walkConfig.cmake"
    "/home/david/Projects/RC_Project/build/random_walk/catkin_generated/installspace/random_walkConfig-version.cmake"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/random_walk" TYPE FILE FILES "/home/david/Projects/RC_Project/src/random_walk/package.xml")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/random_walk" TYPE FILE FILES
    "/home/david/Projects/RC_Project/src/random_walk/rndw_tsbot_lrf.launch"
    "/home/david/Projects/RC_Project/src/random_walk/rndw_tsbot_sonars.launch"
    "/home/david/Projects/RC_Project/src/random_walk/rndw_bringup_tbot_lrf.launch"
    "/home/david/Projects/RC_Project/src/random_walk/rndw_bringup_sbot_lrf.launch"
    "/home/david/Projects/RC_Project/src/random_walk/rndw_bringup_tbot_sonars.launch"
    "/home/david/Projects/RC_Project/src/random_walk/rndw_bringup_sbot_sonars.launch"
    "/home/david/Projects/RC_Project/src/random_walk/coppelia_1pioneer.launch"
    "/home/david/Projects/RC_Project/src/random_walk/coppelia_1pioneer_LRF.launch"
    "/home/david/Projects/RC_Project/src/random_walk/coppelia_2pioneer.launch"
    "/home/david/Projects/RC_Project/src/random_walk/coppelia_2pioneer_LRF.launch"
    "/home/david/Projects/RC_Project/src/random_walk/gazebo_1turtlebot_building.launch"
    "/home/david/Projects/RC_Project/src/random_walk/gazebo_2turtlebot_building.launch"
    "/home/david/Projects/RC_Project/src/random_walk/gazebo_8turtlebot_building.launch"
    "/home/david/Projects/RC_Project/src/random_walk/random_walk.launch"
    "/home/david/Projects/RC_Project/src/random_walk/real_pioneer_rndw.launch"
    "/home/david/Projects/RC_Project/src/random_walk/stage_1pioneer_building.launch"
    "/home/david/Projects/RC_Project/src/random_walk/stage_1pioneer_ISR.launch"
    "/home/david/Projects/RC_Project/src/random_walk/stage_2pioneer_building.launch"
    "/home/david/Projects/RC_Project/src/random_walk/stage_2pioneer_ISR.launch"
    "/home/david/Projects/RC_Project/src/random_walk/stage_8pioneer_building.launch"
    "/home/david/Projects/RC_Project/src/random_walk/stage_8pioneer_ISR.launch"
    "/home/david/Projects/RC_Project/src/random_walk/stage_teleop_1pioneer_building.launch"
    )
endif()

