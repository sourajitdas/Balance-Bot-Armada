#include "ros/ros.h"
#include "balancebots/keycomm.h"
#include <iostream>
#include <string>

bool run(balancebots::keycomm::Request  &req,
         balancebots::keycomm::Response &res)
{
int a = req.key;

if (a == 1)
ROS_INFO("request: key is w");
else if (a == 2)
ROS_INFO("request: key is a");
else if (a == 3)
ROS_INFO("request: key is s");
else if (a == 4)
ROS_INFO("request: key is d");
else if (a == 5)
ROS_INFO("request: key is space");

//std::cout << req.key << std::endl;
res.resp = 1; 
ROS_INFO("response is %d",(int)res.resp);
  return true;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "Masterbot");
  ros::NodeHandle n;
  ros::ServiceServer service = n.advertiseService("keyhit_info", run);
  ROS_INFO("Ready");
  ros::spin();
  return 0;
}
