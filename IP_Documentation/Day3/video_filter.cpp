#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Mat frame;

Mat filter;

int thrl=210;
int thru=250;
int thgl=170;
int thgu=210;
int thbl=150;
int thbu=200;

String windowName;

void tkbfn() {
        int i=0,j=0;
        for (i=0;i<frame.rows;i++) {
                for(j=0;j<frame.cols;j++) {
                        if ((frame.at<Vec3b>(i,j)[2]<thrl || frame.at<Vec3b>(i,j)[2]>thru) || (frame.at<Vec3b>(i,j)[1]<thgl || frame.at<Vec3b>(i,j)[1]>thgu) || (frame.at<Vec3b>(i,j)[0]<thbl || frame.at<Vec3b>(i,j)[0]>thbu)) {
                                filter.at<Vec3b>(i,j)[2]=0;
                                filter.at<Vec3b>(i,j)[1]=0;
                                filter.at<Vec3b>(i,j)[0]=0;
                        }
                        else {
                                filter.at<Vec3b>(i,j)[2]=frame.at<Vec3b>(i,j)[2];
                                filter.at<Vec3b>(i,j)[1]=frame.at<Vec3b>(i,j)[1];
                                filter.at<Vec3b>(i,j)[0]=frame.at<Vec3b>(i,j)[0];

                        }
                }
        }


        imshow(windowName,filter);
        return;
}

int main(int, char**)
{
    VideoCapture cap("/home/amshumaan/OpenCV/files/PS/AH.mp4");
    
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    windowName="Filtered Output";
    namedWindow(windowName,WINDOW_NORMAL);
    for(;;)
    {
        cap >> frame; // get a new frame from camera

        filter.create(frame.size(),frame.type());

        tkbfn();

        imshow(windowName,filter);



        createTrackbar( "RedU", windowName , &thru , 255 , tkbfn );
        createTrackbar( "RedL", windowName , &thrl , 255 , tkbfn );
        createTrackbar( "GreenU", windowName , &thgu , 255 , tkbfn );
        createTrackbar( "GreenL", windowName , &thgl , 255 , tkbfn );
        createTrackbar( "BlueU", windowName , &thbu , 255 , tkbfn );
        createTrackbar( "BlueL", windowName , &thbl , 255 , tkbfn );    

        if(waitKey(2000) >= 0) break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
