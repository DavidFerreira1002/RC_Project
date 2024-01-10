#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <chrono>
#include <yolo_handler/ProcessedImageResults.h>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>
#include <yaml-cpp/yaml.h>
#include <yolo_handler/ObjectInformation.h>
#include <algorithm>
#include <yolo_handler/VertexInformation.h>

std::vector<std::vector<yolo_handler::ObjectInformation>> objects_array;
std::vector<std::vector<yolo_handler::ObjectInformation>> objects_new_vertex_array;
std::vector<std::vector<yolo_handler::ObjectInformation>> objects_temp_array;

std::vector<long long> vertex_information = {-1, 0, 0};

std::string worldName = "CurrentWorld";

int num_of_vertices;

int last_vertex;

int graph_vertex_count = 0;

int graph_vertex = -1;


struct ImageWithTimestamp {
    sensor_msgs::Image image;
    long long timestampInSeconds;

    ImageWithTimestamp(const sensor_msgs::Image& img, long long timestamp)
        : image(img), timestampInSeconds(timestamp) {}
};

std::vector<ImageWithTimestamp> depth_buffer;	

void depthCallback(const sensor_msgs::Image& depth_image){

	auto currentTimePoint = std::chrono::system_clock::now();

    auto durationSinceEpoch = currentTimePoint.time_since_epoch();

    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(durationSinceEpoch);

    long long timestampInSeconds = seconds.count();

	std::cout << timestampInSeconds << "\n";

    ImageWithTimestamp imageWithTimestamp(depth_image, timestampInSeconds);

    depth_buffer.push_back(imageWithTimestamp);


}


void depthBufferCallback(const yolo_handler::ProcessedImageResults& results) {

	std::string classification = results.classification;
	float x_image = results.x;
	float y_image = results.y;
	ros::Time yolo_time_stamp = results.timestampInSeconds;
	
	//seja pessoa ou objeto, é no sentido lato
	yolo_handler::ObjectInformation objectFound;
	objectFound.classification = classification;
	objectFound.x = x_image;
	objectFound.y = y_image;

	bool found_image = false;

	bool first_visit = true;

	if (depth_buffer.size() != 0) {

	for (size_t i = 0; i < depth_buffer.size(); ++i) {

		if (depth_buffer[i].timestampInSeconds > yolo_time_stamp.toSec()) {

			sensor_msgs::Image depth_image = depth_buffer[i].image;
			ROS_INFO("Found an image to associate it with");
			found_image = true;

			int depth_value = depth_image.data[y_image*depth_image.step + x_image];

			auto il = std::max_element(depth_image.data.begin(), depth_image.data.end());

			uint8_t max_value = *il;

			std::cout << "Depth of that pixel is: " << depth_value << "\n";
			std::cout << "Max value is: " << static_cast<int>(max_value) << "\n";

			/*
			//fazer contas com x, y e depth
			float focal_x = 521.3629760742188;
			float focal_y = 521.3629760742188;
			float c_x = 326.3819885253906
			float c_y = 241.0760040283203;

			float Z = depth_value;
			float X = depth_value/focal_x*(x_image - c_x);
			float Y = depth_value/focal_y*(y_image - c_y);

			//transformar em coordenadas do mapa

			geometry_msgs::PoseStamped pos_from_camera;
			pos_from_camera.header.frame_id = "obj_pos";
			pos_from_camera.header.stamp = timestamp;
			pos_from_camera.pose.position.x = X
			pos_from_camera.pose.position.y = Y
			pos_from_camera.pose.position.z = Z
			// pos_from_camera.pose.orientation.x  = 
			// pos_from_camera.pose.orientation.y  = 
			// pos_from_camera.pose.orientation.z  = 
			// pos_from_camera.pose.orientation.w  = 
			// (nao sei se isto tudo vai ser preciso)
			// (isto no referencial da camara)
			// get the transform from /camera to /map
			tf2_ros::Buffer tfBuffer;
			tf2_ros::TransformListener tfListener(tfBuffer);
			tfBuffer.canTransform("map", "astra_rgb_optical_frame", ros::Time(0), ros::Duration(5.0));
			geometry_msgs::TransformStamped transformStamped = tfBuffer.lookupTransform("map", "astra_rgb_optical_frame", ros::Time(0));
			geometry_msgs::PoseStamped map_pose;
			tf2::doTransform(pos_from_camera, map_pose, transformStamped);
			*/

			if (objectFound.classification == "person") {
				ROS_INFO("Já sei onde a pessoa está");
			}
			
			else {

				bool changed_vertex = false;

				//detetou enquanto está no local ou já depois de ter saído
				if ((vertex_information[1] != 0 && yolo_time_stamp.toSec() > vertex_information[1] && vertex_information[2] == 0) || (vertex_information[1] != 0 && yolo_time_stamp.toSec() > vertex_information[1] && yolo_time_stamp.toSec() < vertex_information[2]))
				{
					if (graph_vertex != vertex_information[0] && graph_vertex_count == num_of_vertices) {
						first_visit = false;
						changed_vertex = true;
						objects_temp_array = objects_array;
						objects_array = objects_new_vertex_array;
						std::vector<std::vector<yolo_handler::ObjectInformation>> emptyArray;
						objects_new_vertex_array = emptyArray;
						
						last_vertex = graph_vertex;
					}
					graph_vertex = vertex_information[0];

					graph_vertex_count = graph_vertex_count + 1;

					if (!first_visit) {

						for (size_t j = 0; j < objects_array[graph_vertex].size(); ++j) {
							yolo_handler::ObjectInformation objectStored = objects_array[graph_vertex][j];

							if (objectStored.classification == objectFound.classification && std::abs(objectStored.x - objectFound.x) < 0.50 && std::abs(objectStored.y - objectFound.y) < 0.50) {
								//o objeto em estudo foi encontrado
								objects_temp_array[graph_vertex].erase(objects_temp_array[graph_vertex].begin() + j);
								objects_new_vertex_array[graph_vertex].push_back(objectFound);

							}
						}

						//caso tenham ficado objetos por encontrar
						if (changed_vertex && objects_temp_array[last_vertex].size() > 0) {

							for (size_t k = 0; k < objects_temp_array[last_vertex].size(); ++k) {
								std::cout << "One object of classification " << objects_temp_array[last_vertex][k].classification << " was removed!" << "\n";
							}

							changed_vertex = false;

						}
					}
					else {
						//adiciona o objeto, é novo. Adiciona igualmente à new, devido à igualdade lá de cima
						objects_array[graph_vertex].push_back(objectFound);
						objects_new_vertex_array[graph_vertex].push_back(objectFound);
					}
				}
			
			}
			
			}

		if (found_image) {
			break;
		}
	}
	
	if (!found_image) {

			sensor_msgs::Image depth_image = depth_buffer.back().image;
			ROS_INFO("Found an image to associate it with");
			found_image = true;

			uint8_t depth_value = depth_image.data[y_image*depth_image.step + x_image];

			std::cout << "Depth of that pixel is: " << depth_value << "\n";

			//fazer contas com x, y e depth

			//transformar em coordenadas do mapa
			

			if (objectFound.classification == "person") {
				ROS_INFO("Já sei onde a pessoa está");
			}
			
			else {

				bool changed_vertex = false;

				//detetou enquanto está no local ou já depois de ter saído
				if ((vertex_information[1] != 0 && yolo_time_stamp.toSec() > vertex_information[1] && vertex_information[2] == 0) || (vertex_information[1] != 0 && yolo_time_stamp.toSec() > vertex_information[1] && yolo_time_stamp.toSec() < vertex_information[2]))
				{
					if (graph_vertex != vertex_information[0] && graph_vertex_count == num_of_vertices) {
						first_visit = false;
						changed_vertex = true;
						objects_temp_array = objects_array;
						objects_array = objects_new_vertex_array;
						std::vector<std::vector<yolo_handler::ObjectInformation>> emptyArray;
						objects_new_vertex_array = emptyArray;
						
						last_vertex = graph_vertex;
					}
					graph_vertex = vertex_information[0];

					graph_vertex_count = graph_vertex_count + 1;

					if (!first_visit) {

						for (size_t j = 0; j < objects_array[graph_vertex].size(); ++j) {
							yolo_handler::ObjectInformation objectStored = objects_array[graph_vertex][j];

							if (objectStored.classification == objectFound.classification && std::abs(objectStored.x - objectFound.x) < 0.50 && std::abs(objectStored.y - objectFound.y) < 0.50) {
								//o objeto em estudo foi encontrado
								objects_temp_array[graph_vertex].erase(objects_temp_array[graph_vertex].begin() + j);
								objects_new_vertex_array[graph_vertex].push_back(objectFound);

							}
						}

						//caso tenham ficado objetos por encontrar
						if (changed_vertex && objects_temp_array[last_vertex].size() > 0) {

							for (size_t k = 0; k < objects_temp_array[last_vertex].size(); ++k) {
								std::cout << "One object of classification " << objects_temp_array[last_vertex][k].classification << " was removed!" << "\n";
							}

							changed_vertex = false;

						}
					}
					else {
						//adiciona o objeto, é novo. Adiciona igualmente à new, devido à igualdade lá de cima
						objects_array[graph_vertex].push_back(objectFound);
						objects_new_vertex_array[graph_vertex].push_back(objectFound);
					}
				}
			
			}




	}

	}
	else {
		std::cout << "No depth received yet!" << "\n";
	}

}

/*
void vertexReachedCallback (yolo_handler::VertexInformation& vertex_info_msg) {
	int vertex_reached = vertex_info_msg.currentVertex;
	long long vertex_init_timestamp = vertex_info_msg.TimeStamp.toSec();
	vertex_information = {vertex_reached, vertex_init_timestamp, 0};
}

void vertexLeftCallback (yolo_handler::VertexInformation& vertex_info_msg) {
	int vertex_ended = vertex_info_msg.currentVertex;
	long long vertex_end_timestamp = vertex_info_msg.TimeStamp.toSec();

	if (vertex_information[1] != 0) {
		vertex_information[2] = vertex_end_timestamp;
	}
}
*/

int main(int argc, char **argv){
	ros::init(argc, argv, "rgbd_buffer");
	ros::NodeHandle nh;

	std::string pathToHere= __FILE__;

    size_t pos = pathToHere.find("/yolo_handler/src/rgbd_buffer.cpp");

    // If the substring is found, erase it
    if (pos != std::string::npos) {
        pathToHere.erase(pos, std::string("/yolo_handler/src/rgbd_buffer.cpp").length());
    }

    std::string filePath = pathToHere + "/patrol/world/" + worldName + "/" + worldName + ".graph";

	std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error opening YAML file." << std::endl;
        return 1;
    }

	/*
    // Load the YAML document
    YAML::Node yamlNode = YAML::Load(file);

    int targetLineNumber = 1;

	const YAML::Node& targetLine = yamlNode[targetLineNumber - 1];

	num_of_vertices = targetLine.as<int>();
	std::cout << "número de vértices do grafo: " << num_of_vertices << "\n";

	*/

	num_of_vertices = 5;

	for (size_t i = 0; i < num_of_vertices; ++i) {
		std::vector<yolo_handler::ObjectInformation> newVector;
		objects_array.push_back(newVector);
	}

	ros::Subscriber subs_detection = nh.subscribe("/astra/depth/image_raw", 10, depthCallback);
	ros::Subscriber depth_buffer_detection = nh.subscribe("/call_depth_image_buffer", 10, depthBufferCallback);

	/*
	ros::Subscriber vertex_reached_information = nh.subscribe("/start_rotation_time", 10, vertexReachedCallback);
	ros::Subscriber vertex_left_information = nh.subscribe("/end_rotation_time", 10, vertexLeftCallback);
	*/

	ros::Rate rate(10);

	while(ros::ok()){
		rate.sleep();
		ros::spinOnce();
	}
}
