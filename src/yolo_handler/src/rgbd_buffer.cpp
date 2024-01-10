#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <chrono>
#include <yolo_handler/ProcessedImageResults.h>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>
#include <yaml-cpp/yaml.h>
#include <yolo_handler/ObjectInformation.h>


std::vector<std::vector<yolo_handler::ObjectInformation>> objects_array;

int last_evaluated_vertex = -1;

std::string worldName = "CurrentWorld";

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

	bool first_visit;

	//int graph_vertex = ;
	
	//if (objects_array[graph_vertex].size() > 0) {
	//	first_visit = false;
	//}
	//else {
	//	first_visit = true;
	//}


	for (size_t i = 0; i < depth_buffer.size(); ++i) {

		if (depth_buffer[i].timestampInSeconds > yolo_time_stamp.toSec()) {

			sensor_msgs::Image depth_image = depth_buffer[i].image;
			ROS_INFO("Found an image to associate it with");
			found_image = true;

			cv_bridge::CvImagePtr cvImage;

			cvImage = cv_bridge::toCvCopy(depth_image, sensor_msgs::image_encodings::MONO8);

			uint8_t depth_value = cvImage->image.at<uint8_t>(y_image, x_image);

			//fazer contas com x, y e depth

			//transformar em coordenadas do mapa
			

			if (objectFound.classification == "person") {
				ROS_INFO("Já sei onde a pessoa está");
			}
			/*
			else {

				//aceder à lista referente ao vértice do grafo

				if (!first_visit) {
					std::vector<yolo_handler::ObjectInformation> new_visited_vector;
					std::vector<yolo_handler::ObjectInformation> temp_copy_vector;
					temp_copy_vector = objects_array[graph_vertex];

					for (size_t j = 0; j < objects_array[graph_vertex].size(); ++j) {
						yolo_handler::ObjectInformation objectStored = objects_array[graph_vertex][j];

						if (objectStored.classification == objectFound.classification && std::abs(objectStored.x - objectFound.x) < 0.50 && std::abs(objectStored.y - objectFound.y) < 0.50) {
							//o objeto em estudo foi encontrado
							temp_copy_vector.erase(temp_copy_vector.begin() + j);
							new_visited_vector.push_back(objectFound);

						}
					}

					if (temp_copy_vector.size() > 0) {

						for (size_t k = 0; k < temp_copy_vector.size(); ++k) {
							std::cout << "One object of classification " << temp_copy_vector[k].classification << " was removed!" << "\n";
						}

					}
			}
			else {
				objects_array[graph_vertex].push_back(objectFound);
			}
			}
			*/
			}

		if (found_image) {
			break;
		}
	}

}

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

    // Load the YAML document
    YAML::Node yamlNode = YAML::Load(file);

    int targetLineNumber = 1;

	const YAML::Node& targetLine = yamlNode[targetLineNumber - 1];

	int num_of_vertices = targetLine.as<int>();
	std::cout << "número de vértices do grafo: " << num_of_vertices << "\n";

	for (size_t i = 0; i < num_of_vertices; ++i) {
		std::vector<yolo_handler::ObjectInformation> newVector;
		objects_array.push_back(newVector);
	}

	ros::Subscriber subs_detection = nh.subscribe("/astra/depth/image_raw", 10, depthCallback);
	ros::Subscriber depth_buffer_detection = nh.subscribe("/call_depth_image_buffer", 10, depthBufferCallback);

	ros::Rate rate(10);

	while(ros::ok()){
		rate.sleep();
		ros::spinOnce();
	}
}
