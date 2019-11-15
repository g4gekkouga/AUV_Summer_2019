#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <math.h>

using namespace std;
using namespace cv;

const float pi=3.14;

Mat image = imread("/home/amshumaan/OpenCV/images/op.jpg",0);
int r=image.rows;
int c=image.cols;
int th=80;
Mat distang((int)sqrt(r*r+c*c)+1,360,CV_8UC1,Scalar(0));
Mat outimg(r,c,CV_8UC1,Scalar(0));

String windowName;

/*typedef struct pair_{
	int d;
	int ang;
}pair; */

//vector<pair> lines;

void Hough() {

	int i,j,x,y;

	for (i=0;i<r;i++){
		for (j=0;j<c;j++){
			if (image.at<uchar>(i,j)>0) {
				for (x=0;x<distang.rows;x++){
					for (y=0;y<distang.cols;y++){

						if ((int)(j*cos((float)y*pi/180)+i*sin((float)y*pi/180))==x) distang.at<uchar>(x,y)++;
						

					}
				}
			}



		}
	}

	for (x=0;x<distang.rows;x++){
		for (y=0;y<distang.cols;y++){
			if (distang.at<uchar>(x,y)>=th) {
				for (i=0;i<r;i++){
					for (j=0;j<c;j++){
						if ((int)(j*cos((float)y*pi/180)+i*sin((float)y*pi/180))==x) {
							if (image.at<uchar>(i,j)>0) outimg.at<uchar>(i,j)=255;
						}
					}
				}

			}
		}
	}



	

	namedWindow(windowName,WINDOW_NORMAL);
 	imshow(windowName,outimg);

 	waitKey(0); // Wait for any keystroke in the window

 	destroyWindow(windowName); //destroy the created window

	return;

}

int main() {

	int i,j;

	windowName = "Intensity graph"; //Name of the window

 	namedWindow(windowName,WINDOW_NORMAL);
 	imshow(windowName,image);

 	waitKey(0); // Wait for any keystroke in the window

 	destroyWindow(windowName); //destroy the created window

 	Hough();




 return 0;




}