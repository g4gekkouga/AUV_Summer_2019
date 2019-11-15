#include <ros/ros.h>
#include <actionlib/server/simple_action_server.h>
#include <NewtonRaphson/NewtonRaphsonAction.h>
#include <iostream>

#define A 1
#define B 0
#define C -5
#define D 13


class NewtonRaphsonAction
{
protected:

  ros::NodeHandle nh_;
  actionlib::SimpleActionServer<NewtonRaphson::NewtonRaphsonAction> as_; // NodeHandle instance must be created before this line. Otherwise strange error occurs.
  std::string action_name_;
  // create messages that are used to published feedback/result
  NewtonRaphson::NewtonRaphsonFeedback feedback_;
  NewtonRaphson::NewtonRaphsonResult result_;

public:

  NewtonRaphsonAction(std::string name) :
    as_(nh_, name, boost::bind(&NewtonRaphsonAction::executeCB, this, _1), false),
    action_name_(name)
  {
    as_.start();
  }
 ~NewtonRaphsonAction(void)
  {
  }

  void executeCB(const NewtonRaphson::NewtonRaphsonGoalConstPtr &goal)
  {
  	// helper variables
    ros::Rate r(3);
    bool success = true;

    feedback_.estimation_sequence.clear();
    feedback_.estimation_sequence.push_back(goal->initial_guess);


    ROS_INFO("%s: Executing, implementing NewtonRaphson method with initial guess %f ", action_name_.c_str(), goal->initial_guess);

    double x = goal->initial_guess;

    double f_x = A*x*x*x - 5*x + 13 ;

    double der_f_x = 3*x*x - 5 ;

    double h = f_x / der_f_x ;

    int i = 0;

    while (abs(h) >= 0.000001) {
    	// check that preempt has not been requested by the client
      if (as_.isPreemptRequested() || !ros::ok())
      {
        ROS_INFO("%s: Preempted", action_name_.c_str());
        // set the action state to preempted
        as_.setPreempted();
        success = false;
        break;
      }

    	feedback_.estimation_sequence.push_back(x-h) ;
    	as_.publishFeedback(feedback_);
    	i++ ;

    	x = feedback_.estimation_sequence[i] ;
    	f_x = A*x*x*x + B*x*x + C*x + D ;
    	der_f_x = 3*A*x*x + 2*B*x + C ;
    	h = f_x / der_f_x ;
    	r.sleep();
    }

    if(success)
    {
      result_.estimated_root = feedback_.estimation_sequence[i];
      ROS_INFO("%s: Succeeded", action_name_.c_str());
      // set the action state to succeeded
      as_.setSucceeded(result_);
    }
   }

 };












 int main(int argc, char** argv)
{
  ros::init(argc, argv, "newton_raphson");

  NewtonRaphsonAction newton_raphson("newton_raphson");
  ros::spin();

  return 0;
}