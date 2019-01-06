#include "ros/ros.h"
#include "balancebots/instruction.h"
#include "geometry_msgs/PoseStamped.h"
#include "std_msgs/String.h"
#include <math.h>
#include <cstdlib>
#include <iostream>
#include <string>

double x1 = 0;
double ye1 = 0;
double oz1 = 0;

double x2 = 0;
double ye2 = 0;

double ox2 = 0;
double oy2 = 0;
double oz2 = 0;
double ow2 = 0;


void bot_m_loc(const geometry_msgs::PoseStamped::ConstPtr& msg)
{
//ROS_INFO("code comes here");
x1 = msg->pose.position.x;
//ROS_INFO("x1=%f",x1);
ye1 = msg->pose.position.y;
//ROS_INFO("y1=%f",ye1);
oz1 = msg->pose.orientation.z; 
//ROS_INFO("oz1=%f",oz1);
}


void bot1location(const geometry_msgs::PoseStamped::ConstPtr& msg2)
{
x2 = msg2->pose.position.x;
//ROS_INFO("x2=%f",x2);
ye2 = msg2->pose.position.y;
//ROS_INFO("y2=%f",ye2);
ox2 = msg2->pose.orientation.x;
//ROS_INFO("oz2=%f",ox2);
oy2 = msg2->pose.orientation.y;
//ROS_INFO("oz2=%f",oy2);
oz2 = msg2->pose.orientation.z;
//ROS_INFO("oz2=%f",oz2);
ow2 = msg2->pose.orientation.w;
//ROS_INFO("oz2=%f",ow2);
}




int main(int argc, char **argv)
{

double xd = 0;
double yd = 0;
double xds = 0;
double yds = 0;
double xs = 0;
double theta1 = 0;
double theta2 = 0;
double theta = 0;

//double znew = 0;
//double zd = 0;

ros::init(argc, argv, "botguider2");
ros::NodeHandle n;

//ROS_INFO("before subscribe");
ros::Subscriber sub = n.subscribe("vicon/bbot_1/pose", 100, bot_m_loc);
ros::Subscriber sub1 = n.subscribe("vicon/bbot_2/pose", 100, bot1location);
//ROS_INFO("after subscribe");

ros::Rate loop_rate(100);

ros::ServiceClient client = n.serviceClient<balancebots::instruction>("bot2follower");
balancebots::instruction srv;

while(ros::ok())
{

ROS_INFO("x1=%f",x1);
ROS_INFO("y1=%f",ye1);
ROS_INFO("x2=%f",x2);
ROS_INFO("y2=%f",ye2);

//below code for deciding where to go 

//linear translation logic

xd = x1 - x2;
xds = xd * xd;

yd = ye1 - ye2;
yds = yd * yd;

xs = sqrt(xds + yds);
ROS_INFO("distance=%f",xs);

if (xs > 0.6)
{
srv.request.phi = 1;
//ROS_INFO("moving towards bae");
}
else
{
srv.request.phi = 0;
//ROS_INFO("bae said to keep distance");
}

//orientation calculation

//theta 1 of source (yaw)

double sinq = 2 * ((ow2 * oz2) + (ox2 * oy2));
double cosq = 1 - (2 * ((oy2 * oy2) + (oz2 * oz2)));
theta1 = atan2(sinq,cosq);

// Dot product for angle between 2 vectors : yaw and vector formed by two bots
double cx = xd * cos(theta1);
double cy = yd * sin(theta1);
theta = acos((cx + cy)/xs);

// Cross Product between 2 vectos for direction
double kcap = (yd * cos(theta1)) - (xd * sin(theta1));

ROS_INFO("thetaold=%f thetanew=%f theta=%f, cos=%f",theta1,theta2,theta);

if (theta > 0.1)
{ 

if (kcap > 0)
{
srv.request.gamma = 0.3;
ROS_INFO("LEFT");
}
else
{
srv.request.gamma = -0.3;
ROS_INFO("RIGHT");
}

}


/*
//Quad 1
if ((xd > 0) && (yd > 0))
{

if (theta1 > theta2)
{
srv.request.gamma = -0.3;
ROS_INFO("RIGHT");
}
else 
{
srv.request.gamma = 0.3;
ROS_INFO("LEFT");
}

}

//Quad 2
if ((xd < 0) && (yd > 0))
{

if (3.1416 + theta2 < theta1)
{
srv.request.gamma = -0.3;
ROS_INFO("RIGHT");
}
else 
{
srv.request.gamma = 0.3;
ROS_INFO("LEFT");
}

}

//Quad 3
if ((xd < 0) && (yd < 0))
{

if (3.1416 + theta1 > theta2)
{
srv.request.gamma = -0.3;
ROS_INFO("RIGHT");
}
else 
{
srv.request.gamma = 0.3;
ROS_INFO("LEFT");
}

}

//Quad 4
if ((xd > 0) && (yd < 0))
{

if (theta1 > theta2)
{
srv.request.gamma = -0.3;
ROS_INFO("RIGHT");
}
else 
{
srv.request.gamma = 0.3;
ROS_INFO("LEFT");
}

}
*/

else if (theta < 0.1)
{
srv.request.gamma = 0;
ROS_INFO("straight");
}
 
 if (client.call(srv))
  {
  //ROS_INFO("Communicated to master bot %d",(int)srv.response.resp);
  }
  else
  {
  //ROS_ERROR("Failed to reach master bot");
  //return 1;
  } 
ros::spinOnce();
loop_rate.sleep();
}								

return 0;
}
