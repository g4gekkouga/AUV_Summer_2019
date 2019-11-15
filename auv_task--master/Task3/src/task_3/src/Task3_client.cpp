#include <ros/ros.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include <task_3/Task3Action.h>

#include <opencv2/core/core.hpp>
 
#include <opencv2/highgui/highgui.hpp>
 
#include <opencv2/imgproc/imgproc.hpp>
 
#include <opencv2/video/video.hpp>
 
#include <iostream>
 
#include <vector>





using namespace std;
using namespace cv;
 

#define Max_H_Black 60



int main (int argc, char **argv)
{
  ros::init(argc, argv, "test_task3");

  // create the action client
  // true causes the client to spin its own thread
  actionlib::SimpleActionClient<task_3::Task3Action> ac("task3", true);

  ROS_INFO("Waiting for action server to start.");
  // wait for the action server to start
  ac.waitForServer(); //will wait for infinite time

  ROS_INFO("Action server started, sending goal.");
  // send a goal to the action
  
  task_3::Task3Goal goal; // creating goal object to pass to the serevr   


   
  // Same as Task 2 code with extra lines of code to define and pass Goal where ever commented



   Mat frame;
 
    
    int stateSize = 6; 
    int measSize = 4;  
    int contrSize = 0;
 
    KalmanFilter kf(stateSize, measSize, contrSize, CV_32F);
 
    Mat state(stateSize, 1, CV_32F);  
    Mat meas(measSize, 1, CV_32F);    
    

   
    setIdentity(kf.transitionMatrix); 

   
    
    kf.measurementMatrix = Mat::zeros(measSize, stateSize, CV_32F);
   
    kf.measurementMatrix.at<float>(0) = 1.0f;   
    kf.measurementMatrix.at<float>(7) = 1.0f;   
    kf.measurementMatrix.at<float>(16) = 1.0f;  
    kf.measurementMatrix.at<float>(23) = 1.0f;  
 
    
    
    setIdentity(kf.processNoiseCov, cv::Scalar(1e-2)); 
 
    kf.processNoiseCov.at<float>(14) = 5.0f; 
    kf.processNoiseCov.at<float>(21) = 5.0f;

   
    setIdentity(kf.measurementNoiseCov,Scalar(1e-1)); 


    VideoCapture cap("/home/amshumaan/auv_task/iptask/3.avi");

    cap >> frame;

    
    if (!cap.isOpened())
    {
        cout << "Error in loading the video\n";
        return -1;
    }
 
    cout << "\nHit 'x' to exit...\n";
 
    char ch = 'a';
 
    double ticks = 0;
    
    int found = 0;
 
    int notFoundCount = 0;
 
    
    while (ch != 'x')
    {
        double precTick = ticks;
        ticks = (double) cv::getTickCount(); 
 
        double T = (ticks - precTick) / cv::getTickFrequency();
 
        cap >> frame;

        // To pass the center of the frame to the server through the goal object

        goal.frame_center_x = frame.cols / 2 ; 
        goal.frame_center_y = frame.rows / 2 ;
 
        Mat res;
        frame.copyTo( res );
 
        if (found)
        {
            
           
            kf.transitionMatrix.at<float>(2) = T;
            kf.transitionMatrix.at<float>(9) = T;
            
 
            cout << "T:" << endl << T << endl;
 
            state = kf.predict();
            cout << "State post:" << endl << state << endl;
 
            Rect predRect;
            predRect.width = state.at<float>(4);
            predRect.height = state.at<float>(5);
            predRect.x = state.at<float>(0) - predRect.width / 2;
            predRect.y = state.at<float>(1) - predRect.height / 2;
 
            Point center;
            center.x = state.at<float>(0);
            center.y = state.at<float>(1);
            circle(res, center, 2, CV_RGB(255,255,255), -1);
 
            rectangle(res, predRect, CV_RGB(255,255,255), 2);

            stringstream sstr;                                   
            sstr << "(" << center.x << "," << center.y << ")";
            putText(res, sstr.str(),
                        Point(center.x + 3, center.y - 3),
                        FONT_HERSHEY_SIMPLEX, 0.5, CV_RGB(255,255,255), 2);


            // To pass the present center coordinates of the kalman estimate

            goal.KF_center_x = center.x ;
            goal.KF_center_y = center.y ;

            //Passing the goal to the server

            ac.sendGoal(goal);
        }

        else {
        	// If no Kalman Filter is detected , we pass (0,0) 

            goal.KF_center_x = 0 ;
            goal.KF_center_y = 0 ;

            //Passing the goal to the server

            ac.sendGoal(goal);


        }
 
        
        
        Mat blur;
        GaussianBlur(frame, blur, cv::Size(5, 5), 3.0, 3.0);
        
 
        

        Mat frmHsv;
        cvtColor(blur, frmHsv, CV_BGR2HSV);
        
 
        
        Mat rangeRes = Mat::zeros(frame.size(), CV_8UC1);
        inRange(frmHsv, cv::Scalar(0,0,0),
                    cv::Scalar(180, 255, Max_H_Black), rangeRes);
       

 
      
        
        erode(rangeRes, rangeRes, Mat(), Point(-1, -1), 2);
        dilate(rangeRes, rangeRes, Mat(), Point(-1, -1), 2);
        
 
        imshow("Threshold", rangeRes);
 
        vector<vector<cv::Point> > contours;
        findContours(rangeRes, contours, CV_RETR_EXTERNAL,
                         CV_CHAIN_APPROX_NONE);
       
 
        
        vector<vector<cv::Point> > Goals;
        vector<cv::Rect> Goals_Box;
        for (size_t i = 0; i < contours.size(); i++)
        {
            Rect gBox;
            gBox = boundingRect(contours[i]); 
 
            float ratio = (float) gBox.width / (float) gBox.height;
           
 
            
            
            if (ratio >= 0.8 && ratio <= 1.25 && gBox.area() >= 10000) 
            {
                Goals.push_back(contours[i]);
                Goals_Box.push_back(gBox);
            }
        }
        
 
        cout << "Goals found:" << Goals_Box.size() << endl;
 
        
        for (size_t i = 0; i < Goals.size(); i++)
        {
   //         drawContours(res, Goals, i, CV_RGB(255,0,0), 1);
   //         rectangle(res, Goals_Box[i], CV_RGB(0,255,0), 2);
 
            Point center;
            center.x = Goals_Box[i].x + Goals_Box[i].width / 2;
            center.y = Goals_Box[i].y + Goals_Box[i].height / 2;
            circle(res, center, 2, CV_RGB(0,150,0), -1);
 
            stringstream sstr;
            sstr << "(" << center.x << "," << center.y << ")";
  
        }

 
      
        if (Goals.size() == 0)
        {
            notFoundCount++;

            if( notFoundCount >= 100 )
            {
                found = 0;
                cout << "Goal not Found for 100 continuous frames " << endl ;
            }
    
        }
        else
        {
            notFoundCount = 0;
 
            meas.at<float>(0) = Goals_Box[0].x + Goals_Box[0].width / 2;
            meas.at<float>(1) = Goals_Box[0].y + Goals_Box[0].height / 2;
            meas.at<float>(2) = (float)Goals_Box[0].width;
            meas.at<float>(3) = (float)Goals_Box[0].height;
 
            if (!found) 
            {
                
                kf.errorCovPre.at<float>(0) = 1;
                kf.errorCovPre.at<float>(7) = 1; 
                kf.errorCovPre.at<float>(14) = 1;
                kf.errorCovPre.at<float>(21) = 1;
                kf.errorCovPre.at<float>(28) = 1; 
                kf.errorCovPre.at<float>(35) = 1;
 
                state.at<float>(0) = meas.at<float>(0);
                state.at<float>(1) = meas.at<float>(1);
                state.at<float>(2) = 0;
                state.at<float>(3) = 0;
                state.at<float>(4) = meas.at<float>(2);
                state.at<float>(5) = meas.at<float>(3);
 
                kf.statePost = state;
                
                found = 1;
            }
            else
                kf.correct(meas); 
 
            cout << "Measure matrix:" << endl << meas << endl;
        }
        
 
        imshow("Tracking", res);
        
        ch = waitKey(100);
    }


    cap.release();

    destroyAllWindows();
  
  


















































  //wait for the action to return
  bool finished_before_timeout = ac.waitForResult(ros::Duration(500.0));

  if (finished_before_timeout)
  {
    actionlib::SimpleClientGoalState state = ac.getState();
    ROS_INFO("Action finished: %s",state.toString().c_str());
  }
  else
    ROS_INFO("Action did not finish before the time out.");

  //exit
  return 0;
}
