#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Mat img=imread("/home/amshumaan/OpenCV/images/op.jpg",1);

Mat	img1(img.rows,img.cols,CV_8UC3, Scalar(0,0,0));

int a;
int b;

String windowName;

void tkbfn(int , void*) {
		    for( int y = 0; y < img.rows; y++ ) {
        		for( int x = 0; x < img.cols; x++ ) {
		            for( int c = 0; c < img.channels(); c++ ) {
		                img1.at<Vec3b>(y,x)[c] =
		                  saturate_cast<uchar>( ((double)a/10)*img.at<Vec3b>(y,x)[c] + b );
            }
        }
    }


		imshow(windowName,img1);
		imshow("Original",img);

		return;
}

int main(int argc, char** argv) {


 if (img.empty()) {
  	cout << "Could not open or find the image" << endl;
  	cin.get(); //wait for any key press
  	return -1;
 }

	windowName = "Contrast"; //Name of the window

 	namedWindow(windowName,WINDOW_NORMAL);
 	namedWindow("Original",WINDOW_NORMAL); // Create a window

 	createTrackbar( "Contrast", windowName , &a , 100 , tkbfn );
 	createTrackbar( "Brightness", windowName , &b , 100 , tkbfn );

	waitKey(0); // Wait for any keystroke in the window

 	destroyWindow(windowName); //destroy the created window

 return 0;
}