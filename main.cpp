#include <iostream>
#include "transfert.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
int main( void )
{
  /// Create a window that can display an image.
  char window_name[] = "Transfer";
  namedWindow( window_name, WINDOW_AUTOSIZE );

  /// Load the source image
  Mat src = imread( "fruits.jpg", 1 );
  Mat dst = src.clone();
  /// Load the image we will used to extract colors.
  Mat clr = imread("lena.jpg", 1);

  SwitchColor(src, clr, dst);

  /// Display the source image in the window.
  imshow( window_name, dst );

  /// Wait for the key to be pressed to end the program.
  waitKey(0);

  return 0;
}
