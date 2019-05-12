DOCUMENTATION------------------DAY 2-------------P AMSHUMAAN
VARMA-------------IP PHASE 1---------------------------------

1)Installing Ubuntu and OpenCv Libraries.

2)Introduction to Image Planes and Pixels.

3)Accessing Individual Pixels.

4)Basic Syntax for creating and displaying Windows and Images

5)Different Intensities and Order of channels

6)Headders Used :

include "openCV2/highgui/highgui.hpp"
=====================================

include "openCV2/imgproc/imgproc.hpp"
=====================================

include "openCV2/core/core.hpp"
===============================

7)Compilig the cpp files using MAKE and executing them

Eg---------------------CODES-------------------------------------------------

------------------------DIAGONAL
BOARD---------------------------------------

include "openCV2/highgui/highgui.hpp"
=====================================

include "openCV2/imgproc/imgproc.hpp"
=====================================

include "openCV2/core/core.hpp"
===============================

int main() { \#FOR RED-YELLOW int i,j; Mat
img(50,50,CV\_8UC3,scalar(0,0,255)); for(i=0;i&lt;50;i++) {
for(j=49;j&gt;49-i-1;j--) { img.at<Vec3b>(i,j)={0,255,255}; } }
namedWindow("win",WINDOW\_NORMAL); waitKey(0); return 0; }

------------------------------------------------------------------------

--------------------------------CHESS
BOARD------------------------------------

include &lt;opencv2/highgui/highgui.hpp&gt;
===========================================

include &lt;opencv2/imgproc/imgproc.hpp&gt;
===========================================

include &lt;opencv2/core/core.hpp&gt;
=====================================

using namespace cv;

int main() { Mat image(240,240,CV\_8UC1,Scalar(0));

// initialising an image with rows and columns of size 240. // CV\_8UC1
... here // '8U' represents 8-bit unsigned integer. // 'C1' represents
that only one channel is present (greyscale). // initialised with a
value 0

int box\_size = image.rows/8;

for(int i=0 ; i&lt;image.rows ; i++) { for(int j=0 ; j&lt;image.cols ;
j++) { int color = ((i/box\_size)+(j/box\_size))%2; image.at<uchar>(i,j)
= color\*255; } }

namedWindow("chessboard",CV\_WINDOW\_NORMAL);
imshow("chessboard",image); waitKey(0);

return 0; }

--------------------------------------END-----------------------------------------
