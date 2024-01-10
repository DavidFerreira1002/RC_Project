#include <ros/ros.h>
#include <vision_msgs/Detection2DArray.h>
#include <yolo_handler/ProcessedImageResults.h>

ros::Publisher call_depth_image_buffer;

void detectionCallback(const vision_msgs::Detection2DArray& det_array){

	ROS_INFO("got something");
	std::vector<vision_msgs::Detection2D> det = det_array.detections;

	std::vector<std::string> categories = {
        "person", "bicycle", "car", "motorcycle", "airplane", "bus", "train", "truck", "boat",
        "traffic light", "fire hydrant", "stop sign", "parking meter", "bench", "bird", "cat",
        "dog", "horse", "sheep", "cow", "elephant", "bear", "zebra", "giraffe", "backpack",
        "umbrella", "handbag", "tie", "suitcase", "frisbee", "skis", "snowboard", "sports ball",
        "kite", "baseball bat", "baseball glove", "skateboard", "surfboard", "tennis racket",
        "bottle", "wine glass", "cup", "fork", "knife", "spoon", "bowl", "banana", "apple",
        "sandwich", "orange", "broccoli", "carrot", "hot dog", "pizza", "donut", "cake", "chair",
        "couch", "potted plant", "bed", "dining table", "toilet", "tv", "laptop", "mouse", "remote",
        "keyboard", "cell phone", "microwave", "oven", "toaster", "sink", "refrigerator", "book",
        "clock", "vase", "scissors", "teddy bear", "hair drier", "toothbrush"
    };

	for (const auto& detection : det) {
        float x = detection.bbox.center.x;
        float y = detection.bbox.center.y;

		std::string classification;

		std::vector<vision_msgs::ObjectHypothesisWithPose> det_results = detection.results;
		for (const auto& result : det_results) {
			int id = result.id;
			classification = categories[id];
			float score = result.score;
			std::cout << "id is: " << id << "\n";
			std::cout << "classification is: " << classification << "\n";
			std::cout << "score is: " << score << "\n";


			if (id == 0) {
				ROS_INFO("A person was found!!");
			}


		}
		ros::Time time_stamp = det_array.header.stamp;
		std::cout << "x is: " << x << "\n";
		std::cout << "y is: " << y << "\n";
		std::cout << "TIMESTAMP: " << time_stamp << "\n";

		yolo_handler::ProcessedImageResults resultsToSend;

		resultsToSend.classification = classification;
		resultsToSend.x = x;
		resultsToSend.y = y;
		resultsToSend.timestampInSeconds = time_stamp;

		call_depth_image_buffer.publish(resultsToSend);
    }

	


}

int main(int argc, char **argv){
	ros::init(argc, argv, "yolo_handler");
	ros::NodeHandle nh;

	ros::Subscriber subs_detection = nh.subscribe("/yolov7/yolov7", 10, detectionCallback);
	call_depth_image_buffer = nh.advertise<yolo_handler::ProcessedImageResults>("/call_depth_image_buffer",10);

	ros::Rate rate(10);
	while(ros::ok()){
		rate.sleep();
		ros::spinOnce();
	}
}
