#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
 Mat image = imread("/home/amshumaan/OpenCV/images/madhav.jpg",0);

 // Check for failure
 if (image.empty()) 
 {
  cout << "Could not open or find the image" << endl;
  cin.get(); //wait for any key press
  return -1;
 }

  Mat image1(image.rows,2*image.cols,CV_8UC3, Scalar(0,0,0));

  Canny(image,image1,25,50,3,0);



 

String windowName = "Canny Image"; //Name of the window

 namedWindow(windowName,WINDOW_NORMAL); // Create a window

 imshow(windowName, image1); // Show our image inside the created window.

 waitKey(0); // Wait for any keystroke in the window

 destroyWindow(windowName); //destroy the created window

 return 0;
}