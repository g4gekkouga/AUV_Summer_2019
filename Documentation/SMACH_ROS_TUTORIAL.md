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
<br />
server_name: this name is used to create a namespace for the ROS introspection topics. You can name this anything you like, as long as this name is unique in your system. This name is not shown in the smach viewer.
<br />
SM_ROOT: your state machine will show up under this name in the smach viewer. So you can pretty much choose any name you like. If you have sub-state machines that are in different executables, you can make them show up as hierarchical state machines by choosing this name in a clever way: if the top level state machine is called 'SM_TOP', you can call the sub state machine 'SM_TOP/SM_SUB', and the viewer will recognize the sub state machine as being part of the top state machine. 

#### Note : Smach_Viewer doesnt run in ros kinetic version. Refer this document for making it work. [DOCUMENT](https://gist.github.com/matt3o/88bced95dba37a8932a51904d0734dff)

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

There are optional arguments to the static add method. The signature of the add method is: 
```sh
add(label, state, transitions=None, remapping=None) 
```

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

>goal_slots=['max_effort','position'] <br />
>remapping={'max_effort':'user_data_max','position':'user_data_position'}

Note : Similar methods can be applied for feedback and results also.

The state can also pass goal on callback from action. Refer the aboce tutorial link for sample goal callBack code.

![alt text]( http://wiki.ros.org/smach/Tutorials/Simple%20Action%20State?action=AttachFile&do=get&target=actionstate.png "Simple Action State")

[Simple Action Server](http://wiki.ros.org/smach/Tutorials/Simple%20Action%20State)

### Result to userdata

You can write the result of the action directly to the userdata of your state.  <br />
Example :
>result_slots=['max_effort','position'] <br />
>remapping={'max_effort':'user_data_max','position':'user_data_position'}

Similar to goal callback, there can also be result callback methods in the state.

## Concurrent State Machine [(Link)](http://wiki.ros.org/smach/Tutorials/Concurrent%20States)

This is used to run 2 states parallely or simultaneously. <br />
Example :
```sh
# Create the sub SMACH state machine
        sm_con = smach.Concurrence(outcomes=['outcome4','outcome5'],
                                   default_outcome='outcome4',
                                   outcome_map={'outcome5':
                                       { 'FOO':'outcome2',
                                         'BAR':'outcome1'}})

        # Open the container
        with sm_con:
            # Add states to the container
            smach.Concurrence.add('FOO', Foo())
            smach.Concurrence.add('BAR', Bar())

        smach.StateMachine.add('CON', sm_con,
                               transitions={'outcome4':'CON',
                                            'outcome5':'outcome6'})
```
![alt text]( http://wiki.ros.org/smach/Tutorials/Concurrent%20States?action=AttachFile&do=get&target=concurrence2.png "Concurrent Machine")

Once all the states in the concurrence have terminated, if one of these child-outcome mappings is satisfied, the concurrence will return its associated outcome.<br />If none of the mappings are satisfied, the concurrence will return its default outcome.

### Using Callbacks : [(Link)](http://wiki.ros.org/smach/Tutorials/Concurrence%20container)

If you want full control over a concurrence state machine, you can use the callbacks it provides, the child_termination_cb and the outcome_cb: 

```sh
# gets called when ANY child state terminates
def child_term_cb(outcome_map):

  # terminate all running states if FOO finished with outcome 'outcome3'
  if outcome_map['FOO'] == 'outcome3':
    return True
    
  return False
  
# gets called when ALL child states are terminated
def out_cb(outcome_map):
   if outcome_map['FOO'] == 'succeeded':
      return 'outcome1'
   else:
      return 'outcome2'
      
sm = Concurrence(outcomes=['outcome1', 'outcome2'],
                 default_outcome='outcome1',
                 input_keys=['sm_input'],
                 output_keys=['sm_output'],
                 child_termination_cb = child_term_cb,
                 outcome_cb = out_cb)
```

The child_termination_cb is called every time one of the child states terminates. In the callback function you can decide if the state machine should keep running (return False), or if it should preempt all remaining running states (return True).<br />
The outcome_cb is called once when the last child state terminates. This callback returns the outcome of the concurrence state machine. 

## Sequence container [(Tutorial)](http://wiki.ros.org/smach/Tutorials/Sequence%20container)

The Sequence container is a StateMachine container, extended with auto-generated transitions that create a sequence of states from the order in which said states are added to the container.
<br />
Example :
```sh
sq = Sequence(
        outcomes = ['succeeded','aborted','preempted'],
        connector_outcome = 'succeeded')
with sq:
    Sequence.add('MOVE_ARM_GRAB_PRE', MoveVerticalGripperPoseActionState())
    Sequence.add('MOVE_GRIPPER_OPEN', MoveGripperState(GRIPPER_MAX_WIDTH))
    Sequence.add('MOVE_ARM_GRAB',     MoveVerticalGripperPoseActionState())
    Sequence.add('MOVE_GRIPPER_CLOSE', MoveGripperState(grab_width))
    Sequence.add('MOVE_ARM_GRAB_POST', MoveVerticalGripperPoseActionState())
```

Note : Import the following library.
>from smach import Sequence

Each state added will receive an additional transition from it to the state which is added after it. The transition will follow the outcome specified at construction of this container.
<br />
If one of the transitions given in the dictionary mapping parameter to 'Sequence.add()' follows the connector outcome specified in the constructor, the provided transition will override the automatically generated connector transition.

## Iterator container [(Tutorial)](http://wiki.ros.org/smach/Tutorials/Iterator%20container)

The iterator allows you to loop through a state or states until success conditions are met. This tutorial demonstrates how to use an iterator to sort a list of numbers into evens and odds. 

Note : Import the following library.
>from smach import Iterator

```sh
__init__(self, outcomes, input_keys, output_keys, it=[], it_label='it_data', exhausted_outcome='exhausted')
```

In this example, the outcomes now include preempted, a default outcome of the iterator. The it argument is the list of objects that will be iterated over and the it_label is key that holds the current value of the of the item in the it list. The exhausted argument should be set to the preferred state machine outcome, in this example the iterator outcome is succeeded when the iterator is done looping through the it list.<br />
Example :
```sh
    tutorial_it = Iterator(outcomes = ['succeeded','preempted','aborted'],
                               input_keys = ['nums', 'even_nums', 'odd_nums'],
                               it = lambda: range(0, len(sm.userdata.nums)),
                               output_keys = ['even_nums', 'odd_nums'],
                               it_label = 'index',
                               exhausted_outcome = 'succeeded')
```

Example : <br />
![alt text](http://wiki.ros.org/smach/Tutorials/Iterator%20container?action=AttachFile&do=get&target=smach_tutorial.png "Example of an iterator container")

## Wrapping a Container With actionlib [(Tutorial)](http://wiki.ros.org/smach/Tutorials/Wrapping%20a%20SMACH%20Container%20With%20actionlib)

```sh
# Construct action server wrapper
asw = ActionServerWrapper(
    'my_action_server_name', MyAction,
    wrapped_container = sm,
    succeeded_outcomes = ['did_something','did_something_else'],
    aborted_outcomes = ['aborted'],
    preempted_outcomes = ['preempted'] 
    goal_key = 'my_awesome_goal',
    result_key = 'egad_its_a_result'
    )
```
The above is the general code for wrapping a state machine with Action Wrapper and to call the goal and result.
<br />
The keyword arguments goal_key and result_key are the SMACH userdata keys in the context of the ActionServerWrapper. 
<br />
In order to copy in the keys form the parent, you can replace the construction call for the state machine sm with this: 
```sh
# Construct state machine
sm = StateMachine(
        outcomes=['did_something','did_something_else','aborted','preempted'],
        input_keys = ['my_awesome_goal'],
        output_keys = ['egad_its_a_result'])
```
## CBState : [(Tutorial)](http://wiki.ros.org/smach/Tutorials/CBState)

Library to import :
>from smach import CBState
This state simply executes a single callback when the state is executed. This is useful for executing arbitrary code in a state, without having to declare a new state class. This class supports the use of the smach.cb_interface decorator.<br />
The CBState calls the callback with at least one argument: the container's userdata. Additional arguments and keyword arguments can be given to the CBState on construction. These args will be passed into the callback when the CBState is executed. <br />
Example :
```sh
StateMachine.add('MY_CB', CBState(my_cb,
                                      cb_args=[10],
                                      cb_kwargs={'z':2,'y':3}),
                              {'foo':'OTHER_STATE'})
```

## Monitor State : [(Tutorial)](http://wiki.ros.org/smach/Tutorials/MonitorState)

Used if have to move frm one state to another by accessig the value published to a specific topic.

Example :
```sh
    sm = smach.StateMachine(outcomes=['DONE'])
    with sm:
        smach.StateMachine.add('FOO', 
                               smach_ros.MonitorState("/sm_reset", 
                                                      Empty, 
                                                      monitor_cb), 
                               transitions={'invalid':'BAR', 
                                            'valid':'FOO', 
                                            'preempted':'FOO'})
```
Note that your MonitorState needs to define transitions for all three possible outcomes ('valid', 'invalid', 'preempted'). 

## State Preemption Implementation :

Sometimes, no state or container type will not satisfy your needs. Before you decide this, make sure you have examined all the provided state and container types. There are three key features that need to be considered when defining a new type of SMACH state:

    Interface declaration
    Execution implementation
    Preemption implementation 
    
#### SMACH Interface

SMACH containers interact with their contained states through state outcomes and userdata keys. The outcomes need to be declared in order to be able to be bound to targets in a container, and the userdata keys need to be declared in order to trace the dataflow in the system. This explicit declaration allows us to catch errors on construction of a SMACH tree instead as well as while we're executing it.

#### Execution Implementation

When a state is entered by the execution of some container, its execute() method is called. This is a method that blocks and returns some (registered) outcome identifier (string). Once this method exits, the state should be considered dormant, or inactive.

If a certain state corresponds to a long-running task, it should be brought up in a concurrence or some other parallel container, instead of spinning off a thread and exiting quickly.

#### Preemption Implementation

In simple cases like the example above, preemption can be done simply by checking the method preempt_requested(). If this cannot be done, the request_preempt() method can be overloaded in this new state class. Preempts will usually come in on a separate thread, and this method should not block longer than it takes to notify any child threads / processes to preempt. 

### Refer this link for State Machine Preemption.[LINK](http://wiki.ros.org/smach/Tutorials/State%20Machine%20Preemption%20with%20MonitorState)


## Writing Custom State Classes with User-defined Callbacks [(Tutorial)](http://wiki.ros.org/mysmach/Tutorials/Writing%20Custom%20State%20Classes%20With%20User-defined%20Callbacks)

Sometimes you want to create a state class like smach.SimpleActionState which receives user callbacks as arguments. If these callbacks change the SMACH interface of a state (outcomes, userdata keys), then the callbacks need to be annotated with this information. <br />

SMACH provides a decorator which can attach a SMACH interface to a function. This includes user data input keys, output keys, and state outcomes. A common pattern for extensible state design is to use callback functions that are called on various events. For example, SimpleActionState can call callbacks for generating goal messages or processing result messages, and MonitorState calls a callback each time a message comes in on a specified topic.
<br />
If a state defines an interface that can take callback functions, it should support this decorator model. Doing so is as simple as checking for the existence of the SMACH interface methods. A single function is provided bu the smach.util module. 

#### Single User Callback 
Example :
```sh
class MyState(smach.State):
    def __init__(self, cb):
        smach.State.__init__(self, outcomes=['done'])

        self._cb = cb
        if cb and smach.has_smach_interface(cb):
            self.register_input_keys(cb.get_registered_input_keys())
            self.register_output_keys(cb.get_registered_output_keys())
            self.register_outcomes(cb.get_registered_outcomes())
    ...
    def execute(self, ud):
        # Call callback
        cb_outcome = self._cb(ud)

        # If callback returned an outcome, return it as the state outcome
        if cb_outcome:
            return cb_outcome 
        
        return 'done'
```

#### Multiple User Callbacks

Example :
```sh
class MyState(smach.State):
    def __init__(self, cb_list):
        smach.State.__init__(self, outcomes=['done'])

        self._cbs = cb_list
        for cb in cb_list:
            if cb and smach.has_smach_interface(cb):
                self._cb_input_keys.append(cb.get_registered_input_keys())
                self._cb_output_keys.append(cb.get_registered_output_keys())
 
                self.register_input_keys(self._cb_input_keys[-1])
                self.register_output_keys(self._cb_output_keys[-1])
                self.register_outcomes(self._cb_input_keys[-1])
    ...
    def execute(self, ud):
        # Call callbacks
        for (cb, ik, ok) in zip(self._cbs,
                                self._cb_input_keys,
                                self._cb_output_keys):

            # Call callback with limited userdata
            cb_outcome = self._cb(smach.Remapper(ud,ik,ok,{}))

            # If callback returned an outcome, return it as the state outcome
            if cb_outcome:
                return cb_outcome 
        
        return 'done'
```

If more than one callback is supplied, the state class implementer should ensure that no side-effects are caused by adding callbacks with different interfaces. This is done by using a userdata remapper when passing userdata to the callbacks.
<br />




























   
   
   
   
   
   
   
   
 
 
 
 

