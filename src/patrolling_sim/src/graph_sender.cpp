
#include <ros/ros.h>
#include <iostream>
#include <fstream>
#include <yaml-cpp/yaml.h>
#include "std_msgs/Int16.h"


int main(int argc, char **argv){
	ros::init(argc, argv, "graph_sender");
	ros::NodeHandle nh;
  
  std::string worldName = "CurrentWorld";

  std::string pathToHere= __FILE__;

    size_t pos = pathToHere.find("/patrolling_sim/src/graph_sender.cpp");

    // If the substring is found, erase it
    if (pos != std::string::npos) {
        pathToHere.erase(pos, std::string("/patrolling_sim/src/graph_sender.cpp").length());
    }

    std::string filePath = pathToHere + "/patrol/world/" + worldName + "/" + worldName + ".graph";

  std::string line;
  std::ifstream file(filePath);
  if (file.is_open()) {
      std::cout << line << "\n";
      file.close();
  }

  std_msgs::Int16 msg;

  msg.data = stoi(line);
  
	ros::Publisher num_vertex_pub = nh.advertise<std_msgs::Int16>("/num_of_vertices", 10);
  num_vertex_pub.publish(msg);
  
}



