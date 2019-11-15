#include <ros/ros.h>
#include <actionlib/server/simple_action_server.h>
#include <NewtonRaphson/NewtonRaphson2Action.h>
#include <iostream>

#define A 1
#define B 0
#define C -5
#define D 13



class NewtonRaphson2Action
{
protected:

  ros::NodeHandle nh_;
  actionlib::SimpleActionServer<NewtonRaphson::NewtonRaphson2Action> as_; // NodeHandle instance must be created before this line. Otherwise strange error occurs.
  std::string action_name_;
  // create messages that are used to published feedback/result
  NewtonRaphson::NewtonRaphson2Feedback feedback_;
  NewtonRaphson::NewtonRaphson2Result result_;

public:

  NewtonRaphson2Action(std::string name) :
    as_(nh_, name, boost::bind(&NewtonRaphson2Action::executeCB, this, _1), false),
    action_name_(name)
  {
    as_.start();
  }

 ~NewtonRaphson2Action(void)
  {
  }

  void executeCB(const NewtonRaphson::NewtonRaphson2GoalConstPtr &goal)
  {
    // helper variables
    ros::Rate r(3);
    bool success = true;


    ROS_INFO("%s: Executing, implementing NewtonRaphson method with input %lf ", action_name_.c_str(), goal->x);

    double x = goal->x;

    double f_x = A*x*x*x - 5*x + 13 ;

    double der_f_x = 3*x*x - 5 ;

    double h = f_x / der_f_x ;

    feedback_.h = h ;
    as_.publishFeedback(feedback_);

    r.sleep();


    if(success)
    {
      result_.h = feedback_.h;
      ROS_INFO("%s: Succeeded", action_name_.c_str());
      // set the action state to succeeded
      as_.setSucceeded(result_);
    }
   }

 };












 int main(int argc, char** argv)
{
  ros::init(argc, argv, "newton_raphson_2");


  NewtonRaphson2Action newton_raphson_2("newton_raphson_2");
  ros::spin();

  return 0;
}