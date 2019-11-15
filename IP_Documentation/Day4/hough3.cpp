#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <math.h>

using namespace std;
using namespace cv;

const float pi=3.14;

Mat image = imread("/home/amshumaan/OpenCV/images/pentagon.png",0);
int r=image.rows;
int c=image.cols;
int th=80;

String windowName;

int main() {
	int i,j;

	windowName = "Hough Lines"; //Name of the window

 	namedWindow(windowName,WINDOW_NORMAL);

 	Mat outimg(r,c,CV_8UC1,Scalar(0));

 	vector<Vec2f> lines;
    HoughLines(image,lines,1, CV_PI/180,75,0,0);

  for( int i = 0; i < lines.size(); i++ )
  {
     float rho = lines[i][0], theta = lines[i][1];
     Point pt1, pt2;
     double a = cos(theta), b = sin(theta);
     double x0 = a*rho, y0 = b*rho;
     pt1.x = cvRound(x0 + 5000*(-b));
     pt1.y = cvRound(y0 + 5000*(a));
     pt2.x = cvRound(x0 - 5000*(-b));
     pt2.y = cvRound(y0 - 5000*(a));
     line( outimg , pt1, pt2, Scalar(255),1,8,0 );
  }
  imshow(windowName,outimg);

 	waitKey(0); // Wait for any keystroke in the window

 	destroyWindow(windowName); //destroy the created window

	return 0;

}