/*********************************************************************
*
* Software License Agreement (BSD License)
*
*  Copyright (c) 2014, ISR University of Coimbra.
*  All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*   * Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*   * Redistributions in binary form must reproduce the above
*     copyright notice, this list of conditions and the following
*     disclaimer in the documentation and/or other materials provided
*     with the distribution.
*   * Neither the name of the ISR University of Coimbra nor the names of its
*     contributors may be used to endorse or promote products derived
*     from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
*  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
*  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
*  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
*  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
*  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
*  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
*  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
*  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*  POSSIBILITY OF SUCH DAMAGE.
*
* Author: David Portugal (2011-2014), and Luca Iocchi (2014-2016)
*********************************************************************/

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>

#include <ros/ros.h>

int main(int argc, char** argv) {
    
    ros::init(argc,argv,"init_pos_node");
    ros::NodeHandle nh("~");

    std::string mapname = argv[1]; // Replace with the actual map name
    std::string fnr = "maps/" + mapname + "/robots.inc";
    std::ofstream fr(fnr);
    
    std::vector<std::string> colors = {"blue", "red", "cyan", "magenta", "blue", "red", "cyan", "magenta", "blue", "red", "cyan", "magenta"};

    
    std::string vip;

    nh.param<std::string>("initial_pos", vip, "[1.425, 24.375]");

    std::vector<double> ip;
    size_t pos = 0;
    while ((pos = vip.find(',')) != std::string::npos) {
        std::string token = vip.substr(1, pos - 1);
        vip.erase(0, pos + 1);
        ip.push_back(std::stod(token));
    }
    
    int n = ip.size() / 2; // Assuming ip is a std::vector<double> containing initial pose data

    for (int i = 0; i < n; ++i) {
        double x = ip[i * 2];
        double y = ip[i * 2 + 1];
        double th = 90.0;

        std::string fn = "params/amcl/robot_" + std::to_string(i) + "_initial_pose.xml";
        std::ofstream f(fn);

        f << "<launch>\n";
        f << "  <param name=\"amcl/initial_pose_x\" value=\"" << x << "\"/>\n";
        f << "  <param name=\"amcl/initial_pose_y\" value=\"" << y << "\"/>\n";
        f << "  <param name=\"amcl/initial_pose_a\" value=\"" << M_PI / 180.0 * th << "\"/>\n";
        f << "  <param name=\"srrg_localizer/initial_pose_x\" value=\"" << x << "\"/>\n";
        f << "  <param name=\"srrg_localizer/initial_pose_y\" value=\"" << y << "\"/>\n";
        f << "  <param name=\"srrg_localizer/initial_pose_a\" value=\"" << M_PI / 180.0 * th << "\"/>\n";
        f << "</launch>\n";
        f.close();

        fr << "crobot( pose [ " << x << "  " << y << "  0  " << th << " ]   name \"robot" << i << "\"  color \"" << colors[i] << "\")\n";
    }

    if (n == 1) {
        fr << "crobot( pose [ -2.0   -2.0    0   0.0 ]   name \"robot1\"  color \"red\")\n";
    }

    fr.close();

    return 0;
}
