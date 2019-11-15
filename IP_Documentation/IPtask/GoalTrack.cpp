#include <opencv2/core/core.hpp>
 
#include <opencv2/highgui/highgui.hpp>
 
#include <opencv2/imgproc/imgproc.hpp>
 
#include <opencv2/video/video.hpp>

#include <opencv2/video/tracking.hpp>
 
#include <iostream>
 
#include <vector>
 
using namespace std;
using namespace cv;
 

#define Max_H_Black 60
 
 
int main()
{

    Mat frame;
 
    // Kalman Filter
    int stateSize = 6; // 2-d position and velocity and width and height of the goapl post
    int measSize = 4;  // 2-d position and width and height
    int contrSize = 0;
 
    KalmanFilter kf(stateSize, measSize, contrSize, CV_32F);
 
    Mat state(stateSize, 1, CV_32F);  // [x,y,v_x,v_y,w,h]
    Mat meas(measSize, 1, CV_32F);    // [x,y,w,h]
    
 
    // Transition State Matrix A
    
    // T is time between the measurements / updates 
    
    
    // [ 1 0 T 0  0 0 ]    x = x + v_x*T
    // [ 0 1 0  T 0 0 ]    y = y + v_y*T
    
   
    setIdentity(kf.transitionMatrix); // A is set to I (Identity Matrix ) .... T is updated later 

    // No Matrix B as acceleration is assumed to be 0 

    // Nosie in State Determination is also excluded
 
    // Measure Matrix H
    
    kf.measurementMatrix = Mat::zeros(measSize, stateSize, CV_32F);
   
    kf.measurementMatrix.at<float>(0) = 1.0f;   // [ 1 0 0 0 0 0 ]
    kf.measurementMatrix.at<float>(7) = 1.0f;   // [ 0 1 0 0 0 0 ]
    kf.measurementMatrix.at<float>(16) = 1.0f;  // [ 0 0 0 0 1 0 ]
    kf.measurementMatrix.at<float>(23) = 1.0f;  // [ 0 0 0 0 0 1 ]
 
    // Process Noise Covariance Matrix Q
    
    setIdentity(kf.processNoiseCov, cv::Scalar(1e-2)); // Default for Error in x y width and height
 
    kf.processNoiseCov.at<float>(14) = 5.0f; // Updating error for velocity
    kf.processNoiseCov.at<float>(21) = 5.0f;

    // Measures Noise Covariance Matrix R
    setIdentity(kf.measurementNoiseCov,Scalar(1e-1)); //Default Settings

    // Loading Video

    VideoCapture cap("3.avi");

    cap >> frame;

    VideoWriter video("output_temp.avi",CV_FOURCC('M','J','P','G'),60, Size(frame.cols,frame.rows));
 
    // Error in File Loading
    
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
 
    // Video loop
    
    while (ch != 'x')
    {
        double precTick = ticks;
        ticks = (double) cv::getTickCount(); 
 
        double T = (ticks - precTick) / cv::getTickFrequency(); //Current Ticks count - Precious Ticks Count Divided by Frequency
 
        cap >> frame;
 
        Mat res;
        frame.copyTo( res );
 
        if (found)
        {
            // Matrix A
            // Updating the time from measurement to measurement
           
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

            stringstream sstr;                                   // Display Estimated KF goal center
            sstr << "(" << center.x << "," << center.y << ")";
            putText(res, sstr.str(),
                        Point(center.x + 3, center.y - 3),
                        FONT_HERSHEY_SIMPLEX, 0.5, CV_RGB(255,255,255), 2);
        }
 
        //  Smoothening the Image
        
        Mat blur;
        GaussianBlur(frame, blur, cv::Size(5, 5), 3.0, 3.0);
        
 
        //  HSV conversion
        // Required colours can be easily filtered from HSV format

        Mat frmHsv;
        cvtColor(blur, frmHsv, CV_BGR2HSV);
        
 
        // Color Thresholding
        
        Mat rangeRes = Mat::zeros(frame.size(), CV_8UC1);
        inRange(frmHsv, cv::Scalar(0,0,0),
                    cv::Scalar(180, 255, Max_H_Black), rangeRes);
       

 
        // Improving the result
        
        erode(rangeRes, rangeRes, Mat(), Point(-1, -1), 2);
        dilate(rangeRes, rangeRes, Mat(), Point(-1, -1), 2);
        
 
        // Thresholding viewing
        imshow("Threshold", rangeRes);
 
        // Contours detection
        vector<vector<cv::Point> > contours;
        findContours(rangeRes, contours, CV_RETR_EXTERNAL,
                         CV_CHAIN_APPROX_NONE);
       
 
        // Filtering
        vector<vector<cv::Point> > Goals;
        vector<cv::Rect> Goals_Box;
        for (size_t i = 0; i < contours.size(); i++)
        {
            Rect gBox;
            gBox = boundingRect(contours[i]); // To get the Bounding Rectangle for the detected contours
 
            float ratio = (float) gBox.width / (float) gBox.height;
           
 
            // Searching for a goal Box almost square , shape of the goal post
            
            if (ratio >= 0.8 && ratio <= 1.25 && gBox.area() >= 10000) // the squar should be sufficiently large
            {
                Goals.push_back(contours[i]);
                Goals_Box.push_back(gBox);
            }
        }
        
 
        cout << "Goals found:" << Goals_Box.size() << endl;
 
        // Detection result
        for (size_t i = 0; i < Goals.size(); i++)
        {
            drawContours(res, Goals, i, CV_RGB(255,0,0), 1);
            rectangle(res, Goals_Box[i], CV_RGB(0,255,0), 2);
 
            Point center;
            center.x = Goals_Box[i].x + Goals_Box[i].width / 2;
            center.y = Goals_Box[i].y + Goals_Box[i].height / 2;
            circle(res, center, 2, CV_RGB(0,150,0), -1);
 
            stringstream sstr;
            sstr << "(" << center.x << "," << center.y << ")";
   //       putText(res, sstr.str(), Point(center.x + 3, center.y - 3),FONT_HERSHEY_SIMPLEX, 0.5, CV_RGB(0,150,0), 2);
   // Uncomment the above line to print center of Measured Goal on output     
        }

 
        //  Kalman Update
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
 
            if (!found) // First detection
            {
                // Initialization
                kf.errorCovPre.at<float>(0) = 1; // px
                kf.errorCovPre.at<float>(7) = 1; // px
                kf.errorCovPre.at<float>(14) = 1;
                kf.errorCovPre.at<float>(21) = 1;
                kf.errorCovPre.at<float>(28) = 1; // px
                kf.errorCovPre.at<float>(35) = 1; // px
 
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
                kf.correct(meas); // Kalman Correction
 
            cout << "Measure matrix:" << endl << meas << endl;
        }
        
 
        imshow("Tracking", res);
        video.write(res);

        
        ch = waitKey(25);
    }


    cap.release();
    video.release();

    destroyAllWindows();
    return 0;
}   