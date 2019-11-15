#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <string>
#include <queue>

using namespace std;
using namespace cv;


typedef struct pt_{
	int x,y;
}pt;

Mat image = imread("/home/amshumaan/OpenCV/images/dfs_path.jpg",0);
Mat img(image.rows,image.cols,CV_8UC1,Scalar(0));

int r=image.rows;
int c=image.cols;

String windowName;

void Paint(int event, int x, int y, int flag, void* ) {
		if (event==1) {
			if (image.at<uchar>(y,x)<200) {
				int i,j,k,l,id=1;
				vector<vector<int> > pixel_ID(r,vector<int>(c,-1)); //Stores ID of a pixel; -1 means unvisited
				queue<pt> open_list;

				pt start = {x,y};

				open_list.push(start);

				while(!open_list.empty()){
					pt top = open_list.front();
					open_list.pop();
					img.at<uchar>(top.y,top.x)=127;
					pixel_ID[top.y][top.x] = id;

					for(k=top.y-1;k<=top.y+1;k++){
						for(l=top.x-1;l<=top.x+1;l++){
							if (l>=0 && k>=0 && k<img.rows && l<img.cols) {
								
								if(img.at<uchar>(k,l)>200||pixel_ID[k][l]>-1) continue;


								pt next = {l,k};
								pixel_ID[k][l] = id;
								open_list.push(next);
							}
						}
					}
				}

				
    			imshow(windowName,img);

			}
		}
		return;

}











int main(){

	int i=0,j=0;

	/* for (i=0;i<image.rows;i++) {
		for (j=0;j<image.cols;j++) {
			if (image.at<uchar>(i,j)>=225) img.at<uchar>(i,j)=255;
			else img.at<uchar>(i,j)=0;
		}
	} */



	/* for (i=0;i<img.cols;i++) img.at<uchar>(0,i)=255;
	for (i=0;i<img.cols;i++) img.at<uchar>(img.rows-1,i)=255;
	for (i=0;i<img.rows;i++) img.at<uchar>(i,0)=255;
	for (i=0;i<img.rows;i++) img.at<uchar>(i,img.cols-1)=255;  */  

	img=image.clone();

	windowName="ClickToPaint";
	namedWindow(windowName,WINDOW_NORMAL);

	setMouseCallback(windowName,Paint,NULL);

	imshow(windowName,img);

	waitKey(0);

	destroyAllWindows();


    return 0; 


}