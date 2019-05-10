# CATKIN DOCUMENTATION [(TUTORIALS)](http://wiki.ros.org/catkin/Tutorials)

## Create a Workspace :

>mkdir -p (workspace_name)/src

>cd (workspace_name)/

>catkin_init_workspace (Optional, not requires if catkin_make is run initially)

>catkin_make

>source devel/setup.bash

>echo $ROS_PACKAGE_PATH


## Create a Package : [(Tutorial)](http://wiki.ros.org/catkin/Tutorials/CreatingPackage)

The recommended method of working with catkin packages is using a catkin workspace, but you can also build catkin packages standalone. A trivial workspace might look like this: 
```sh
workspace_folder/        -- WORKSPACE
  src/                   -- SOURCE SPACE
    CMakeLists.txt       -- 'Toplevel' CMake file, provided by catkin
    package_1/
      CMakeLists.txt     -- CMakeLists.txt file for package_1
      package.xml        -- Package manifest for package_1
    ...
    package_n/
      CMakeLists.txt     -- CMakeLists.txt file for package_n
      package.xml        -- Package manifest for package_n
```

#### Create a package :

In the src folder :

>catkin_create_pkg (package_name) roscpp std_msgs (other dependencies)

#### Grom Git :

In the src folder : 

>mkdir (package_name)

>cd (package_name)

>git init

>git pull (link og the package to be pulles)

To build this package , run catkin_make in the parent directory of the workspace.

### Package Dependencies :

The First Order Dependencies are those that are mentioned during the package creation. These dependences are stores in package.xml file. These dependencies can be viewed by following command :

>rospack depends1 package_name

These dependencies can further depend on other Indirect Dependencies. Then can be viewed using :

>rospack depends1 First_order_dependency_name

The following comman will show all the dependencies :

>rospack depends Package_name

### Customizing Your Package :

Edit the package.xml file and make any changes required before building the package. Refer the above tutorial link for furthe details on making changes this file.<br />
Example:
```sh
<?xml version="1.0"?>
<package format="2">
  <name>beginner_tutorials</name>
  <version>0.1.0</version>
  <description>The beginner_tutorials package</description>

  <maintainer email="you@yourdomain.tld">Your Name</maintainer>
  <license>BSD</license>
  <url type="website">http://wiki.ros.org/beginner_tutorials</url>
  <author email="you@yourdomain.tld">Jane Doe</author>

  <buildtool_depend>catkin</buildtool_depend>

  <build_depend>roscpp</build_depend>
  <build_depend>rospy</build_depend>
  <build_depend>std_msgs</build_depend>

  <exec_depend>roscpp</exec_depend>
  <exec_depend>rospy</exec_depend>
  <exec_depend>std_msgs</exec_depend>

</package>
```
The above is an example of the package file without any comments and tags.

## Overlaying with catkin workspaces [(Tutorial)](https://www.hackerrank.com/domains/tutorials/30-days-of-code?filters%5Bstatus%5D%5B%5D=unsolved&badge_type=30-days-of-code)

Overlaying refers to building and using a ROS package from source on top of an existing version of that same package. In this way your new or modified version of the package "overlays" the installed one. 

#### wstool : [(Link)](http://wiki.ros.org/wstool)

To install , try any one of the following commands :
> $ sudo apt-get install python-wstool <br />
>$ sudo pip install -U wstool <br />

It includes command-line tools for maintaining a workspace of projects from multiple version-control systems.
 <br />
 
 
  









































