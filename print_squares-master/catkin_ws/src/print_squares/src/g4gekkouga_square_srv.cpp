#include "ros/ros.h"
#include "std_msgs/Int64.h"

bool square(const std_msgs::Int64::ConstPtr& msgr) {
  
  sq = (msgr->data)*(msgr->data);
  
  ROS_INFO("square of input", sq);
  
  return true;
}

int main(int argc, char **argv)
{
  
  ros::init(argc, argv, "g4gekkouga_square_srv");
  
  ros::NodeHandle n;

  ros::ServiceServer service = n.advertiseService("get_square", square);
  
  ROS_INFO("Ready to square input \n");
  
  ros::spin();

  return 0;
}