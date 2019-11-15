#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Mat frame;

Mat filter;

int thrl=0;
int thru=60;
int thgl=100;
int thgu=200;
int thbl=100;
int thbu=200;

RNG rng(12345);

Point OC;
Point PC;

int areathl=100;
int areathu=1000;


String windowName;

void tkbfn() {
        int i=0,j=0;
        for (i=0;i<frame.rows;i++) {
                for(j=0;j<frame.cols;j++) {
                        if ((frame.at<Vec3b>(i,j)[2]<thrl || frame.at<Vec3b>(i,j)[2]>thru) || (frame.at<Vec3b>(i,j)[1]<thgl || frame.at<Vec3b>(i,j)[1]>thgu) || (frame.at<Vec3b>(i,j)[0]<thbl || frame.at<Vec3b>(i,j)[0]>thbu)) {
                                filter.at<Vec3b>(i,j)[2]=0;
                                filter.at<Vec3b>(i,j)[1]=0;
                                filter.at<Vec3b>(i,j)[0]=0;
                        }
                        else {
                                filter.at<Vec3b>(i,j)[2]=frame.at<Vec3b>(i,j)[2];
                                filter.at<Vec3b>(i,j)[1]=frame.at<Vec3b>(i,j)[1];
                                filter.at<Vec3b>(i,j)[0]=frame.at<Vec3b>(i,j)[0];

                        }
                }
        }
        return;
}

int main(int, char**)
{
    OC.x=0;
    OC.y=0;
    PC.x=0;
    PC.y=0;

    VideoCapture cap("/home/amshumaan/OpenCV/files/PS/AH.mp4");
    
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    windowName="Filtered Output";
    namedWindow(windowName,WINDOW_NORMAL);
    for(;;)
    {
        cap >> frame; // get a new frame from camera

        filter.create(frame.size(),frame.type());

        tkbfn();

        Mat frameG;
	 	
	 	cvtColor(filter,frameG,COLOR_BGR2GRAY);

	 	Mat frameGB;
	 	
	 	GaussianBlur(frameG,frameGB,Size(3,3),0,0);

	 	Mat frameB;
	 	
	 	threshold( frameGB,frameB,25,255,THRESH_BINARY);

	 	Mat frameC;
	 	
	 	Canny(frameB,frameC,20,25,3,0);

        vector<vector<Point> > contours,contours1;
  		vector<Vec4i> hierarchy;

  		Mat frameCont(frame.rows,frame.cols,CV_8UC3,Scalar(0,0,0));


	 	findContours( frameB,contours,hierarchy,CV_RETR_TREE,CV_CHAIN_APPROX_SIMPLE,Point(0, 0) );

	// 	cout<<contours.size()<<endl;

	// 	createTrackbar( "Area",windowName,&areathl,1000);


	 	Mat drawing = Mat::zeros( frame.size(), CV_8UC1 );
   		for( int i = 0; i< contours.size(); i++ )
    	{	
    		if ((contourArea(contours[i])>=areathl) && (contourArea(contours[i])<=areathu)) {
        //		Scalar color = Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256) );
        		drawContours( drawing, contours, (int)i,Scalar(255), 2, LINE_8, hierarchy, 0 );
    		}
    	}

    	findContours(drawing,contours1,hierarchy,CV_RETR_TREE,CV_CHAIN_APPROX_SIMPLE,Point(0, 0) );
    //	cout<<contours1.size()<<endl;

    	int max_x,min_x;
       	int max_y,min_y;
       	double cen_x=0,cen_y=0;
      // 	double area=0;



    	for( int i = 0; i< contours1.size(); i++ ){
    		max_x=0;
    		max_y=0;
    		min_x=frame.cols;
    		min_y=frame.rows;
    		

    		for (int j=0;j<contours1[i].size();j++) {
       			if (contours1[i][j].x > max_x) max_x=contours1[i][j].x;
       			if (contours1[i][j].x < min_x) min_x=contours1[i][j].x;
       			if (contours1[i][j].y > max_y) max_y=contours1[i][j].y;
       			if (contours1[i][j].y < min_y) min_y=contours1[i][j].y;
       		}

       		cen_x=cen_x+(max_x-min_x)/2;
    		cen_y=cen_y+(max_y-min_y)/2;

       	//	area=area+contourArea(contours1[i]);

       	//	cen_x=cen_x+contourArea(contours1[i])*(max_x-min_x)/2;
       	//	cen_y=cen_y+contourArea(contours1[i])*(max_y-min_y)/2;
    	}

    	

    	OC.x=PC.x;
   		OC.y=PC.y;

    	if (contours1.size()) {
    		
    	//	PC.x=(int)(cen_x/area);
    	//	PC.y=(int)(cen_y/area);
    		PC.x=cen_x/contours1.size();
    		PC.y=cen_y/contours1.size();


    	}

    	else {

    		PC.x=0;
    		PC.y=0;

    	}

    	cout<<cen_x/contours[0][0].x<<endl;


    	imshow(windowName,drawing);



    /*    createTrackbar( "RedU", windowName , &thru , 255  );
        createTrackbar( "RedL", windowName , &thrl , 255  );
        createTrackbar( "GreenU", windowName , &thgu , 255  );
        createTrackbar( "GreenL", windowName , &thgl , 255  );
        createTrackbar( "BlueU", windowName , &thbu , 255 );
        createTrackbar( "BlueL", windowName , &thbl , 255  );  */  



        if(waitKey(500) >= 0) break; 

    }
    cap.release();
    destroyAllWindows();
    return 0;
}
