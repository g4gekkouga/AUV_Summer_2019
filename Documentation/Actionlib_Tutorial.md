# ACTIONLIB TUTORIALS [Tutorials](http://wiki.ros.org/actionlib_tutorials/Tutorials)

## Contents :
* [Creating the Action Messages](#1)
* [Writing a Simple Server](#2)
* [Writing a Simple Action Client](#3)
* [Writing a Simple Action Server using the Goal Callback Method](#4) 
* [Writing a Threaded Simple Action Client](#5)
* [Running an Action Server and Client with Other Nodes](#6)
* [Writing a Callback Based SimpleActionClient](#7)
## Creating the Action Messages <a name="1"> </a>

In the scr folder of the package , create a action folder and place all the .action files in this folder.<br />
.action file format : (Example)

```sh
#goal definition
int32 order
---
#result definition
int32[] sequence
---
#feedback
int32[] sequence
```

The order of goal, result and feedback has to be maintained.

```sh
add_action_files(
  DIRECTORY action
  FILES Fibonacci.action
)

generate_messages(
  DEPENDENCIES actionlib_msgs std_msgs
)

catkin_package(
  CATKIN_DEPENDS actionlib_msgs
)
```
Add the above lines to CMake file to compile .action file.

```sh
<exec_depend>message_generation</exec_depend>
```
Add this line to the package.xml file.

## Writing a Simple Server [Tutorial](http://wiki.ros.org/actionlib_tutorials/Tutorials/SimpleActionServer%28ExecuteCallbackMethod%29) <a name="2"> </a>

Libraries to Include :

```sh
#include <ros/ros.h>
#include <actionlib/server/simple_action_server.h>
#include <(package_name)/(name of action files to include).h>
(Similarly for other action files to include)
```

Main Function :

```sh
int main(int argc, char** argv)
{
  ros::init(argc, argv, "server_name");

  CallbackFunction_Name object_name(Args to callback function);
  ros::spin();

  return 0;
}
```

CallBackFunction can be written based on the requirement. For examples on callback function and its working, refer this [LINK](http://wiki.ros.org/actionlib_tutorials/Tutorials/SimpleActionServer%28ExecuteCallbackMethod%29) <br />

NOTE : Can include using namespace (package_name) and need not include it before function names.<br /> 

Add the following lines of code to CMakeLists file to build the server :

```sh
add_executable(<server_name> src/<server_name>.cpp)

target_link_libraries(
  <server_name>
  ${catkin_LIBRARIES}
)

add_dependencies(
  <server_name>
  ${<package_name>_EXPORTED_TARGETS}
)
```

Also add the following line at the start if boost library is used to wrap any code :

```sh
find_package(Boost REQUIRED COMPONENTS system)
```

To run the server : (note that roscore i.e master should be running)

>rosrun package_name server_name

## Writing a Simple Action Client [Tutorial](http://wiki.ros.org/actionlib_tutorials/Tutorials/SimpleActionClient) <a name="3"> </a>

Refer the above link for basic client example and the libraries to include.<br /><br />

Add the following lines of code to CMakeLists file to build the server :

```sh
add_executable(<client_name> src/f<client_name>.cpp)

target_link_libraries( 
  <client_name>
  ${catkin_LIBRARIES}
)

add_dependencies(
  <client_name>
  ${<package_name>_EXPORTED_TARGETS}
)
```

Also add the following line at the start if boost library is used to wrap any code :

```sh
find_package(Boost REQUIRED COMPONENTS system)
```

To run the server : (note that roscore i.e master should be running)

>rosrun package_name client_name

Similarly, refer the following links for writing clients and servers in python.<br />

### Writing Simple Action Server in [Python](http://wiki.ros.org/actionlib_tutorials/Tutorials/Writing%20a%20Simple%20Action%20Server%20using%20the%20Execute%20Callback%20%28Python%29)
### Writing Simple Action Client in [Python](http://wiki.ros.org/actionlib_tutorials/Tutorials/Writing%20a%20Simple%20Action%20Client%20%28Python%29)

## Writing a Simple Action Server using the Goal Callback Method [Tutorial](http://wiki.ros.org/actionlib_tutorials/Tutorials/SimpleActionServer%28GoalCallbackMethod%29) <a name="4"> </a>

Note : For geneating or compiling message files manually (not using catkin_make), run the foolowing in the terminal<br />
>$ roscd package_name <br />
>$ rosrun actionlib_msgs genaction.py -o msg/ action/(action file name).action

In the server , we write a goal callback function to keep accepting the incomming goals from the client and store the received goals. Example :
```sh
 void goalCB()
 {
 // reset helper variables
 data_count_ = 0;
 sum_ = 0;
 sum_sq_ = 0;
 // accept the new goal
 goal_ = as_.acceptNewGoal()->samples;
 }
```
Here is the goalCB function referenced in the constructor. The callback function returns nothing and takes no arguments. When the goalCB is called the action needs to accept the goal and store any important information.<br />

Then we write an analysis callback function that is called whenever a goal is received from the client. This function computes the required task and outputs the results and increments the feedback. Example :
```sh
  void analysisCB(const std_msgs::Float32::ConstPtr& msg)
  {
    // make sure that the action hasn't been canceled
    if (!as_.isActive())
      return;
```
Here the analysis callback takes the message format of the subscribed data channel and checks that the action is still in an active state before continuing to process the data. <br />

Compiling and running the server is similar to the above mentioned process in Server with Executive CallBack.

## Writing a Threaded Simple Action Client [Tutorial](http://wiki.ros.org/actionlib_tutorials/Tutorials/SimpleActionClient%28Threaded%29) <a name="5"> </a>

Extra libraries to include :
```sh
#include <ros/ros.h>
#include <actionlib/client/terminal_state.h>
#include <boost/thread.hpp>
```

The client needs to be running and an external function can be written for that rather than in the main like :
```sh 
void spinThread()
{
  ros::spin();
}
```
Refer the above tutorial link for an example client code and its working.<br/><br />

Compiling and running the client is similar to the above process in simple action client.<br />

## Running an Action Server and Client with Other Nodes [Tutorial](http://wiki.ros.org/actionlib_tutorials/Tutorials/RunningServerAndClientWithNodes) <a name="6"> </a>

Before Running the action server and client , we can create a Data Node which publishes the required data to a specific topic and then the server and the client can access that data. The following is an example of data node in python :

```sh
#!/usr/bin/env python

import rospy
from std_msgs.msg import Float32
import random
def gen_number():
    pub = rospy.Publisher('random_number', Float32)
    rospy.init_node('random_number_generator', log_level=rospy.INFO)
    rospy.loginfo("Generating random numbers")

    while not rospy.is_shutdown():
        pub.publish(Float32(random.normalvariate(5, 1)))
        rospy.sleep(0.05)

if __name__ == '__main__':
  try:
    gen_number()
  except Exception, e:
    print "done"
```
Don't forget to make the node executable: 
>chmod +x gen_numbers.py

The above created data node publishes random numbers to the random_number topic which follow normal distribution with mean 5 and variance 1. Then the server and client can subscribe to topic random_number and access the produce data. Start the data node before accessing it.

<br />

## Writing a Callback Based SimpleActionClient [Tutorial](http://wiki.ros.org/actionlib_tutorials/Tutorials/Writing%20a%20Callback%20Based%20Simple%20Action%20Client) <a name="7"> </a>

In some cases, blocking until a goal completes doesn't provide enough flexibility. Instead, event based execution might make more sense. We can use callbacks to avoid using a waitForResult() call to block for the goal to finish. 
<br />
Example :
```sh
void doneCb(const actionlib::SimpleClientGoalState& state,
            const FibonacciResultConstPtr& result)
{
  ROS_INFO("Finished in state [%s]", state.toString().c_str());
  ROS_INFO("Answer: %i", result->sequence.back());
  ros::shutdown();
}
```
This above method is executed once the goal is completed.

```sh
void activeCb()
{
  ROS_INFO("Goal just went active");
}
```
The above method is called once the goal went active.<br /><br />

But if we want to call the method every time a feedback is received, we can use a method based on callback every time feedback is received. Example:
```sh
void feedbackCb(const FibonacciFeedbackConstPtr& feedback)
{
  ROS_INFO("Got Feedback of length %lu", feedback->sequence.size());
}
```
<br />
If we want to use classes instead of methods, we will have to use boost library to wrap the classes. Refer the above tutorial link for example on usein classes.















































>
