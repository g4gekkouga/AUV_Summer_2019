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

### Specifying User Data :
```sh
  class Foo(smach.State):
     def __init__(self, outcomes=['outcome1', 'outcome2'],
                        input_keys=['foo_input'],
                        output_keys=['foo_output'])

     def execute(self, userdata):
        # Do something with userdata
        if userdata.foo_input == 1:
            return 'outcome1'
        else:
            userdata.foo_output = 3
            return 'outcome2'
```
The input_keys list enumerates all the inputs that a state needs to run. A state declares that it expect these fields to exist in the userdata. The execute method is provided a copy of the userdata struct. The state can read from all userdata fields that it enumerates in the input_keys list, but it can't write to any of these fields.
<br />
The output_keys list enumerates all the outputs that a state provides. The state can write to all fields in the userdata struct that are enumerated in the output_keys list. 
<br />
Note: If we want to run methods on userinput, we will have to mention it in useroutput also since inputkeys are immutable.

### Connecting User Data

```sh
 sm_top = smach.StateMachine(outcomes=['outcome4','outcome5'],
                          input_keys=['sm_input'],
                          output_keys=['sm_output'])
  with sm_top:
     smach.StateMachine.add('FOO', Foo(),
                            transitions={'outcome1':'BAR',
                                         'outcome2':'outcome4'},
                            remapping={'foo_input':'sm_input',
                                       'foo_output':'sm_data'})
     smach.StateMachine.add('BAR', Bar(),
                            transitions={'outcome2':'FOO'},
                            remapping={'bar_input':'sm_data',
                                       'bar_output1':'sm_output'})
 ```
Here sm_data is used to map FOO output to BAR input.<br />
Remapping is used to connect input and output keys of different nodes to each other defining input and output keys for the state machine.
>So when you remap 'x':'y':
>x needs to be an input_key or an output_key of the state.
>y will automatically become part of the userdata of the state machine. 

 ![alt text]( http://wiki.ros.org/smach/Tutorials/User%20Data?action=AttachFile&do=get&target=user_data.png "Above Mapping")
 
## Create a Hierarchical State Machine [(Tutorial)](http://wiki.ros.org/smach/Tutorials/Create%20a%20hierarchical%20state%20machine)

```sh
# Create the top level SMACH state machine
    sm_top = smach.StateMachine(outcomes=['outcome5'])

    # Open the container
    with sm_top:

        # Create the sub SMACH state machine 
        sm_sub = smach.StateMachine(outcomes=['outcome4'])

        # Open the container 
        with sm_sub:
```
This is how we create sub machines in a machine and create hierarchy among the machines.

```sh
smach.StateMachine.add('SUB', sm_sub, transitions={'outcome4':'outcome5'})
```
To add one statemachine to another.

## Calling Actions from a State Machine [(Tutorial)](http://wiki.ros.org/smach/Tutorials/Calling%20Actions)

Library to include :
>from smach_ros import SimpleActionState
The possible outcomes of the simple action state are 'succeeded', 'preempted' and 'aborted'.

ActionState without any goal :

```sh
smach.StateMachine.add('State_name',
                           SimpleActionState('action_server_namespace',
                                             action_name),
                           transitions={'succeeded':'state_on_success'})
```
ActionState with a goal :

```sh
    _goal = required_params
    StateMachine.add('state_name',
                      SimpleActionState('action_server_namespace',
                                        action_name,
                                        goal=_goal),
                      transitions={'succeeded':'state_on_success'})
``` 
If we already userdate required for goal at different keys, the we can use goal_slots and assign it some keys and the remap them to the requires userdate. Example :
```sh
>goal_slots=['max_effort','position']
>remapping={'max_effort':'user_data_max','position':'user_data_position'}
```
Note : Similar methods can be applied for feedback and results also.




   
   
   
   
   
   
   
   
 
 
 
 

