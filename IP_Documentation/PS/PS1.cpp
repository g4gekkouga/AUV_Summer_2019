
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <string>

#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <termios.h>

using namespace std;
using namespace cv;

int fd;

double th=25;
double AreaMax;
int X;
int Y;

int t;
int dist;

void settings(const char *abc);
void sendCommand(const char *abc);

int main() {

	 VideoCapture cap("/home/amshumaan/OpenCV/images/cup.jpg");

	 if(!cap.isOpened()) return -1;



	 while(1) {

	 	Mat frame;
	 	cap >> frame;

	 	Mat frameG;
	 	
	 	cvtColor(frame,frameG,COLOR_BGR2GRAY);

	 	Mat frameGB;
	 	
	 	GaussianBlur(frameG,frameGB,Size(3,3),0,0);

	 	Mat frameB;
	 	
	 	threshold( frameGB,frameB,200,255,THRESH_BINARY);

	 	Mat frameC;
	 	
	 	Canny(frameB,frameC,20,25,3,0);

	 	AreaMax=0.8*frame.rows*frame.cols;

	 	vector<vector<Point> > contours;
  		vector<Vec4i> hierarchy;

  		Mat frameCont(frame.rows,frame.cols,CV_8UC3,Scalar(0,0,0));


	 	findContours( frameB,contours,hierarchy,CV_RETR_TREE,CV_CHAIN_APPROX_SIMPLE,Point(0, 0) );

	 	cout<<contours.size()<<endl;
	 //	for (int j=0;j<contours.size();j++) cout<<contours[j][0]<<endl;

	 	vector<Point> contourMax;

	 	double max=0;
	 	double area=0;
	 	int ind=1;

	 	for (int j=1;j<contours.size();j++) {
	 		area=contourArea(contours[j]);
	 		if (max<area) {
	 			max=area;
	 			ind=j;
	 		}
	 	}

	 	if (max < th) {
	 		X=-125;
	 		Y=0;
	 		continue;
	 	}

	 	if (max >= AreaMax ) {
	 		X=0;
	 		Y=0;
	 		continue;
	 	}

	 	contourMax=contours[ind];


       	drawContours( frameCont, contours,ind, Scalar(0,0,255),-1, 8, hierarchy ,2, Point(0,0) );

       	int max_x=0,min_x=frame.cols;
       	int max_y=0,min_y=frame.rows;
       	int cen_x,cen_y;

       	for (int i=0;i<contourMax.size();i++) {
       		if (contourMax[i].x > max_x) max_x=contourMax[i].x;
       		if (contourMax[i].x < min_x) min_x=contourMax[i].x;
       		if (contourMax[i].y > max_y) max_y=contourMax[i].y;
       		if (contourMax[i].y < min_y) min_y=contourMax[i].y;
       	}

       	cen_x=(max_x+min_x)/2;
       	cen_y=(max_y+min_y)/2;

       	Point Center;
       	Center.x=cen_x;
       	Center.y=cen_y;

       	int Pos_x=frame.cols/2;

       	X=(int)(125-(125*(max-th)/(AreaMax-th)));

       	Y=(int)(250*(Center.x)/frame.cols-125);



       	

       	

   //  	cout<<endl<<Center<<endl;

	 	namedWindow("edges",WINDOW_NORMAL);
	 	imshow("edges", frameCont);

	 	 if(waitKey(0) == 98) break;
	 }
	 return 0;
}

void settings(const char *abc)
{
      fd = open(abc,O_RDWR | O_NOCTTY); /* ttyUSB0 is the FT232 based USB2SERIAL Converter   */
      usleep(3500000);
                                    /* O_RDWR Read/Write access to serial port           */
                                    /* O_NOCTTY - No terminal will control the process   */
                                    /* O_NDELAY -Non Blocking Mode,Does not care about-  */
                                    /* -the status of DCD line,Open() returns immediatly */                                       
                                   
            if(fd == -1)                        /* Error Checking */
                   printf("\n  Error! in Opening ttyUSB0  ");
            else
                   printf("\n  ttyUSB0 Opened Successfully ");
       struct termios toptions;         /* get current serial port settings */
       tcgetattr(fd, &toptions);        /* set 9600 baud both ways */
       cfsetispeed(&toptions, B9600);
       cfsetospeed(&toptions, B9600);   /* 8 bits, no parity, no stop bits */
       toptions.c_cflag &= ~PARENB;
       toptions.c_cflag &= ~CSTOPB;
       toptions.c_cflag &= ~CSIZE;
       toptions.c_cflag |= CS8;         /* Canonical mode */
       toptions.c_lflag |= ICANON;       /* commit the serial port settings */
       tcsetattr(fd, TCSANOW, &toptions);
}

void sendCommand(const char *abc)
{
   write(fd, abc, 1);
}