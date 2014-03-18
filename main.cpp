#include <iostream>
#include "transfert.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"

using namespace std;
using namespace cv;

int main(int argc, char * argv[])
{
  if (argc != 3) {
    cout << "ERROR - Invalide number of arguments" << endl;
    cout << "source/file/path color/file/path" << endl;
    return 2;
  }
  /// Create a window that can display an image.
  char window_name[] = "Transfer";
  namedWindow( window_name, WINDOW_AUTOSIZE );

  /// Load the source image
  Mat src = imread(argv[1], 1 );
  Mat dst = Mat::zeros(src.rows, src.cols, CV_8UC3);

  if (src.data == NULL) {
    cout << "The source image can not be found. Invalid path: " << argv[1] << endl;
    return 1;
  }
  //Mat clr = imread("leaves-green.jpg", 1);
  Mat clr = imread(argv[2], 1);
  if (clr.data == NULL) {
    cout << "The color image can not be found. Invalid path: " << argv[2] << endl;
    return 1;
  }

  SwitchColor(src, clr, dst);
  /// Display the source image in the window.
  imshow( window_name, dst );
  /// Wait for the key to be pressed to end the program.
  waitKey(0);

  /// Save the result.
  const char * sOutputName = "colored.jpg";
  imwrite(sOutputName, dst);

  cout << "The result has been saved as" << sOutputName << endl;
  return 0;
}
