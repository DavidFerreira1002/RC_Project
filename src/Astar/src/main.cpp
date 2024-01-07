#include <iostream>
#include <ros/ros.h>
#include <nav_msgs/OccupancyGrid.h>
#include <nav_msgs/Path.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/Quaternion.h>
#include <opencv2/opencv.hpp>
#include "Astar.h"
#include "OccMapTransform.h"
#include <std_msgs/Bool.h>

#include <yaml-cpp/yaml.h>

using namespace cv;
using namespace std;


//-------------------------------- Global variables ---------------------------------//
// Subscriber
ros::Subscriber map_sub;
ros::Subscriber startPoint_sub;
ros::Subscriber targetPoint_sub;
ros::Subscriber clickedPoint_sub;
// Publisher
ros::Publisher mask_pub;
ros::Publisher path_pub;

// Object
nav_msgs::OccupancyGrid OccGridMask;
nav_msgs::Path path;
pathplanning::AstarConfig config;
pathplanning::Astar astar;
OccupancyGridParam OccGridParam;
Point startPoint, targetPoint, clickedPoint;
std::vector<Point> clickedPointsSet;
int id_ini;
int id_fin;

// Parameter
double InflateRadius;
bool map_flag;
bool startpoint_flag;
bool targetpoint_flag;
bool start_flag;
int rate;
string worldName;

bool mss_state = false;
bool astar_initiated = false;


void startingPath(){
    if(start_flag)
        {
            double start_time = ros::Time::now().toSec();
            // Start planning path
            vector<Point> PathList;
            astar.PathPlanning(startPoint, targetPoint, PathList, worldName);
            if(!PathList.empty())
            {
                path.header.stamp = ros::Time::now();
                path.header.frame_id = "map";
                path.poses.clear();
                for(int i=0;i<PathList.size();i++)
                {
                    Point2d dst_point;
                    OccGridParam.Image2MapTransform(PathList[i], dst_point);

                    geometry_msgs::PoseStamped pose_stamped;
                    pose_stamped.header.stamp = ros::Time::now();
                    pose_stamped.header.frame_id = "map";
                    pose_stamped.pose.position.x = dst_point.x;
                    pose_stamped.pose.position.y = dst_point.y;
                    pose_stamped.pose.position.z = 0;
                    path.poses.push_back(pose_stamped);
                }
                path_pub.publish(path);
                double end_time = ros::Time::now().toSec();

                ROS_INFO("Find a valid path successfully from %d to %d! Use %f s", id_ini, id_fin, end_time - start_time);
            }
            else
            {
                ROS_ERROR("Can not find a valid path");
            }

            // Set flag
            start_flag = false;
        }

        if(map_flag)
        {
            mask_pub.publish(OccGridMask);
        }
}


//-------------------------------- Callback function ---------------------------------//
void MapCallback(const nav_msgs::OccupancyGrid& msg)
{
    // Get parameter
    OccGridParam.GetOccupancyGridParam(msg);

    // Get map
    int height = OccGridParam.height;
    int width = OccGridParam.width;
    int OccProb;
    Mat Map(height, width, CV_8UC1);
    for(int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
        {
            OccProb = msg.data[i * width + j];
            OccProb = (OccProb < 0) ? 100 : OccProb; // set Unknown to 0
            // The origin of the OccGrid is on the bottom left corner of the map
            Map.at<uchar>(height-i-1, j) = 255 - round(OccProb * 255.0 / 100.0);
        }
    }

    // Initial Astar
    Mat Mask;
    config.InflateRadius = round(InflateRadius / OccGridParam.resolution);
    astar.InitAstar(Map, Mask, config);

    // Publish Mask
    OccGridMask.header.stamp = ros::Time::now();
    OccGridMask.header.frame_id = "map";
    OccGridMask.info = msg.info;
    OccGridMask.data.clear();
    for(int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
        {
            OccProb = Mask.at<uchar>(height-i-1, j) * 255;
            OccGridMask.data.push_back(OccProb);
        }
    }

    // Set flag
    map_flag = true;
    startpoint_flag = false;
    targetpoint_flag = false;
}

void StartPointCallback(const geometry_msgs::PoseWithCovarianceStamped& msg)
{
    Point2d src_point = Point2d(msg.pose.pose.position.x, msg.pose.pose.position.y);
    OccGridParam.Map2ImageTransform(src_point, startPoint);

    // Set flag
    startpoint_flag = true;
    if(map_flag && startpoint_flag && targetpoint_flag)
    {
        start_flag = true;
    }

//    ROS_INFO("startPoint: %f %f %d %d", msg.pose.pose.position.x, msg.pose.pose.position.y,
//             startPoint.x, startPoint.y);
}

void TargetPointtCallback(const geometry_msgs::PoseStamped& msg)
{
    Point2d src_point = Point2d(msg.pose.position.x, msg.pose.position.y);
    OccGridParam.Map2ImageTransform(src_point, targetPoint);

    // Set flag
    targetpoint_flag = true;
    if(map_flag && startpoint_flag && targetpoint_flag)
    {
        start_flag = true;
    }

//    ROS_INFO("targetPoint: %f %f %d %d", msg.pose.position.x, msg.pose.position.y,
//             targetPoint.x, targetPoint.y);
}

void ClickedPointCallback(const geometry_msgs::PointStamped& msg)
{
    ros::NodeHandle nh;

    Point2d src_point = Point2d(msg.point.x, msg.point.y);
    OccGridParam.Map2ImageTransform(src_point, clickedPoint);

    clickedPointsSet.push_back(clickedPoint);

    std::string pathToHere= __FILE__;

    size_t pos = pathToHere.find("/Astar/src/main.cpp");

    if (pos != std::string::npos) {
        pathToHere.erase(pos, std::string("/Astar/src/main.cpp").length());
    }

    std::string yamlPath = pathToHere + "/patrol/world/" + worldName + "/" + worldName + ".yaml";
    //cout << yamlPath << "\n";

    YAML::Node yaml_config = YAML::LoadFile(yamlPath);

    std::string filePath = pathToHere + "/patrol/world/" + worldName + "/" + worldName + ".graph";

    std::ifstream fin;
    fin.open(pathToHere + "/patrol/world/" + worldName + "/" + worldName + ".pgm");

    char magicNumber[2];
    fin.read(magicNumber, 2);
    //cout << magicNumber;
    char non_info[37];
    fin.read(non_info, 37);
    //cout << non_info<< "\n";
    //fin.ignore(std::numeric_limits<std::streamsize>::max(), '\n' );
    //std::vector<char> non_usable_info;

    //fin.read(non_usable_info,36)
    //fin.ignore(36, '\n');
    //fin.ignore();
    
    //int width, height;
    //fin.read(reinterpret_cast<char*>(&width), sizeof(width));
    //fin.read(reinterpret_cast<char*>(&height), sizeof(height));

    std::string width, height;
    fin >> width >> height;
    fin.close();

    int dimension = clickedPointsSet.size();
    float resolution;
    std::vector<float> origin;

    float yaw;
    std::vector<float> position;
    //nh.getParam("resolution", resolution);
    //nh.getParam("origin", origin);
    
    //cout << resolution << "\n";

    //float offset_x = origin.at(0);
    //float offset_y = origin.at(1);

    
    if (yaml_config["resolution"]) {
        resolution = yaml_config["resolution"].as<float>();
    }

    if (yaml_config["origin"]) {
        origin = yaml_config["origin"].as<std::vector<float>>();
    }
    
    //ros::param::get("resolution", resolution);
    //ros::param::get("origin", origin);
    
    //cout << resolution << "\n";

    float offset_x = origin[0];
    float offset_y = origin[1];
    

    //fin.open(pathToHere + "/patrol/world/CurrentWorld.yaml");
    /*
    const std::string path_yaml = pathToHere + "/patrol/world/CurrentWorld.yaml";
    YAML::Node yamlNode = YAML::LoadFile(path_yaml);
    float resolution; //= yamlNode["resolution"].as<float>();
    std::string offset_x; //= yamlNode["origin"][0].as<string>();
    std::string offset_y;// = yamlNode["origin"][1].as<string>();*/
    
    //fin.close();

    //std::ofstream fout;
    //fout.open(filePath);

    //std::ofstream outputFile("teste.txt", std::ios::trunc);

    //outputFile.open();
    /*
    if (outputFile.is_open()){
        //info mapa
        std::cout << "i'm in \n";
    }
    else{
        std::cerr << "Error creating or overwriting file \n";
    }*/
    //if (fout.is_open()){
    int width_int = stoi(width);
    int height_int = stoi(height);
    std::ofstream fout;
    fout.open(filePath);

    fout << dimension << "\n";
    fout << width << "\n";
    fout << height << "\n";
    fout << resolution << "\n";
    fout << offset_x << "\n";
    fout << offset_y << "\n" << "\n";

    for (int i = 0; i < clickedPointsSet.size(); ++i)
    {
        fout << i << "\n";
        fout << clickedPointsSet[i].x << "\n";
        fout << clickedPointsSet[i].y << "\n";
        fout << clickedPointsSet.size()-1 << "\n";
        //fout.close();

        for (int j = 0; j < clickedPointsSet.size(); ++j) 
        {
            startPoint = clickedPointsSet[i];
            startpoint_flag = true;
            if (j != i)
            {
                if (!fout.is_open()){
                    fout.open(filePath, ios::app);
                }

                fout << j << "\n";
                fout << "N" << "\n";
                //fout << "cost" << "\n";
                fout.close();
                
                targetPoint = clickedPointsSet[j];
                targetpoint_flag = true;
                id_ini = i;
                id_fin = j;
                
                if(map_flag && startpoint_flag && targetpoint_flag)
                {
                    start_flag = true;
                    startingPath();
                }
            }
        }
        fout.open(filePath, ios::app);
        fout << "\n";
    }
    //}
    //fout.close();
}


//callback caso o mapa já tenha sido gravado
void mssCallback(const std_msgs::Bool& mss){
	mss_state = mss.data;

  //caso o mapa já tenha sido guardado
  if (mss_state) {
    ROS_INFO("The user may now select points using the Publish Point feature on rviz!");
  }
}

//-------------------------------- Main function ---------------------------------//
int main(int argc, char * argv[])
{
    //  Initial node
    ros::init(argc, argv, "astar");
    ros::NodeHandle nh;
    ros::NodeHandle nh_priv("~");
    ROS_INFO("Start astar node!\n");

    // Initial variables
    map_flag = false;
    startpoint_flag = false;
    targetpoint_flag = false;
    start_flag = false;

    // Parameter
    nh_priv.param<bool>("Euclidean", config.Euclidean, true);
    nh_priv.param<int>("OccupyThresh", config.OccupyThresh, -1);
    nh_priv.param<double>("InflateRadius", InflateRadius, -1);
    nh_priv.param<int>("rate", rate, 10);
    nh_priv.param<string>("worldName", worldName, "CurrentWorld");

    ros::Subscriber mss_sub = nh.subscribe("/map_saved_status", 2, mssCallback);

    // Loop and wait for callback
    ros::Rate loop_rate(rate);
    while(ros::ok())
    {
        //caso o mapa esteja já guardado, pode iniciar o funcionamento do nodo Astar
        if(mss_state && !astar_initiated){
            // Subscribe topics
            map_sub = nh.subscribe("map", 10, MapCallback);
            startPoint_sub = nh.subscribe("initialpose", 10, StartPointCallback);
            targetPoint_sub = nh.subscribe("move_base_simple/goal", 10, TargetPointtCallback);
            clickedPoint_sub = nh.subscribe("clicked_point", 10, ClickedPointCallback);

            // Advertise topics
            mask_pub = nh.advertise<nav_msgs::OccupancyGrid>("mask", 1);
            path_pub = nh.advertise<nav_msgs::Path>("nav_path", 10);
            
            astar_initiated = true;
        }

        loop_rate.sleep();
        ros::spinOnce();
    }


    return 0;
}
