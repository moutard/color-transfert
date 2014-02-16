/**
 * file Smoothing.cpp
 * brief Sample code for simple filters
 * author OpenCV team
 */
#include <iostream>
#include <vector>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"

using namespace std;
using namespace cv;



/**
 * function main
 */
int main( void )
{
  /// Create a window that can display an image.
  char window_name[] = "Transfer";
  cvNamedWindow( window_name, WINDOW_AUTOSIZE );

  IplImage * pl = cvLoadImage("lena.png");
  cvShowImage( window_name, pl);
  waitKey(0);

  cvReleaseImage(&pl);

  return 0;
}
