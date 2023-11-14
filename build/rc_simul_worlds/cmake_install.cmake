# Install script for directory: /home/david/Projects/RC_Project/src/rc_simul_worlds

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "/home/david/Projects/RC_Project/build/rc_simul_worlds/catkin_generated/installspace/rc_simul_worlds.pc")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/rc_simul_worlds/cmake" TYPE FILE FILES
    "/home/david/Projects/RC_Project/build/rc_simul_worlds/catkin_generated/installspace/rc_simul_worldsConfig.cmake"
    "/home/david/Projects/RC_Project/build/rc_simul_worlds/catkin_generated/installspace/rc_simul_worldsConfig-version.cmake"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/rc_simul_worlds" TYPE FILE FILES "/home/david/Projects/RC_Project/src/rc_simul_worlds/package.xml")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/rc_simul_worlds" TYPE FILE FILES
    "/home/david/Projects/RC_Project/src/rc_simul_worlds/turtlebot3_gazebo.launch"
    "/home/david/Projects/RC_Project/src/rc_simul_worlds/turtlebot3_gazebo_house.launch"
    "/home/david/Projects/RC_Project/src/rc_simul_worlds/turtlebot3_gazebo_building.launch"
    "/home/david/Projects/RC_Project/src/rc_simul_worlds/turtlebot3_gazebo_1r5.launch"
    "/home/david/Projects/RC_Project/src/rc_simul_worlds/2turtlebot3_gazebo.launch"
    "/home/david/Projects/RC_Project/src/rc_simul_worlds/2turtlebot3_gazebo_building.launch"
    "/home/david/Projects/RC_Project/src/rc_simul_worlds/2turtlebot3_gazebo_1r5.launch"
    "/home/david/Projects/RC_Project/src/rc_simul_worlds/8turtlebot3_gazebo.launch"
    "/home/david/Projects/RC_Project/src/rc_simul_worlds/8turtlebot3_gazebo_building.launch"
    "/home/david/Projects/RC_Project/src/rc_simul_worlds/pioneer_stage.launch"
    "/home/david/Projects/RC_Project/src/rc_simul_worlds/pioneers_stage.launch"
    "/home/david/Projects/RC_Project/src/rc_simul_worlds/2pioneers_stage_MR_SLAM.launch"
    )
endif()

