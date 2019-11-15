#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
 Mat image = imread("/home/amshumaan/OpenCV/images/guitar.jpeg",0);

 // Check for failure
 if (image.empty()) 
 {
  cout << "Could not open or find the image" << endl;
  cin.get(); //wait for any key press
  return -1;
 }

int i=0;
int j=0;

  Mat image1(image.rows,image.cols,CV_8UC1, Scalar(0));

  for (j=0;j<image.cols;j++) {
  		for (i=0;i<image.rows;i++) {
  			image1.at<uchar>(i,j)=image.at<uchar>(i,image.cols-1-j);
 		}
  }

String windowName = "NMirror Image"; //Name of the window

 namedWindow(windowName,WINDOW_NORMAL); // Create a window

 imshow(windowName, image1); // Show our image inside the created window.

 waitKey(0); // Wait for any keystroke in the window

 destroyWindow(windowName); //destroy the created window

 return 0;
}
