#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Mat img=imread("/home/amshumaan/OpenCV/images/guitar.jpeg",0);
Mat	img1(img.rows,img.cols,CV_8UC1, Scalar(0));

int th;

String windowName;

void tkbfn(int , void*) {
		int i=0,j=0;
		for (i=0;i<img.rows;i++) {
				for(j=0;j<img.cols;j++) {
						if (img.at<uchar>(i,j)<th) img1.at<uchar>(i,j)=0;
						else img1.at<uchar>(i,j)=255;
				}
		}

		imshow(windowName,img1);

		return;
}

int main(int argc, char** argv) {

	windowName = "TrackBar"; //Name of the window

 	namedWindow(windowName,WINDOW_NORMAL); // Create a window

 	 createTrackbar( "Trackbar", windowName , &th , 255 , tkbfn );

	waitKey(0); // Wait for any keystroke in the window

 	destroyWindow(windowName); //destroy the created window

 return 0;
}
