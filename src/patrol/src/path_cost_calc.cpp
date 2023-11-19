#include <ros/ros.h>
#include <navfn/navfn_ros.h>
#include <nav_msgs/GetPlan.h>
#include <geometry_msgs/PoseStamped.h>

class PathCostCalculator
{
public:
    PathCostCalculator() : nh("~")
    {
        // Advertise a service for planning paths
        ROS_DEBUG("stage 2 complete");
        plan_service = nh.serviceClient<nav_msgs::GetPlan>("MakeNavPlan");
        ROS_DEBUG("stage 3 complete");
    }

    double computePathCost(const geometry_msgs::PoseStamped &start, const geometry_msgs::PoseStamped &goal)
    {
        // Call the NavfnROS service to plan a path
        nav_msgs::GetPlan srv;
        srv.request.start = start;
        srv.request.goal = goal;

        if (plan_service.call(srv))
        {
            // Compute the cost of the path
            const auto &path = srv.response.plan.poses;

            double cost = 0.0;
            for (size_t i = 1; i < path.size(); ++i)
            {
                cost += calculateDistance(path[i - 1].pose.position, path[i].pose.position);
            }

            return cost;
        }
        else
        {
            ROS_ERROR("Failed to call the NavfnROS service");
            return -1.0; // Indicates an error
        }
    }

private:
    double calculateDistance(const geometry_msgs::Point &point1, const geometry_msgs::Point &point2)
    {
        // Calculate Euclidean distance between two points
        return std::sqrt(std::pow(point2.x - point1.x, 2) + std::pow(point2.y - point1.y, 2));
    }

private:
    ros::NodeHandle nh;
    ros::ServiceClient plan_service;
};



int main(int argc, char **argv)
{
    ros::init(argc, argv, "path_cost_calc");
    ROS_DEBUG("stage 1 complete");

    PathCostCalculator path_cost_calculator;

    // Define the start and goal points
    geometry_msgs::PoseStamped start;
    start.header.frame_id = "map";
    start.pose.position.x = 1.6;
    start.pose.position.y = 1.6;
    start.pose.orientation.w = 1.0;

    geometry_msgs::PoseStamped goal;
    goal.header.frame_id = "map";
    goal.pose.position.x = 0.8;
    goal.pose.position.y = 0.8;
    goal.pose.orientation.w = 1.0;

    // Compute the cost of the path
    ROS_DEBUG("stage 4 complete");
    double path_cost = path_cost_calculator.computePathCost(start, goal);

    if (path_cost >= 0.0)
    {
        ROS_INFO("Cost of the path from (%f, %f) to (%f, %f): %f", start.pose.position.x, start.pose.position.y,
                 goal.pose.position.x, goal.pose.position.y, path_cost);
    }
    else
    {
        ROS_ERROR("Failed to compute the path cost");
    }

    return 0;
}