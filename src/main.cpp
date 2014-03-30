#include <iostream>
#include <string>
#include "transfert.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include <boost/program_options.hpp>
using namespace std;
using namespace cv;
namespace po = boost::program_options;

int main(int argc, char * argv[])
{

  // Declare the supported options.
  po::options_description desc("Transfert Color program takes an input image and apply to it the color of the other image.\n Save the result into the output image. Do not modify the input and color image. \n Allowed options");
  string output;
  int kmeans;
  desc.add_options()
    ("help,h", "produce help message")
    ("input,i", po::value<string>(), "path to the input image")
    ("color,c", po::value<string>(), "path to the color image")
    ("output,o", po::value<string>(&output)->default_value("colored.png"), "path to the output image")
    ("kmeans,k", po::value<int>(&kmeans)->default_value(0), "use kmeans cluster before recoloration [!BETHA can produce some unexpected results]");

  po::variables_map oVMap;
  po::store(po::parse_command_line(argc, argv, desc), oVMap);
  po::notify(oVMap);

  if (oVMap.count("help")) {
    cout << desc << endl;
    return 1;
  }

  if (!oVMap.count("input")) {
     cout << "ERROR - Input image is mandatory." << endl;
     cout << desc << endl;
    return 1;
  }
  if (!oVMap.count("color")) {
     cout << "ERROR - Color image is mandatory." << endl;
     cout << desc << endl;
    return 1;
  }

  cout << "Input set to "
        << oVMap["input"].as<string>() << ".\n";

  /// Create a window that can display an image.
  char window_name[] = "Transfer";
  namedWindow( window_name, WINDOW_AUTOSIZE );

  /// Load the source image
  Mat src = imread(oVMap["input"].as<string>(), 1);
  Mat dst = Mat::zeros(src.rows, src.cols, CV_8UC3);

  if (src.data == NULL) {
    cout << "The source image can not be found. Invalid path: " << oVMap["input"].as<string>() << endl;
    return 1;
  }
  //Mat clr = imread("leaves-green.jpg", 1);
  Mat clr = imread(oVMap["color"].as<string>() , 1);
  if (clr.data == NULL) {
    cout << "The color image can not be found. Invalid path: " << oVMap["color"].as<string>() << endl;
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
