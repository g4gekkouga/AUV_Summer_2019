#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Mat img0 = imread("/home/amshumaan/OpenCV/images/op.jpg",0);

Mat img(img0.rows,img0.cols,CV_8UC1, Scalar(0));

void Gus_Blur() {
    int i,j;
    int value;
    for (i=1;i<img0.rows-1;i++) {
        for(j=1;j<img0.cols-1;j++) {
          value=0;
          value=value+(img0.at<uchar>(i-1,j-1)+img0.at<uchar>(i-1,j+1)+img0.at<uchar>(i+1,j-1)+img0.at<uchar>(i+1,j+1))/16;
          value=value+(img0.at<uchar>(i-1,j)+img0.at<uchar>(i,j-1)+img0.at<uchar>(i,j+1)+img0.at<uchar>(i+1,j))/8;
          value=value+(img0.at<uchar>(i,j))/4;
          img.at<uchar>(i,j)=value;
        }
    }
}

int main(int argc, char** argv)
{
 // Check for failure
 if (img0.empty()) 
 {
  cout << "Could not open or find the image" << endl;
  cin.get(); //wait for any key press
  return -1;
 }
 
 Gus_Blur();

String windowName = "Mirror Image"; //Name of the window

 namedWindow(windowName,WINDOW_NORMAL); // Create a window

 imshow(windowName, img0); // Show our image inside the created window.

 waitKey(0); // Wait for any keystroke in the window

 destroyWindow(windowName); //destroy the created window

 return 0;
}
