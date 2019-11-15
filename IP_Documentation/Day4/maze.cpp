#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <string>
#include <queue>

using namespace std;
using namespace cv;

Mat img(121,121,CV_8UC1,Scalar(0));

void kernel(int i,int j) {

	for(int x=i-5;x<=i+5;x++) {
		for(int y=j-5;j<=j+5;y++) {
			if (x>=0 && y>=0 && x<r && y<c)
			img.at<uchar>(x,y)
		}
	}

}


int main() {
	int i=0;
	int j=0;

	for 


}