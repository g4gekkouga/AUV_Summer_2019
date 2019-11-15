#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>

using namespace cv;

int main()
{
  Mat image(240,240,CV_8UC1,Scalar(0));

  // initialising an image with rows and columns of size 240.
  // CV_8UC1 ... here
  // '8U' represents 8-bit unsigned integer.
  // 'C1' represents that only one channel is present (greyscale).
  // initialised with a value 0

  int box_size = image.rows/8;

  for(int i=0 ; i<image.rows ; i++)
  {
    for(int j=0 ; j<image.cols ; j++)
    {
      int color = ((i/box_size)+(j/box_size))%2;
      image.at<uchar>(i,j) = color*255;
    }
  }

  namedWindow("chessboard",CV_WINDOW_NORMAL);
  imshow("chessboard",image);
  waitKey(0);

  return 0;
}
