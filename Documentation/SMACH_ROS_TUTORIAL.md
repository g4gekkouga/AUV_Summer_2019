# SMACH DOCUMENTATION [(TUTORIALS)](http://wiki.ros.org/smach/Tutorials)

## Contents :

<br /><br /><br />

Smach, which stands for "State Machine", is a powerful and scalable Python-based library for hierarchical state machines. The Smach library does not depend on ROS, and can be used in any Python project. The executive_smach stack however provides very nice integration with ROS, including smooth actionlib integration and a powerful Smach viewer to visualize and introspect state machines. 

### Do not use smach in following cases :
>Unstructured tasks: SMACH will fall short as the scheduling of your task becomes less structured.

>Low-level systems: SMACH is not meant to be used as a state machine for low-level systems that require high efficiency, SMACH is a task-level architecture. 

## Additional SMACH State Types [(Link)](http://wiki.ros.org/executive_smach/AdditionalStateTypes)
University of Applied Sciences Hamburg Robot Vision Lab ROS Repository.
<br />
States :

>WaitForMsgState - This class acts as a generic message listener with blocking, timeout, latch and flexible usage.

>SleepState - Sleep for a time duration, given either on initialization or via userdata.

>PromptState - Prompt and wait for user action or input on command line.

>CheckSmachEnabledState(WaitForMsgState) - Simple way to enable or disable smach via ROS message. 

>ReadRobotPositionState - Return the current robot position in the given frame via userdata.

>MoveBaseState - Calls a move_base action server with the goal (x, y, yaw) from userdata.

>CalcRandomGoalState - Return a random (x, y, yaw) tuple via userdata.

>TransformListenerSingleton - To avoid running multiple transform listeners, this singleton class provides one transform listener that is initialised and retrieved via class methods init() and get().

Methods :

>get_current_robot_position - Return a (x,y,yaw) tuple for the robot in a given frame. 

>get_random_goal_smach - Returns a SMACH Sequence for navigation to a random goal, combining CalcRandomGoalState with MoveBaseState. 

## SMACH_VIEWER [(Link)](http://wiki.ros.org/smach_viewer)
The smach viewer is a GUI that shows the state of hierarchical SMACH state machines. It can visualize the possible transitions between states, as well as the currently active state and the values of user data that is passed around between states. The smach viewer uses the SMACH debugging interface based on the smach messages to gather information from running state machines.<br /> To run the viewer :
>rosrun smach_viewer smach_viewer.py

![alt text](http://wiki.ros.org/smach/Tutorials/Smach%20Viewer?action=AttachFile&do=get&target=smach_viewer.png "Example of Smach_Viewer")

There are differrent view available like :
#### GRAPH VIEW
#### DEPTH VIEW
#### TREE VIEW

Your state machine needs to run an introspection server to allow the smach viewer to connect to it.

## Creating an Introspection Server [(Link)](http://wiki.ros.org/smach/Tutorials/Smach%20Viewer)
SMACH containers can provide a debugging interface (over ROS) which allows a developer to get full introspection into a state machine. The SMACH viewer can use this debugging interface to visualize and interact with your state machine. To add this debugging interface to a state machine, add the following lines to your code: 
```sh
# First you create a state machine sm
# .....
# Creating of state machine sm finished

# Create and start the introspection server
sis = smach_ros.IntrospectionServer('server_name', sm, '/SM_ROOT')
sis.start()

# Execute the state machine
outcome = sm.execute()

# Wait for ctrl-c to stop the application
rospy.spin()
sis.stop()
```
Then run the smach viewer

## Creating a State Machine [(Tutorial)](http://wiki.ros.org/smach/Tutorials/Getting%20Started)

### Creating a State :
The convention is to name states with all caps.
```sh
  class Foo(smach.State):
     def __init__(self, outcomes=['outcome1', 'outcome2']):
       # Your state initialization goes here

     def execute(self, userdata):
        # Your state execution goes here
        if xxxx:
            return 'outcome1'
        else:
            return 'outcome2'
 ```
 
 ### Adding states to a state machine :
 ```sh
  sm = smach.StateMachine(outcomes=['outcome4','outcome5'])
  with sm:
     smach.StateMachine.add('FOO', Foo(),
                            transitions={'outcome1':'BAR',
                                         'outcome2':'outcome4'})
     smach.StateMachine.add('BAR', Bar(),
                            transitions={'outcome2':'FOO'})
```
![alt text](http://wiki.ros.org/smach/Tutorials/Getting%20Started?action=AttachFile&do=get&target=simple.png "Graph Of Above Machine")
##### Note : Every state machine container is also a state. So you can nest state machines by adding a state machine container to another state machine container. 

Refer tha above link for an example of simple state machine example.
<br />
Though we can write our own states and containers , smach comes with a lot of predifines states and containers fo almost all situations as we will see further.

## Passing User Data between States [(Tutorial)](http://wiki.ros.org/smach/Tutorials/User%20Data)


 
 
 
 

