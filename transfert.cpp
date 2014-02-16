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
  return ((a11 * red) + (a12 * green) + (a13 * blue));
};
float rgbToM(const uchar red, const uchar green, const uchar blue)
{
  float a21 = 0.1967f;
  float a22 = 0.7244f;
  float a23 = 0.0782f;
  return ((a21 * red) + (a22 * green) + (a23 * blue));
};
float rgbToS(const uchar red, const uchar green, const uchar blue)
{
  float a31 = 0.0241f;
  float a32 = 0.1288f;
  float a33 = 0.8444f;
  return ((a31 * red) + (a32 * green) + (a33 * blue));
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
  float a12 = 3.5873f;
  float a13 = 0.1193f;
  return ((a11 * l) - (a12 * m) + (a13 * s));
};
float lmsToG(const float l, const float m, const float s)
{
  float a21 = -1.2186f;
  float a22 = 2.3809f;
  float a23 = 0.1624f;
  return ((a21 * l) + (a22 * m) - (a23 * s));
};
float lmsToB(const float l, const float m, const float s)
{
  float a31 = 0.0497f;
  float a32 = 0.2439f;
  float a33 = 1.2045f;
  return ((a31 * l) - (a32 * m) + (a33 * s));
};


void SwitchColor(Mat& oSrc, Mat& oClr, Mat& oDst)
{
    // accept only char type matrices
    CV_Assert(oSrc.depth() != sizeof(uchar));

    const int channels = oSrc.channels();
    if (channels == 3)
    {
      // Create a temp matrix.

      Mat oLAB = Mat::zeros(oSrc.rows, oSrc.cols, CV_32FC3);
      MatIterator_<Vec3b> itSrc, endSrc;
      MatIterator_<Vec3b> itDst, endDst;
      MatIterator_<Vec3f> itlab, endlab;

      // oSrc -> LMS -> LAB
      itlab = oLAB.begin<Vec3f>();
      for( itSrc = oSrc.begin<Vec3b>(), endSrc = oSrc.end<Vec3b>(); itSrc != endSrc; ++itSrc)
      {
          uchar r = (*itSrc)[0];
          uchar g = (*itSrc)[1];
          uchar b = (*itSrc)[2];

          float L = log( rgbToL(r, g, b) );
          float M = log( rgbToM(r, g, b) );
          float S = log( rgbToS(r, g, b) );
          (*itlab)[0] = lmsToLambda(L, M, S);
          (*itlab)[1] = lmsToAlpha( L, M, S);
          (*itlab)[2] = lmsToBetha( L, M, S);
          ++itlab;
      }


      // Reverse
      // LAB -> LMS -> oDst
      itDst = oDst.begin<Vec3b>();
      for (itlab = oLAB.begin<Vec3f>(), endlab = oLAB.end<Vec3f>(); itlab != endlab; ++itlab)
      {
          float l = (*itlab)[0];
          float a = (*itlab)[1];
          float b = (*itlab)[2];
          float L = exp( labToL(l, a, b) );
          float M = exp( labToM(l, a, b) );
          float S = exp( labToS(l, a, b) );
          (*itDst)[0] = (uchar)lmsToR(L, M, S);
          (*itDst)[1] = (uchar)lmsToG(L, M, S);
          (*itDst)[2] = (uchar)lmsToB(L, M, S);
          ++itDst;
      }
    }
};

void test(Mat& oSrc, Mat& oDst) {
      MatIterator_<Vec3b> it, end;
      MatIterator_<Vec3b> itlab, endlab;

      // oSrc -> oLMS
      for( it = oSrc.begin<Vec3b>(), end = oSrc.end<Vec3b>(); it != end; ++it)
      {
          uchar r = (*it)[0];
          uchar g = (*it)[1];
          uchar b = (*it)[2];
          float l = rgbToL(r, g, b );
          float m = rgbToM(r, g, b );
          float s = rgbToS(r, g, b );
          float _r = lmsToR(l, m, s);
          float _g = lmsToG(l, m, s);
          float _b = lmsToB(l, m, s);
          uchar __r = (uchar)_r;
          uchar __g = (uchar)_g;
          uchar __b = (uchar)_b;

          if ( abs(__r - r) > 5 || abs(__g - g) > 5 || abs(__b - b) > 5) {
            cout << "origi rgb: " << (int)r << " " << (int)g << " " << (int)b <<  endl;
            cout << "inter lms: " << l << " " << m << " " << s <<  endl;
            cout << "final rgb: " << _r << " " << _g << " " << _b <<  endl;
            cout << "uchar rgb: " << (int)__r << " " << (int)__g << " " << (int)__b <<  endl;
            cout << endl;
          }
      }


}
