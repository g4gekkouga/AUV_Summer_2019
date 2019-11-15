#include <ros/ros.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include <NewtonRaphson/NewtonRaphson2Action.h>
#include <iostream>

#define INITIAL_GUESS 50

double estimate ;
double h;

using namespace NewtonRaphson;
using namespace std;

typedef actionlib::SimpleActionClient<NewtonRaphson2Action> Client;

// Called once when the goal completes


void doneCb(const actionlib::SimpleClientGoalState& state,
            const NewtonRaphson2ResultConstPtr& result)
{
  
  ROS_INFO("Finished in state [%s]", state.toString().c_str());
  ROS_INFO("Answer: %f", result->h);
  
}

// Called once when the goal becomes active
void activeCb()
{
  ROS_INFO("Goal just went active");
}

// Called every time feedback is received for the goal
void feedbackCb(const NewtonRaphson2FeedbackConstPtr& feedback)
{
  ROS_INFO("Got Feedback of length %f", feedback->h);
  h = feedback->h ;


}











int main (int argc, char **argv)
{
  ros::init(argc, argv, "test_newton_raphson_2");

  // Create the action client
  Client ac("newton_raphson_2", true);

  ROS_INFO("Waiting for action server to start.");
  ac.waitForServer();
  ROS_INFO("Action server started, sending goal.");

  // Send Goal
  
  NewtonRaphson2Goal goal;

  goal.x = INITIAL_GUESS ;

  h = 1 ;

  while ( abs(h) >= 0.000001 ) {
  
  goal.x = goal.x - h;
  
  ac.sendGoal(goal, &doneCb, &activeCb, &feedbackCb);

  //wait for the action to return
  bool finished_before_timeout = ac.waitForResult(ros::Duration(5.0));

  if (finished_before_timeout)
  {
    actionlib::SimpleClientGoalState state = ac.getState();
    ROS_INFO("Action finished: %s",state.toString().c_str());
  }
  else
    ROS_INFO("Action did not finish before the time out.");

  }

  ROS_INFO("Final root is  %f ", goal.x);
  
  return 0;
}