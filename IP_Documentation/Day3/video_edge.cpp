#include "opencv2/opencv.hpp"

using namespace cv;

int main(int, char**)
{
    VideoCapture cap("/home/amshumaan/OpenCV/images/BellamBabu.mp4");
    
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    Mat edges,edgesC;
    namedWindow("edges",WINDOW_NORMAL);
    for(;;)
    {
        int c=0;
        int d=0;
        Mat frame;
        cap >> frame; // get a new frame from camera
        edgesC.create(frame.size(),frame.type());
        edgesC=Scalar::all(0);
        cvtColor(frame, edges, COLOR_BGR2GRAY);
        GaussianBlur(edges, edges, Size(7,7), 1.5, 1.5);
        Canny(edges, edges, 0, 30, 3);
        frame.copyTo(edgesC,edges);
        imshow("edges", edgesC);
        
        c=waitKey(30);

        if (c==32) d=waitKey(0);

        if (d==32) continue;

        if(c >= 0) break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
