#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Mat img=imread("/home/amshumaan/OpenCV/images/parrot.jpg",0);
Mat    img1(img.rows,img.cols,CV_8UC1, Scalar(255));

int main()
{
    String windowName;
    String windowN;

    int i,j,a,b,n,x;
    x=0;
    n=0;
        for(i=0;i<img.rows;i++)
    {
        for(j=0;j<img.cols;j++)
        {
            for(a=(i-1);a<=(i+1);a++)
            {
                for(b=(j-1);b<=(j+1);b++)
                {
                    if(a>=0 && b>=0 && a<img.rows && b<img.cols)
                    {
                        n = n + img.at<uchar>(a,b);
                        x = x + 1;
                    }
                }
            }
            img1.at<uchar>(i,j)=n/x;
            x = 0;
            n = 0;
        }
    }
    windowName = "Blur";
    windowN = "bw";   
    namedWindow(windowName,WINDOW_NORMAL);
    namedWindow(windowN,WINDOW_NORMAL);
    imshow(windowName,img1);
    imshow(windowN,img);   
    waitKey(0);
    destroyWindow(windowName);
    return 0;
}

