#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
 Mat image = imread("/home/amshumaan/OpenCV/images/op.jpg",1);

 // Check for failure
 if (image.empty()) 
 {
  cout << "Could not open or find the image" << endl;
  cin.get(); //wait for any key press
  return -1;
 }

Mat image1(3*image.rows,3*image.cols,CV_8UC3, Scalar(0,0,0));

float h_ind=(float)image.cols/image1.cols;
float v_ind=(float)image.rows/image1.rows;

int i,j;

for (i=0;i<image1.rows;i++) {
  		for (j=0;j<image1.cols;j++) {
  				image1.at<Vec3b>(i,j)=image.at<Vec3b>((int)i*v_ind,(int)j*h_ind);
  		}
}

namedWindow("Original",WINDOW_NORMAL); // Create a window

 namedWindow("Resize",WINDOW_NORMAL); // Create a window

 imshow("Original", image); // Show our image inside the created window.
 imshow("Resize", image1); // Show our image inside the created window.

 waitKey(0); // Wait for any keystroke in the window

 destroyWindow("Original");
  destroyWindow("Resize"); //destroy the created window

 return 0;
}