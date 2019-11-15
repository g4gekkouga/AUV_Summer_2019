#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

void Reflect (Mat& img1,Mat& img2) {
		cout<<"2";
		int i,j,k;
		for (i=0;i<img1.rows;i++) {
				for(j=0;j<img1.cols;j++) {
					for (k=0;k<3;k++) {
					img2.at<Vec3b>(i,j)[k]=img1.at<Vec3b>(i,img1.cols-1-j)[k];
				}
				}
		}
		return;
}

int main(int argc, char** argv)
{
 // Read the image file
 Mat image = imread("/home/amshumaan/OpenCV/images/op.jpg");
 Mat img(image.rows,image.cols,CV_8UC3, Scalar(0,0,0));


 // Check for failure
 if (image.empty()) 
 {
  cout << "Could not open or find the image" << endl;
  cin.get(); //wait for any key press
  return -1;
 }

  cout<<"1";

 String windowName = "Inverted"; //Name of the window

 namedWindow(windowName,WINDOW_NORMAL); // Create a window

 Reflect(image,img);

 imshow(windowName, img); // Show our image inside the created window.

 waitKey(0); // Wait for any keystroke in the window

 destroyWindow(windowName); //destroy the created window

 return 0;
}