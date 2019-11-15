#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include "opencv2/imgcodecs.hpp"
#include <iostream>

using namespace cv;
using namespace std;

Mat img1=imread("/home/amshumaan/OpenCV/images/parrot.jpg",1);
Mat img2=imread("/home/amshumaan/OpenCV/images/op.jpg",1);

Mat Nimg1,Nimg2;

Mat	img3(220,180,CV_8UC3, Scalar(0,0,0));

int t;

String windowName3;

void tkbfn(int , void*) {
		

		addWeighted( Nimg1 , (float)(100-t)/100 , Nimg2, (float)t/100 , 0.0, img3);

		imshow(windowName3,img3);

		return;
}




int main(int argc, char** argv)
{

 if (img1.empty() || img2.empty()) 
 {
  cout << "Could not open or find the image" << endl;
  cin.get(); //wait for any key press
  return -1;
 }

 windowName3 = "Club";

 resize(img1,Nimg1,Size(500,500));
 resize(img2,Nimg2,Size(500,500));


 namedWindow(windowName3,WINDOW_NORMAL);

createTrackbar( "Index", windowName3 , &t , 100 , tkbfn );

 waitKey(0); // Wait for any keystroke in the window

  destroyWindow(windowName3); //destroy the created window

 return 0;
}