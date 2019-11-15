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

typedef struct blob_{

	int min_x,max_x;
	int min_y,max_y;
	int cen_x,cen_y;
	int n_pixels;
	int ID;
}blob;

Mat image = imread("/home/amshumaan/OpenCV/images/dfs_path.jpg",0);
Mat img(image.rows,image.cols,CV_8UC1,Scalar(0));
Mat img1;

//Mat img(image.rows,image.cols,CV_8UC1,Scalar(0));

vector<blob> blobs;

void GetBlobs(){

	int i,j,k,l,r = img.rows,c = img.cols,id=1;
	vector<vector<int> > pixel_ID(r,vector<int>(c,-1)); //Stores ID of a pixel; -1 means unvisited
	queue<pt> open_list; //Breadth-First-Search hence queue of points

	for(i=0;i<r;i++){
		for(j=0;j<c;j++){
			if(img.at<uchar>(i,j)==255||pixel_ID[i][j]>-1)
				continue;
			pt start = {j,i};

			open_list.push(start);
			int sum_x=0,sum_y=0,n_pixels=0,max_x=0,max_y=0;
			int min_x = c+1, min_y=r+1;
			while(!open_list.empty()){
				//Dequeue the element at the head of the queue
				pt top = open_list.front();
				open_list.pop();
				pixel_ID[top.y][top.x] = id;
				n_pixels++;

				//To obtain the bounding box of the blob w.r.t the original image
				min_x = (top.x<min_x)?top.x:min_x;
				min_y = (top.y<min_y)?top.y:min_y;
				max_x = (top.x>max_x)?top.x:max_x;
				max_y = (top.y>max_y)?top.y:max_y;
				sum_y+=top.y; sum_x+=top.x;

				//Add the 8-connected neighbours that are yet to be visited, to the queue
				for(k=top.y-1;k<=top.y+1;k++){
					for(l=top.x-1;l<=top.x+1;l++){
						if (l>=0 && k>=0 && k<img.rows && l<img.cols) {
							if(img.at<uchar>(k,l)==255||pixel_ID[k][l]>-1)
								continue;
							pt next = {l,k};
							pixel_ID[k][l] = id;
							open_list.push(next);
						}
					}
				}
			}
			if(n_pixels < 20) //At least 20 pixels
				continue;
			blob nextcentre = {min_x,max_x,min_y,max_y,sum_x/n_pixels,sum_y/n_pixels,id};
			blobs.push_back(nextcentre);
			id++;
		}
	}

	cout<<blobs.size()<<endl<<endl; //To test correctness; can use the vector as desired
}

void OutlineBlobs() {

	img1=image.clone();

	int i=0;
	int radius;
	int maxx,maxy;
	for (i=0;i<blobs.size();i++) {


		maxx = ((blobs[i].max_x-blobs[i].cen_x)>(blobs[i].cen_x-blobs[i].min_x))?(blobs[i].max_x-blobs[i].cen_x):(blobs[i].cen_x-blobs[i].min_x);

		maxy = ((blobs[i].max_y-blobs[i].cen_y)>(blobs[i].cen_y-blobs[i].min_y))?(blobs[i].max_y-blobs[i].cen_y):(blobs[i].cen_y-blobs[i].min_y);

		radius=sqrt(maxx*maxx+maxy*maxy);

		circle(img1,Point(blobs[i].cen_x,blobs[i].cen_y),radius,Scalar(127),1,8,0);

	}

	return;

}

int main(){

	int i=0,j=0;

	for (i=0;i<image.rows;i++) {
		for (j=0;j<image.cols;j++) {
			if (image.at<uchar>(i,j)>=225) img.at<uchar>(i,j)=255;
			else img.at<uchar>(i,j)=0;
		}
	}



/*	for (i=0;i<img.cols;i++) img.at<uchar>(0,i)=255;
	for (i=0;i<img.cols;i++) img.at<uchar>(img.rows-1,i)=255;
	for (i=0;i<img.rows;i++) img.at<uchar>(i,0)=255;
	for (i=0;i<img.rows;i++) img.at<uchar>(i,img.cols-1)=255;      */    


	namedWindow("Image",WINDOW_NORMAL);
    imshow("Image",img);

	waitKey(0);
	destroyAllWindows();

	GetBlobs();
    
    OutlineBlobs();

    namedWindow("Image",WINDOW_NORMAL);
    imshow("Image",img1);

    waitKey(0);
	destroyAllWindows();




    return 0; 


}