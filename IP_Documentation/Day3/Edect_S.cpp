#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Mat img=imread("/home/amshumaan/OpenCV/images/madhav.jpg",0);

Mat	imgf(img.rows,img.cols,CV_8UC1, Scalar(0));

Mat	img1(img.rows,img.cols,CV_8UC1, Scalar(0));

int th;

String windowName1;
String windowName2;

void createimgf() {
	int i=0,j=0;
	int x,y;
	int gx,gy,gf;
		for (i=1;i<img.rows-1;i++) {
				for(j=1;j<img.cols-1;j++) {
					gx=0;
					for (x=i-1,y=j-1;x<=i+1;x++) gx=gx+(-1)*img.at<uchar>(x,y);
					for (x=i+1,y=j+1;x>=i-1;x--) gx=gx+img.at<uchar>(x,y);
					gx=gx+(-1)*img.at<uchar>(i,j-1)+img.at<uchar>(i,j+1); 
					gx=gx/8;
					gy=0;
					for (x=i-1,y=j-1;y<=j+1;y++) gy=gy+(-1)*img.at<uchar>(x,y);
					for (x=i+1,y=j+1;y>=j-1;y--) gy=gy+img.at<uchar>(x,y);
					gy=gy+(-1)*img.at<uchar>(i-1,j)+img.at<uchar>(i+1,j);
					gy=gy/8;
					gf=(int)sqrt(gx*gx+gy*gy);
					imgf.at<uchar>(i,j)=gf;
			}
		}

}


void tkbfn(int , void*) {
		int i=0,j=0;
		for (i=1;i<img.rows-1;i++) {
				for(j=1;j<img.cols-1;j++) {
						
						if (imgf.at<uchar>(i,j) >= th) img1.at<uchar>(i,j)=255;
						else img1.at<uchar>(i,j)=0;
				}
		}


		imshow(windowName1,img1);
		imshow(windowName2,img);

		return;
}

int main(int argc, char** argv) {


 if (img.empty()) {
  	cout << "Could not open or find the image" << endl;
  	cin.get(); //wait for any key press
  	return -1;
 }

	windowName1 = "Edge";
	windowName2 = "Original"; //Name of the window

	createimgf();

 	namedWindow(windowName1,WINDOW_NORMAL);
 	namedWindow(windowName2,WINDOW_NORMAL); // Create a window

 	createTrackbar( "Th", windowName1 , &th , 200 , tkbfn );

	waitKey(0); // Wait for any keystroke in the window

 	destroyWindow(windowName1); //destroy the created window
 	destroyWindow(windowName2);

 return 0;
}