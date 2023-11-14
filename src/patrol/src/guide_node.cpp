#include <ros/ros.h>
#include <std_msgs/String.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>

#include <map>
#include <fstream>

struct Point{
    double x; 
    double y; 
    double z;
};

std::map<std::string, Point> artwork_database;

bool moveTo(double x, double y);

void speechCallback(const std_msgs::String& msg){
    //procurar a string na db
    auto item = artwork_database.find(msg.data);
    //caso não encontre, devolve o .end

    if (item != artwork_database.end()){
        ROS_INFO("Moving to %s. Please follow me!", msg.data.c_str());
        moveTo(item->second.x, item->second.y);
    }
    else{
        ROS_ERROR("Target artwork not found in the database. Item was %s", msg.data.c_str());
    }
}


bool moveTo(double x, double y){
    
    actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction>  ac("move_base", true);
    //wait for the action server to come up
    while( !ac.waitForServer(ros::Duration(5.0)) && ros::ok()){ //esperar 5 segundos caso o server não esteja ainda ligado
        ROS_INFO("Waiting for the move_base action server to come up");
    }
    //local onde se quer ir
    move_base_msgs::MoveBaseGoal goal;
    
    //posição definida em relação ao mapa global
    goal.target_pose.header.frame_id = "map";
    goal.target_pose.header.stamp = ros::Time::now();

    //posição
    goal.target_pose.pose.position.x = x;
    goal.target_pose.pose.position.y = y;
    goal.target_pose.pose.position.z = 0.0;

    //orientação
    goal.target_pose.pose.orientation.x = 0.0;
    goal.target_pose.pose.orientation.y = 0.0;
    goal.target_pose.pose.orientation.z = 0.0;
    goal.target_pose.pose.orientation.w = 1.0;

    ROS_INFO("Sending goal %2.2f %2.2f", goal.target_pose.pose.position.x, goal.target_pose.pose.position.y);
    
    ac.sendGoal(goal); //pode-se ter funções callback

    //esperar que o server volte
    //ac.waitForResult(); //esperar indefinidamente
    bool result = ac.waitForResult(ros::Duration(30.0)); //espera até 30 segundos
    //if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
    
    //checks result
    if (result)
        ROS_INFO("We are there.");
    else
        ROS_INFO("Error reaching destination!");
    return false;
}

int main(int argc, char **argv){
	ros::init(argc, argv, "guide_node");
	ros::NodeHandle nh;

    ros::Subscriber subs_barcode = nh.subscribe("/speech", 1, speechCallback);

    std::ifstream file_artworkdb("artwork_database.csv", std::ifstream::in);
    if (!file_artworkdb.is_open()){
        ROS_ERROR("Could not open artwork database!");
        return -1;
    }
    
    std::string label;
    Point pt;
    std::string line;

    //ler linha a linha o ficheiro da db e separar na label + x + y + z
    while (std::getline(file_artworkdb, line)){
        
        char* pch;
        char buf[120];
        memcpy(buf, line.c_str(), line.size());
        pch = strtok(buf, ","); //divide até à próxima vírgula
        label = std::string(pch);
        
        pch = strtok(NULL, ",");
        pt.x = atof(pch);
        pch = strtok(NULL, ",");
        pt.y = atof(pch);
        pch = strtok(NULL, ",");
        pt.z = atof(pch);
        
        artwork_database.emplace(label, pt);
    }
    file_artworkdb.close();

    std::cout << "Current Artwork in the Database: (" << artwork_database.size() << ")" << std::endl;
    for(auto artwork : artwork_database){
        std::cout << artwork.first << "\t" << artwork.second.x << "\t" << artwork.second.y << "\t" << artwork.second.z << std::endl;
    }
    
	ros::Rate rate(10);
	while(ros::ok()){
		rate.sleep();
		ros::spinOnce();
	}
	
}
