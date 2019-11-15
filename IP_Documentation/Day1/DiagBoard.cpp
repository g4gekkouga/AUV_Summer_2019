#include <openCV2/highgui.hpp>
#include <openCV2/imgproc.hpp>
#include <openCV2/core.hpp>
#include <iostream>
int main()
{
	#FOR RED-YELLOW
	int i,j;
	Mat img(50,50,CV_8UC3,scalar(0,0,255));
	for(i=0;i<50;i++)
	{
		for(j=49;j>49-i-1;j--)
		{
			img.at<Vec3b>(i,j)={0,255,255};
		}
	}
	namedWindow("win",WINDOW_NORMAL);
	waitKey(0);
	return 0;
}