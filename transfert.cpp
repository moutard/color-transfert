/**
 * file Smoothing.cpp
 * brief Sample code for simple filters
 * author OpenCV team
 */
#include <iostream>
#include <vector>
#include <math.h>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"

#include "transfert.h"
using namespace std;
using namespace cv;

/**
 * RGB -> LMS
 */
float rgbToL(const uchar red, const uchar green, const uchar blue)
{
  float a11 = 0.3811f;
  float a12 = 0.5783f;
  float a13 = 0.0402f;
  return a11 * red + a12 * green + a13 * blue;
};
float rgbToM(const uchar red, const uchar green, const uchar blue)
{
  float a21 = 0.1967f;
  float a22 = 0.7244f;
  float a23 = 0.0782f;
  return a21 * red + a22 * green + a23 * blue;
};
float rgbToS(const uchar red, const uchar green, const uchar blue)
{
  float a31 = 0.0241f;
  float a32 = 0.1288f;
  float a33 = 0.8444f;
  return a31 * red + a32 * green + a33 * blue;
};

/**
 * LMS -> lambdaAlphaBetha
 */
float lmsToLambda(const float l, const float m, const float s)
{
  float coeff = 0.5773f; // 1/sqrt(3)
  return coeff * (l + m + s);
};
float lmsToAlpha(const float l, const float m, const float s)
{
  float coeff = 0.4082; // 1/sqrt(6)
  return coeff * (l + m - 2*s);
};
float lmsToBetha(const float l, const float m, const float s)
{
  float coeff = 0.7071; // 1/sqrt(2)
  return coeff * (l - m);
};

/**
 * Reverse
 *
 * lambdaAlphaBetha -> LMS
 */
float labToL(const float l, const float a, const float b)
{
  float coeff1 = 0.5773f; // sqrt(3)/3
  float coeff2 = 0.4082f; // sqrt(6)/6
  float coeff3 = 0.7071f; // sqrt(2)/2
  return (coeff1*l + coeff2*a + coeff3*b);
};
float labToM(const float l, const float a, const float b)
{
  float coeff1 = 0.5773f; // sqrt(3)/3
  float coeff2 = 0.4082f; // sqrt(6)/6
  float coeff3 = 0.7071f; // sqrt(2)/2
  return (coeff1*l + coeff2*a - coeff3*b);
};
float labToS(const float l, const float a, const float b)
{
  float coeff1 = 0.5773f; // sqrt(3)/3
  float coeff2 = 0.4082f; // sqrt(6)/6
  float coeff3 = 0.7071f; // sqrt(2)/2
  return (coeff1*l -  2*coeff2*a);
};

/**
 * LMS -> RGB
 */
float lmsToR(const float l, const float m, const float s)
{
  float a11 = 4.4679f;
  float a12 = -3.5873f;
  float a13 = 0.1193f;
  return a11 * l + a12 * m + a13 * s;
};
float lmsToG(const float l, const float m, const float s)
{
  float a21 = -1.2186f;
  float a22 = 2.3809f;
  float a23 = -0.1624f;
  return a21 * l + a22 * m + a23 * s;
};
float lmsToB(const float l, const float m, const float s)
{
  float a31 = 0.0497f;
  float a32 = -0.2439f;
  float a33 =  1.2045f;
  return a31 * l + a32 * m + a33 * s;
};


void SwitchColor(Mat& oSrc, Mat& oClr, Mat& oDst)
{
    // accept only char type matrices
    CV_Assert(oSrc.depth() != sizeof(uchar));

    const int channels = oSrc.channels();
    if (channels == 3)
    {
      // Create a temp matrix.
      Mat oLMS = cvCreateMat(oSrc.rows, oSrc.cols, CV_32FC3);
      MatIterator_<Vec3b> it, end;
      MatIterator_<Vec3b> itlms, endlms;

      // oSrc -> oLMS
      itlms = oLMS.begin<Vec3b>();
      for( it = oSrc.begin<Vec3b>(), end = oSrc.end<Vec3b>(); it != end; ++it)
      {
          uchar r = (*it)[0];
          uchar g = (*it)[1];
          uchar b = (*it)[2];
          (*itlms)[0] = ( rgbToL(r, g, b ));
          (*itlms)[1] = ( rgbToM(r, g, b ));
          (*itlms)[2] = ( rgbToS(r, g, b ));
          ++itlms;
      }
/*
      for (itlms = oLMS.begin<Vec3b>(), endlms = oLMS.end<Vec3b>(); itlms != endlms; ++itlms)
      {
          float l = (*itlms)[0];
          float m = (*itlms)[1];
          float s = (*itlms)[2];

          (*itlms)[0] = lmsToLambda(l, m, s);
          (*itlms)[1] = lmsToAlpha(l, m, s);
          (*itlms)[2] = lmsToBetha(l, m, s);
      }

      // Reverse
      // oLMS -> oDst
      for (itlms = oLMS.begin<Vec3b>(), endlms = oLMS.end<Vec3b>(); itlms != endlms; ++itlms)
      {
          float l = (*itlms)[0];
          float a = (*itlms)[1];
          float b = (*itlms)[2];
          (*itlms)[0] = ( labToL(l, a, b));
          (*itlms)[1] = ( labToM(l, a, b));
          (*itlms)[2] = ( labToS(l, a, b));
      }
*/
      itlms = oLMS.begin<Vec3b>();
      for( it = oDst.begin<Vec3b>(), end = oDst.end<Vec3b>(); it != end; ++it)
      {
          float l = (*itlms)[0];
          float m = (*itlms)[1];
          float s = (*itlms)[2];
          (*it)[0] = (uchar)lmsToR(l, m, s);
          (*it)[1] = (uchar)lmsToG(l, m, s);
          (*it)[2] = (uchar)lmsToB(l, m, s);
          ++itlms;
      }
    }
};
