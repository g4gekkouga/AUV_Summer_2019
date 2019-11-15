#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Mat img=imread("/home/amshumaan/OpenCV/images/madhav.jpg",1);
Mat	img1(img.rows,img.cols,CV_8UC3, Scalar(0,0,0));

int thr;
int thg;
int thb;

String windowName;

void tkbfn(int , void*) {
		int i=0,j=0;
		for (i=0;i<img.rows;i++) {
				for(j=0;j<img.cols;j++) {
						if (img.at<Vec3b>(i,j)[2]<thr) img1.at<Vec3b>(i,j)[2]=0;
						else img1.at<Vec3b>(i,j)[2]=255;
						if (img.at<Vec3b>(i,j)[1]<thg) img1.at<Vec3b>(i,j)[1]=0;
						else img1.at<Vec3b>(i,j)[1]=255;
						if (img.at<Vec3b>(i,j)[0]<thb) img1.at<Vec3b>(i,j)[0]=0;
						else img1.at<Vec3b>(i,j)[0]=255;
				}
		}

		imshow(windowName,img1);

		return;
}

int main(int argc, char** argv) {

	windowName = "TrackBar"; //Name of the window

 	namedWindow(windowName,WINDOW_NORMAL); // Create a window

 	createTrackbar( "Red", windowName , &thr , 255 , tkbfn );
 	createTrackbar( "Green", windowName , &thg , 255 , tkbfn );
 	createTrackbar( "Blue", windowName , &thb , 255 , tkbfn );

	waitKey(0); // Wait for any keystroke in the window

 	destroyWindow(windowName); //destroy the created window

 return 0;
}