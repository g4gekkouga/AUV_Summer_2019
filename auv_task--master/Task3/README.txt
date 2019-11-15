Direction of Movement based on task 2

Compilation :
	
1) Create a catkin workspace

2) Copy the enitre task_3 Directory present in src/ to your catkin_workspace/src directory

3) Build the package using catkin_make from the workspace directory

Note :

1) rosrun task_3 Task3_client – To run the client

2) rosrun task_3 Task3_server – To run the server

3)  rostopic echo /task3/feedback – To get the feedback status (Server name is task3) and similarly for result.

4) The code from Task 2 is used here in the client code. Extra lines of code added wherever commented

5) server returns only the direction of rotation and movement direction
