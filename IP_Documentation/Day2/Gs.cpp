#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
 Mat image = imread("/home/amshumaan/OpenCV/images/guitar.jpeg",1);

 // Check for failure
 if (image.empty()) 
 {
  cout << "Could not open or find the image" << endl;
  cin.get(); //wait for any key press
  return -1;
 }



 Mat image1(image.rows,image.cols,CV_8UC1, Scalar(0));
 Mat image2(image.rows,image.cols,CV_8UC1, Scalar(0));
 Mat image3(image.rows,image.cols,CV_8UC1, Scalar(0));

int i=0,j=0;
 
 for (i=0;i<image.rows;i++) {
 		for (j=0;j<image.cols;j++) {
 				image1.at<uchar>(i,j)=(image.at<Vec3b>(i,j)[0]+image.at<Vec3b>(i,j)[1]+image.at<Vec3b>(i,j)[2])/3;
 		}
 }

for (i=0;i<image.rows;i++) {
 		for (j=0;j<image.cols;j++) {
 				image3.at<uchar>(i,j)=0.21*image.at<Vec3b>(i,j)[2]+0.72*image.at<Vec3b>(i,j)[1]+0.07*image.at<Vec3b>(i,j)[0];
 		}
 }

uchar max,min;

   for (i=0;i<image.rows;i++) {
 		for (j=0;j<image.cols;j++) {
 			if (image.at<Vec3b>(i,j)[0]>=image.at<Vec3b>(i,j)[1]) {
 					max=image.at<Vec3b>(i,j)[0];
 					min=image.at<Vec3b>(i,j)[1];
 				}
 			else {
 				max=image.at<Vec3b>(i,j)[1];
 				min=image.at<Vec3b>(i,j)[0];
 			}

 			if (max<image.at<Vec3b>(i,j)[2]) max=image.at<Vec3b>(i,j)[2];
 			if (min>image.at<Vec3b>(i,j)[2]) min=image.at<Vec3b>(i,j)[2];

 			image2.at<uchar>(i,j)=(max+min)/2;
 		}
 }


String windowName = "1"; //Name of the window

 namedWindow(windowName,WINDOW_NORMAL); // Create a window

 imshow(windowName, image1); // Show our image inside the created window.

 waitKey(0); // Wait for any keystroke in the window

 imshow(windowName, image2); // Show our image inside the created window.

 waitKey(0); // Wait for any keystroke in the window

 imshow(windowName, image3); // Show our image inside the created window.

 waitKey(0); // Wait for any keystroke in the window

 destroyWindow(windowName); //destroy the created window

 return 0;
}
