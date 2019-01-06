# Balance-bot-armada
A series of balance bots that take in tracking coordinates and follow a leader in various formations without crashing into each other.

Packages and Code structure :

VICON PACKAGE (mocap_vicon):
The Vicon package enables the use of the location and orientation for the objects selected to be tracked. 
vicon.launch : The Vicon launch node which uses the ros environment to push out coordinates for the objects selected in the Vicon motion tracking space. 
Launch file specifically selects which objects to analyze.
Link for github vicon repo: https://github.com/KumarRobotics/motion_capture_system.git

MASTER CONTROLLER PACKAGE (balancebots):
Relevant Codes in the Master URI computer to control the multi-bot operation: 
(CMakelist.txt lists out the executable files to be made when compiled and dependencies to be considered for the executable files)  
(Package.xml) Package info for "balance bot" package and the dependencies required for the executable files in the package.

1. command.cpp - Contains code for a ROS client node publishing key hits on the computer to the Leader/Master bot.
2. botguider.cpp - ROS location and orientation subscriber and client node. Guides the Second bot in line.
   Inputs from the Vicon environent - Master bot's location and orientation , Second bot's location and orientation. 
   Output - Instructions to move towards the first bot, actively checking the collision radius and direction pointing to the leader.
3. botguider2.cpp - ROS location and orientation subscriber and client node. Guides the Third bot in line. 
   Inputs from the Vicon environent - Second bot's location and orientation , Third bot's location and orientation. 
   Output - Instructions to move towards the second bot, actively checking the collision radius and direction pointing to the second bot.
4. obstacle: ROS client node for obstacle detection (premature: needs fixes)

BOT PACKAGE ("edumip_balance_ros" Package on all three bots)
Relevant Codes in the bot's catkin workspaces to actuate control instructions:

1. edumip_balance_ros_keyboard.cpp - Leader Bot ROS Server code communicating to the command node through "keyhit_info" service and keycomm.srv service type
2. follow_1.cpp - Bot 1 ROS server code communicating to botguider.cpp through "bot1follower" service and instruction.srv service type
3. follow_2.cpp - Bot 2 ROS server code communicating to botguider2.cpp through "bot2follower" service and instruction.srv service type
