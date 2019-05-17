#!/usr/bin/env python

import roslib; roslib.load_manifest('smach_usecase')
from smach_ros import ServiceState
import turtlesim.srv

import rospy
import smach

import std_srvs
from std_srvs.srv import Empty
import smach_ros

import turtle_actionlib.msg

from smach_tutorials.msg import TestAction, TestGoal
from actionlib import *
from actionlib_msgs.msg import *

def main():
    rospy.init_node('smach_usecase_executive')

    sm = smach.StateMachine(outcomes=['succeeded','aborted','preempted'])

    with sm:
    	
    	smach.StateMachine.add('RESET', ServiceState('reset', std_srvs.srv.Empty), transitions = {'succeeded':'SPAWN'})
        smach.StateMachine.add('SPAWN', ServiceState('spawn', turtlesim.srv.Spawn,request = turtlesim.srv.SpawnRequest(0.0, 0.0, 0.0, 'turtle2')), transitions = {'succeeded':'TELEPORT1'})
    	smach.StateMachine.add('TELEPORT1', ServiceState('turtle1/teleport_absolute', turtlesim.srv.TeleportAbsolute, request = turtlesim.srv.TeleportAbsoluteRequest(5.0, 1.0, 0.0)), transitions = {'succeeded':'TELEPORT2'})
    	smach.StateMachine.add('TELEPORT2', ServiceState('turtle2/teleport_absolute', turtlesim.srv.TeleportAbsolute, request = turtlesim.srv.TeleportAbsoluteRequest(9.0, 5.0, 0.0)), transitions = {'succeeded':'BIG'})



    	smach.StateMachine.add('BIG', smach_ros.SimpleActionState('turtle_shape1', turtle_actionlib.msg.ShapeAction, goal = turtle_actionlib.msg.ShapeGoal(edges = 11, radius = 4.0)), transitions = {'succeeded':'SMALL'})
    	smach.StateMachine.add('SMALL', smach_ros.SimpleActionState('turtle_shape2', turtle_actionlib.msg.ShapeAction, goal = turtle_actionlib.msg.ShapeGoal(edges = 6, radius = 2.0)), transitions = {'succeeded':'succeeded', 'aborted':'aborted', 'preempted':'preempted'})




    # Create and start the introspection server
	ser = smach_ros.IntrospectionServer('server_name', sm, '/SM_ROOT')
	ser.start()

	# Execute the state machine
	outcome = sm.execute()

    rospy.spin()

if __name__ == '__main__':
    main()
