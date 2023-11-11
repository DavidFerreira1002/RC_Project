#include <ros/ros.h>
#include <sensor_msgs/CompressedImage.h>
#include <std_msgs/String.h>
#include <nav_msgs/Odometry.h>
#include <sensor_msgs/PointCloud2.h>
#include <tf/tf.h>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/calib3d.hpp>

#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_types.h>
#include <pcl/PCLPointCloud2.h>
#include <pcl/conversions.h>
#include <pcl_ros/transforms.h>

#include <fstream>
#include <map>

bool is_there_new_artwork = false;
std::string artwork_label = "";
std::vector<cv::KeyPoint> new_keypoints;
std::map<std::string, std::string> artworks;
std::ofstream artwork_database;

void barcodeCallback(const std_msgs::String& msg){
   is_there_new_artwork = true;
   artwork_label = msg.data;
   ROS_INFO("New artwork: %s", artwork_label.c_str());
}

//x,y,z são coords em relações ao robô, nx,ny,nz é a posição em relação ao mapa
bool calculatePoseWrtMap(const std::string& frame_id, const double x, const double y, const double z, double &nx, double &ny, double &nz){
   
   //procura as TF's quando o robô se mexe
   static tf::TransformListener tf_listener; //!static

   //para a transformação
   tf::StampedTransform tf_transform;

   try{
       // the first line shouldn't be necessary in general
       // but during the few milliseconds in the beginning we don't have the transform yet ready
       
       //o tf espera por dados; passa-os de frame_id para map
       tf_listener.waitForTransform("map", frame_id, ros::Time(0), ros::Duration(2.0));
       //guarda em tf_transform
       tf_listener.lookupTransform("map", frame_id, ros::Time(0), tf_transform);
   }
   catch(tf::TransformException& ex){
       ROS_ERROR_THROTTLE(0.2, "%s", ex.what());
       return false;
   }

   tf::Vector3 v(x, y, z);

   //aplica a transformação ao vetor v, i.e., às posições atuais
   v = tf_transform(v);
   nx = v.x();
   ny = v.y();
   nz = v.z();

	return true;
}


void pointcloudCallback(const sensor_msgs::PointCloud2& msg){
   if (new_keypoints.empty()){
       return;
   }

   //há point clouds densas e sparce. No caso, estamos a trabalhar com sparce, isto é, não há 1 ponto por cada pixel RGB-D
   //if (!msg.is_dense){ // a bug?
   //    return;
   //}

   // make sure it's ordered
   // if ((msg.height != last_img.height) || (msg.width != last_img.width)) return;

   //point cloud em representação não-ROS, pcl
   pcl::PCLPointCloud2 pc2;
   pcl_conversions::toPCL(msg, pc2);
   
   //objeto para guardar a point cloud
   pcl::PointCloud<pcl::PointXYZ>::Ptr pointcloud(new pcl::PointCloud<pcl::PointXYZ>);
   pcl::fromPCLPointCloud2(pc2, *pointcloud); //o asterisco é para ter o valor e não o pointer
   std::cout << " ---------------------- \n";
   double sum_x = 0.0;
   double sum_y = 0.0;
   double sum_z = 0.0;
   int count = 0;

   //ter o x,y,z para cada ponto
   for (int i=0; i<new_keypoints.size(); i++){

       //conversão das coordenadas do ponto em estudo para o index, que é a forma como a pointcloud é guardada
       int ind = new_keypoints[i].pt.x + new_keypoints[i].pt.y*msg.width;

       //garante que há uma medição para este ponto
       if (!std::isnan(pointcloud->points[ind].x)){
           sum_x += pointcloud->points[ind].x;
           sum_y += pointcloud->points[ind].y;
           sum_z += pointcloud->points[ind].z;
           count++;
       }
       std::cout << pointcloud->points[ind].x << "   " << pointcloud->points[ind].y << "   " << pointcloud->points[ind].z << " \n";
   }

   if (count == 0){
       return;
   }

   //cálculos das médias, que ficam na variável sum_x,y,z
   sum_x /= count;
   sum_y /= count;
   sum_z /= count;
   
   std::cout << " --- \n";
   std::cout << sum_x << "   " << sum_y << "   " << sum_z << "\n";
   double nx = 0.0;
   double ny = 0.0;
   double nz = 0.0;

   //retira o offset de a câmara não estar no chão mas sim no robô, é para dar a altura ao chão
   sum_z = sum_z - 0.30;
   

   calculatePoseWrtMap(msg.header.frame_id, sum_x, sum_y, sum_z, nx, ny, nz);
   

   char out_string[512];
   // format string: label, x, y, z
   sprintf(out_string, "%s, %2.2f, %2.2f, %2.2f\n", artwork_label.c_str(), nx, ny, nz);
   if (sqrt(sum_x*sum_x + sum_y*sum_y + sum_z*sum_z) < 2.0){
       auto rv = artworks.emplace(artwork_label, out_string);
       
       if (rv.second){
           //artwork_database é um ficheiro onde vamos guardar os dados
           artwork_database << out_string;
           artwork_database.flush();
       }        
   }

   std::cout << "# artwork: " << artworks.size() << std::endl;
   std::cout << "candidate: \n" << out_string << std::endl;

   new_keypoints.clear();
}


void cameraCallback(const sensor_msgs::CompressedImageConstPtr& msg){
	try{
		cv::Mat color_img = cv::imdecode(cv::Mat(msg->data), cv::IMREAD_COLOR);

	   //caso não haja novo artwork, isto é, caso não tenha sido detetado um barcode, só vamos mostrar a imagem
       if (!is_there_new_artwork){
           cv::Mat disp_img;
           cv::resize(color_img, disp_img, cv::Size(0, 0), 0.25, 0.25, cv::INTER_LINEAR);
           cv::imshow("Current Image", disp_img);
           cv::waitKey(1);                        
           return;
       }

	   //repõe a flag
       is_there_new_artwork = false;
       
       cv::Mat gray_img;
       //passa para intensity image
	   cv::cvtColor(color_img, gray_img, cv::COLOR_RGB2GRAY);        
       
	   //construção do SimpleBlobDetector para procurar barcodes
	   cv::SimpleBlobDetector::Params params;
       params.filterByColor = true;
       params.minThreshold = 0;
       params.maxThreshold = 200;
       cv::Ptr<cv::SimpleBlobDetector> detector = cv::SimpleBlobDetector::create(params);
        
       new_keypoints.clear();
	   //procura na imagem e guarda como new_keypoints
       detector->detect(gray_img, new_keypoints);
       
	   //objeto para guardar os blobs numa imagem nova
	   cv::Mat marked_img;
       if (new_keypoints.size()>0){
		   //desenha os keypoints sobre a color_img e guarda como marked:img
           cv::drawKeypoints(color_img, new_keypoints, marked_img, cv::Scalar(0,0,255)); // , cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS
       }
       cv::resize(marked_img, marked_img, cv::Size(0, 0), 0.25, 0.25, cv::INTER_LINEAR);
       imshow("Current Image", marked_img);
       cv::waitKey(1);
	}
	
	catch(cv::Exception& e){
		ROS_ERROR("Error converting image, %s", e.what());
	}
}


int main(int argc, char **argv){
	ros::init(argc, argv, "explorer");
	ros::NodeHandle nh;

    ros::Subscriber subs_barcode = nh.subscribe("/barcode", 1, barcodeCallback);
	ros::Subscriber subs_camera = nh.subscribe("/camera/rgb/image_raw/compressed", 1, cameraCallback);
    ros::Subscriber subs_pc = nh.subscribe("/camera/depth/points", 1, pointcloudCallback);
    

    //abrir o ficheiro da base de dados
    artwork_database.open("./artwork_database.csv", std::ios::out);
    
	ros::Rate rate(10);

	//isto assegura o rate, isto é, se for preciso esperar, dá sleep, caso contrário dá spin e passa para a próxima
	while(ros::ok()){
		rate.sleep();
		ros::spinOnce();
	}
	
	artwork_database.close();
}
