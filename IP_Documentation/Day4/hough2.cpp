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

String windowName;

int main() {
	int i,j;

	windowName = "Hough Lines"; //Name of the window

 	namedWindow(windowName,WINDOW_NORMAL);

 	Mat outimg(r,c,CV_8UC1,Scalar(0));

 	vector<Vec4i> lines;
    HoughLinesP(image,lines,1, CV_PI/180,80,25,15);

    for( int i = 0; i < lines.size(); i++ )
  {
    Vec4i l = lines[i];
    line( outimg, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255),1,8,0);
  }

  imshow(windowName,outimg);

 	waitKey(0); // Wait for any keystroke in the window

 	destroyWindow(windowName); //destroy the created window

	return 0;

}