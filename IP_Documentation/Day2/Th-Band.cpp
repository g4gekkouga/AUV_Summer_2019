#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Mat img=imread("/home/amshumaan/OpenCV/images/parrot.jpg",1);

Mat	img1(img.rows,img.cols,CV_8UC3, Scalar(0,0,0));

int thrl;
int thru;
int thgl;
int thgu;
int thbl;
int thbu;

String windowName;

void tkbfn(int , void*) {
		int i=0,j=0;
		for (i=0;i<img.rows;i++) {
				for(j=0;j<img.cols;j++) {
						if (img.at<Vec3b>(i,j)[2]<thrl || img.at<Vec3b>(i,j)[2]>thru) img1.at<Vec3b>(i,j)[2]=0;
						else img1.at<Vec3b>(i,j)[2]=img.at<Vec3b>(i,j)[2];
						if (img.at<Vec3b>(i,j)[1]<thgl || img.at<Vec3b>(i,j)[1]>thgu) img1.at<Vec3b>(i,j)[1]=0;
						else img1.at<Vec3b>(i,j)[1]=img.at<Vec3b>(i,j)[1];
						if (img.at<Vec3b>(i,j)[0]<thbl || img.at<Vec3b>(i,j)[0]>thbu) img1.at<Vec3b>(i,j)[0]=0;
						else img1.at<Vec3b>(i,j)[0]=img.at<Vec3b>(i,j)[0];
				}
		}

		imshow(windowName,img1);

		return;
}

int main(int argc, char** argv) {


 if (img.empty()) {
  	cout << "Could not open or find the image" << endl;
  	cin.get(); //wait for any key press
  	return -1;
 }

	windowName = "TrackBar"; //Name of the window

 	namedWindow(windowName,WINDOW_NORMAL); // Create a window

 	createTrackbar( "RedU", windowName , &thru , 255 , tkbfn );
 	createTrackbar( "RedL", windowName , &thrl , 255 , tkbfn );
 	createTrackbar( "GreenU", windowName , &thgu , 255 , tkbfn );
 	createTrackbar( "GreenL", windowName , &thgl , 255 , tkbfn );
 	createTrackbar( "BlueU", windowName , &thbu , 255 , tkbfn );
 	createTrackbar( "BlueL", windowName , &thbl , 255 , tkbfn );

	waitKey(0); // Wait for any keystroke in the window

 	destroyWindow(windowName); //destroy the created window

 return 0;
}
