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

from turtlesim.msg import Pose

turtle1_x = 0
turtle1_y = 0


def main():
    rospy.init_node('smach_usecase_executive')

    sm = smach.StateMachine(outcomes=['succeeded','aborted','preempted'])

    with sm:
    	
    	smach.StateMachine.add('RESET', ServiceState('reset', std_srvs.srv.Empty), transitions = {'succeeded':'SPAWN'})
        smach.StateMachine.add('SPAWN', ServiceState('spawn', turtlesim.srv.Spawn,request = turtlesim.srv.SpawnRequest(0.0, 0.0, 0.0, 'turtle2')), transitions = {'succeeded':'TELEPORT1'})
    	smach.StateMachine.add('TELEPORT1', ServiceState('turtle1/teleport_absolute', turtlesim.srv.TeleportAbsolute, request = turtlesim.srv.TeleportAbsoluteRequest(5.0, 1.0, 0.0)), transitions = {'succeeded':'TELEPORT2'})
    	smach.StateMachine.add('TELEPORT2', ServiceState('turtle2/teleport_absolute', turtlesim.srv.TeleportAbsolute, request = turtlesim.srv.TeleportAbsoluteRequest(9.0, 5.0, 0.0)), transitions = {'succeeded':'DRAW_SHAPES'})

        sm_con = smach.Concurrence(outcomes=['succeeded','aborted', 'preempted'],
                                   default_outcome='aborted',
                                   outcome_map={'succeeded':
                                       { 'BIG':'succeeded',
                                         'SMALL':'succeeded'}})

        with sm_con:

            smach.Concurrence.add('BIG', smach_ros.SimpleActionState('turtle_shape1', turtle_actionlib.msg.ShapeAction, goal = turtle_actionlib.msg.ShapeGoal(edges = 11, radius = 4.0)))
 #           smach.Concurrence.add('SMALL', smach_ros.SimpleActionState('turtle_shape2', turtle_actionlib.msg.ShapeAction, goal = turtle_actionlib.msg.ShapeGoal(edges = 6, radius = 2.0)))


            def child_term_cb(outcome_map):

                    if outcome_map['MONITOR'] == 'invalid':
                        return True

                    if outcome_map['DRAW'] == 'succeeded':
                        return True

                    return False


            sm_small = smach.Concurrence(outcomes=['succeeded','aborted', 'preempted'],
                                   default_outcome='aborted',
                                   outcome_map={'succeeded':{ 'DRAW':'succeeded'}, 'preempted':{'DRAW':'preempted', 'MONITOR':'preempted'}, 'aborted':{'MONITOR':'invalid'}},
                                   child_termination_cb = child_term_cb)

            with sm_small:

                        

                smach.Concurrence.add('DRAW', smach_ros.SimpleActionState('turtle_shape2', turtle_actionlib.msg.ShapeAction, goal = turtle_actionlib.msg.ShapeGoal(edges = 6, radius = 2.0)))

                

                def call(msg):

                    global turtle1_x
                    global turtle1_y
                    
                    turtle1_x = msg.x
                    turtle1_y = msg.y



                def monitor_cb(ud, msg):

                #    rospy.init_node('monitor_cb', anonymous=True)

                    rospy.Subscriber("turtle1/pose", Pose, call)

                    dist = (msg.x - turtle1_x)*(msg.x - turtle1_x) + (msg.y - turtle1_y)*(msg.y - turtle1_y)

                    if  dist < 2 :
                        return False

                    return True

                smach.Concurrence.add('MONITOR', smach_ros.MonitorState("turtle2/pose", Pose, monitor_cb))

                




            smach.Concurrence.add('SMALL', sm_small)


        


        smach.StateMachine.add('DRAW_SHAPES', sm_con, transitions = {'succeeded':'succeeded', 'aborted':'aborted', 'preempted':'preempted'})







    # Create and start the introspection server
	ser = smach_ros.IntrospectionServer('server_name', sm, '/SM_ROOT')
	ser.start()

	# Execute the state machine
	outcome = sm.execute()

    rospy.spin()

if __name__ == '__main__':
    main()
