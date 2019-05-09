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












































