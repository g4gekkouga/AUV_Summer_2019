#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
    Mat image = imread("/home/amshumaan/OpenCV/images/op.jpg",1);

    // allcoate memory for no of pixels for each intensity value
    int histr[256];
    int histg[256];
    int histb[256];

    // initialize all intensity values to 0
    for(int i = 0; i < 255; i++)
    {
        histr[i] = 0;
        histg[i] = 0;
        histb[i] = 0;
    }

    // calculate the no of pixels for each intensity values
    for(int y = 0; y < image.rows; y++)
        for(int x = 0; x < image.cols; x++) {
            histr[(int)image.at<Vec3b>(y,x)[2]]++;
            histg[(int)image.at<Vec3b>(y,x)[1]]++;
            histb[(int)image.at<Vec3b>(y,x)[0]]++;
        
        }

    // draw the histograms
    int hist_w = 512; int hist_h = 500;
    int bin_w = cvRound((double) hist_w/256);

    Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));

    // find the maximum intensity element from histogram
    int max = histr[0];
    for(int i = 1; i < 256; i++){
        if(max < histr[i]){
            max = histr[i];
        }
    }
    for(int i = 0; i < 256; i++){
        if(max < histg[i]){
            max = histg[i];
        }
    }
    for(int i = 0; i < 256; i++){
        if(max < histb[i]){
            max = histb[i];
        }
    }

    // normalize the histogram between 0 and histImage.rows

    for(int i = 0; i < 255; i++){
        histr[i] = ((double)histr[i]/max)*histImage.rows;

        histg[i] = ((double)histg[i]/max)*histImage.rows;


        histb[i] = ((double)histb[i]/max)*histImage.rows;
    }


    // draw the intensity line for histogram
    for(int i = 0; i < 255; i++)
    {
        line(histImage, Point(bin_w*(i), hist_h),
                              Point(bin_w*(i), hist_h - histr[i]),
             Scalar(0,0,255), 1, 8, 0);
    }
    for(int i = 0; i < 255; i++)
    {
        line(histImage, Point(bin_w*(i), hist_h),
                              Point(bin_w*(i), hist_h - histb[i]),
             Scalar(0,255,0), 1, 8, 0);
    }

    for(int i = 0; i < 255; i++)
    {
        line(histImage, Point(bin_w*(i), hist_h),
                              Point(bin_w*(i), hist_h - histg[i]),
             Scalar(255,0,0), 1, 8, 0);
    }


    // display histogram
    namedWindow("Intensity Histogram", WINDOW_NORMAL);
    imshow("Intensity Histogram", histImage);

    namedWindow("Image",  WINDOW_NORMAL);
    imshow("Image", image);
    waitKey();
    return 0;
}
