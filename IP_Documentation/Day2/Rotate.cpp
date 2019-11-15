#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Mat img=imread("/home/amshumaan/OpenCV/images/op.jpg",1);

Mat	img1(img.rows,img.cols,CV_8UC3, Scalar(0,0,0));

int angle;
 const float pi=3.14;

String windowName;

void tkbfn(int , void*) {
		int x,y;

		for (int i=0; i<img.cols ; i++){
			 for (int j=0 ; j<img.rows ; j++)
			 {
			   //x'=(x-p)Cos(ϴ')-(y-q)Sin(ϴ')+p
			   x=(int)((i-img.cols/2)*cos((float)angle*pi/180)-(j-img.rows/2)*sin((float)angle*pi/180)+img.cols/2);
			   //y'=(y-q)Cos(ϴ')+(x-p)Sin(ϴ')+q
			   y=(int)((i-img.cols/2)*sin((float)angle*pi/180)+(j-img.rows/2)*cos((float)angle*pi/180)+img.rows/2);
 
   			   if((x>=0 && x<img.cols) && (y>=0 && y<img.rows)) {
   			   		img1.at<Vec3b>(y,x)=img.at<Vec3b>(j,i);
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

	windowName = "RotatedImage"; //Name of the window

 	namedWindow(windowName,WINDOW_NORMAL);
 	namedWindow("Original",WINDOW_NORMAL); // Create a window

 	createTrackbar( "Angle", windowName , &angle , 360 , tkbfn );

	waitKey(0); // Wait for any keystroke in the window

 	destroyWindow(windowName); //destroy the created window

 return 0;
}