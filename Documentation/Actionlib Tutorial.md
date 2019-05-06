# ACTIONLIB TUTORIALS [Tutorials](http://wiki.ros.org/actionlib_tutorials/Tutorials)

## Creating the Action Messages

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

## Writing a Simple Server [Tutorial](http://wiki.ros.org/actionlib_tutorials/Tutorials/SimpleActionServer%28ExecuteCallbackMethod%29)

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

CallBackFunction can be written based on the requirement. For examples on callback function and its working, refer this [LINK](http://wiki.ros.org/actionlib_tutorials/Tutorials/SimpleActionServer%28ExecuteCallbackMethod%29)<br />

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

## Writing a Simple Action Client [Tutorial](http://wiki.ros.org/actionlib_tutorials/Tutorials/SimpleActionClient)

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















































>
