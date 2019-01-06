#include "ros/ros.h"
#include "balancebots/keycomm.h"
#include "std_msgs/String.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <curses.h>

int main(int argc, char **argv)
{
initscr();
clear();
noecho();
cbreak();
timeout(50);
nodelay(stdscr,TRUE);

int ch;
 ros::init(argc, argv, "command");
// ROS_INFO("enter w:front s:back a:left d:right");

  ros::NodeHandle na;
  ros::ServiceClient client = na.serviceClient<balancebots::keycomm>("keyhit_info");
   
  balancebots::keycomm srv;

//below code if argument given with command
//std::string test_str = std::string(argv[1]);
//std::string test_str = std::to_string(3.14);
//srv.request.key = test_str;
while(ros::ok())
{

//below code for keyboard hits
ch=getch();

if(ch != ERR)
{
if(ch == 'w'){
srv.request.key = 1;}
else if(ch == 'a'){
srv.request.key = 2;}
else if(ch == 's'){
srv.request.key = 3;}
else if(ch == 'd'){
srv.request.key = 4;}
else if(ch == ' '){
srv.request.key = 5;}
  if (client.call(srv))
  {
  //  ROS_INFO("Communicated to master bot %d",(int)srv.response.resp);
  }
  else
  {
  // ROS_ERROR("Failed to reach master bot");
    return 1;
  }

}								


}
  return 0;
}
