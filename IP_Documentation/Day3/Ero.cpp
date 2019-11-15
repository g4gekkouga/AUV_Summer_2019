#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Mat img=imread("/home/amshumaan/OpenCV/images/rubik.jpg",0);

Mat	imgf(img.rows,img.cols,CV_8UC1, Scalar(0));

Mat	img1(img.rows,img.cols,CV_8UC1, Scalar(0));
Mat	img2(img.rows,img.cols,CV_8UC1, Scalar(0));
Mat	img3(img.rows,img.cols,CV_8UC1, Scalar(0));

int th;

String windowName1;
String windowName2;
String windowName3;

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


void Dilate_img() {
		int i=0,j=0;
		int x,y;
		int sum;
		for (i=1;i<img.rows-1;i++) {
				for(j=1;j<img.cols-1;j++) {
					sum=0;
						for (x=i-1,y=j-1;x<=i+1;x++) sum=sum+img1.at<uchar>(x,y);
						for (x=i-1,y=j;x<=i+1;x++) sum=sum+img1.at<uchar>(x,y);
						for (x=i-1,y=j+1;x<=i+1;x++) sum=sum+img1.at<uchar>(x,y);
					if (sum>0) img2.at<uchar>(i,j)=255;
					else img2.at<uchar>(i,j)=0;
				}
		}


}
void Illu_img() {
		int i=0,j=0;
		int x,y;
		int flag;
		for (i=1;i<img.rows-1;i++) {
				for(j=1;j<img.cols-1;j++) {
					flag=0;
						for (x=i-1,y=j-1;x<=i+1;x++) if (img2.at<uchar>(x,y)==0) flag=1;
						for (x=i-1,y=j;x<=i+1;x++) if (img2.at<uchar>(x,y)==0) flag=1;
						for (x=i-1,y=j+1;x<=i+1;x++) if (img2.at<uchar>(x,y)==0) flag=1;
					if (flag==1) img3.at<uchar>(i,j)=0;
					else img3.at<uchar>(i,j)=255;
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

		Dilate_img();
		Illu_img();


		
		imshow(windowName2,img2);
		imshow(windowName3,img3);


		return;
}

int main(int argc, char** argv) {


 if (img.empty()) {
  	cout << "Could not open or find the image" << endl;
  	cin.get(); //wait for any key press
  	return -1;
 }

	windowName1 = "Edge";
	windowName2 = "Dilated";
	windowName3 = "Illu";

	createimgf();

 	namedWindow(windowName2,WINDOW_NORMAL); // Create a window
 	namedWindow(windowName3,WINDOW_NORMAL);

 	createTrackbar( "Th", windowName2 , &th , 200 , tkbfn );

	waitKey(0); // Wait for any keystroke in the window //destroy the created window
 	
 	destroyWindow(windowName2);
 	destroyWindow(windowName3);

 return 0;
}