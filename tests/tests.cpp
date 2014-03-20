#include <iostream>
#include <math.h>
#include <opencv2/ts/ts.hpp>
#include "../src/transfert.h"

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"

using namespace std;
using namespace cv;
// Custom operator to compare cv::Scalar class...
bool scalar_equal(const cv::Scalar &a, const cv::Scalar &b)
{
    bool bResult = false;
    bResult = (a[0] == b[0]) && (a[1] == b[1]) && (a[2] == b[2]);
    return bResult;
}
bool scalar_has_coherent_value(const cv::Scalar &a, const cv::Scalar &b)
{
    bool bResult = true;
    float EPS = 2;
    bResult = bResult && (a[0] >= 0 && a[0] <= 255) && (b[0] >= 0 && b[0] <= 255);
    bResult = bResult && (a[1] >= 0 && a[1] <= 255) && (b[1] >= 0 && b[1] <= 255);
    bResult = bResult && (a[2] >= 0 && a[2] <= 255) && (b[2] >= 0 && b[2] <= 255);
    bResult = bResult && (abs(a[0] - b[0]) < EPS);
    bResult = bResult && (abs(a[1] - b[1]) < EPS);
    bResult = bResult && (abs(a[2] - b[2]) < EPS);
    return bResult;
}

bool scalar_lms_has_coherent_value(const cv::Scalar &a, const cv::Scalar &b)
{
    bool bResult = true;
    float EPS = 0.5f;
    bResult = bResult && (abs(a[0] - b[0]) < EPS);
    bResult = bResult && (abs(a[1] - b[1]) < EPS);
    bResult = bResult && (abs(a[2] - b[2]) < EPS);
    return bResult;
}


Scalar inverseLAB(const Scalar & scalar) {
  float l,m,s;
  float lambda, alpha, betha;
  l = scalar[0], m = scalar[1], s = scalar[2];
  lambda = lmsToLambda(l,m,s);
  alpha = lmsToAlpha(l,m,s);
  betha = lmsToBetha(l,m,s);

  l = labToL(lambda, alpha, betha);
  m = labToM(lambda, alpha, betha);
  s = labToS(lambda, alpha, betha);
  return Scalar(l, m, s);
}

Scalar inverseLMS(const Scalar & scalar) {
  float r, g, b;
  float l,m,s;
  r = scalar[0], g = scalar[1], b = scalar[2];
  l = rgbToL(r, g, b);
  m = rgbToM(r, g, b);
  s = rgbToS(r, g, b);
  r = lmsToR(l, m, s);
  g = lmsToG(l, m, s);
  b = lmsToB(l, m, s);
  return Scalar(r, g, b);
}

Scalar inverseLMSChar(const Scalar & scalar) {
  uchar r, g, b;
  float l,m,s;
  r = scalar[0], g = scalar[1], b = scalar[2];
  l = rgbToL(r, g, b);
  m = rgbToM(r, g, b);
  s = rgbToS(r, g, b);
  r = (unsigned int)lmsToR(l, m, s);
  g = (unsigned int)lmsToG(l, m, s);
  b = (unsigned int)lmsToB(l, m, s);
  return Scalar(r, g, b);
}

Scalar inverseTotal(const Scalar & scalar) {
  float l,m,s;
  float lambda, alpha, betha;
  float r, g, b;
  r = scalar[0], g = scalar[1], b = scalar[2];
  unsigned int R, G, B;
  l = rgbToL(r, g, b);
  m = rgbToM(r, g, b);
  s = rgbToS(r, g, b);

  lambda = lmsToLambda(l,m,s);
  alpha = lmsToAlpha(l,m,s);
  betha = lmsToBetha(l,m,s);

  l = labToL(lambda, alpha, betha);
  m = labToM(lambda, alpha, betha);
  s = labToS(lambda, alpha, betha);

  r = lmsToR(l, m, s);
  g = lmsToG(l, m, s);
  b = lmsToB(l, m, s);

  R = (unsigned int) r;
  G = (unsigned int) g;
  B = (unsigned int) b;
  return Scalar(R, G, B);
}

// google test is preincluded in opencv.
TEST(Fake, FakeTest) {
  bool mybool = true;
  ASSERT_TRUE(mybool);
}

TEST(Inverse, LABInverse_BLACK) {
  Scalar s0(0.0f, 0.0f, 0.0f);
  Scalar s0_ =  inverseLAB(s0);
  ASSERT_PRED2(scalar_lms_has_coherent_value, s0, s0_);
}

TEST(Inverse, LABInverse_RED) {
  Scalar s0(255.0f, 0.0f, 0.0f);
  Scalar s0_ =  inverseLAB(s0);
  ASSERT_PRED2(scalar_lms_has_coherent_value, s0, s0_);
}

TEST(Inverse, LABInverse_GREEN) {
  Scalar s0(0.0f, 255.0f, 0.0f);
  Scalar s0_ =  inverseLAB(s0);
  ASSERT_PRED2(scalar_lms_has_coherent_value, s0, s0_);
}

TEST(Inverse, LABInverse_BLUE) {
  Scalar s0(0.0f, 0.0f, 255.0f);
  Scalar s0_ =  inverseLAB(s0);
  ASSERT_PRED2(scalar_lms_has_coherent_value, s0, s0_);
}

TEST(Inverse, LABInverse_CYAN) {
  Scalar s0(0.0f, 255.0f, 255.0f);
  Scalar s0_ =  inverseLAB(s0);
  ASSERT_PRED2(scalar_lms_has_coherent_value, s0, s0_);
}

TEST(Inverse, LABInverse_PURPLE) {
  Scalar s0(255.0f, 0.0f, 255.0f);
  Scalar s0_ =  inverseLAB(s0);
  ASSERT_PRED2(scalar_lms_has_coherent_value, s0, s0_);
}

TEST(Inverse, LABInverse_YELLOW) {
  Scalar s0(255.0f, 255.0f, 0.0f);
  Scalar s0_ =  inverseLAB(s0);
  ASSERT_PRED2(scalar_lms_has_coherent_value, s0, s0_);
}

TEST(Inverse, LABInverse_WHITE) {
  Scalar s0(255.0f, 255.0f, 255.0f);
  Scalar s0_ =  inverseLAB(s0);
  ASSERT_PRED2(scalar_lms_has_coherent_value, s0, s0_);
}

/**
 * RGB -> LMS -> RGB
 */
TEST(Inverse, LMSInverse_BLACK) {
  Scalar s0(0.0f, 0.0f, 0.0f);
  Scalar s0_ =  inverseLMS(s0);
  ASSERT_PRED2(scalar_lms_has_coherent_value, s0, s0_);
}

TEST(Inverse, LMSInverse_RED) {
  Scalar s0(255.0f, 0.0f, 0.0f);
  Scalar s0_ =  inverseLMS(s0);
  ASSERT_PRED2(scalar_lms_has_coherent_value, s0, s0_);
}

TEST(Inverse, LMSInverse_GREEN) {
  Scalar s0(0.0f, 255.0f, 0.0f);
  Scalar s0_ =  inverseLMS(s0);
  ASSERT_PRED2(scalar_lms_has_coherent_value, s0, s0_);
}

TEST(Inverse, LMSInverse_BLUE) {
  Scalar s0(0.0f, 0.0f, 255.0f);
  Scalar s0_ =  inverseLMS(s0);
  ASSERT_PRED2(scalar_lms_has_coherent_value, s0, s0_);
}

TEST(Inverse, LMSInverse_CYAN) {
  Scalar s0(0.0f, 255.0f, 255.0f);
  Scalar s0_ =  inverseLMS(s0);
  ASSERT_PRED2(scalar_lms_has_coherent_value, s0, s0_);
}

TEST(Inverse, LMSInverse_PURPLE) {
  Scalar s0(255.0f, 0.0f, 255.0f);
  Scalar s0_ =  inverseLMS(s0);
  ASSERT_PRED2(scalar_lms_has_coherent_value, s0, s0_);
}

TEST(Inverse, LMSInverse_YELLOW) {
  Scalar s0(255.0f, 255.0f, 0.0f);
  Scalar s0_ =  inverseLMS(s0);
  ASSERT_PRED2(scalar_lms_has_coherent_value, s0, s0_);
}

TEST(Inverse, LMSInverse_WHITE) {
  Scalar s0(255.0f, 255.0f, 255.0f);
  Scalar s0_ =  inverseLMS(s0);
  ASSERT_PRED2(scalar_lms_has_coherent_value, s0, s0_);
}

/**
 * cast to uchar
 */

/**
 * RGB -> LMS -> RGB
 */
TEST(Inverse, LMS_char_Inverse_BLACK) {
  Scalar s0(0, 0, 0);
  Scalar s0_ =  inverseLMSChar(s0);
  ASSERT_PRED2(scalar_has_coherent_value, s0, s0_);
}

TEST(Inverse, LMS_char_Inverse_RED) {
  Scalar s0(255, 0, 0);
  Scalar s0_ =  inverseLMSChar(s0);
  ASSERT_PRED2(scalar_has_coherent_value, s0, s0_);
}

TEST(Inverse, LMS_char_Inverse_GREEN) {
  Scalar s0(0, 255, 0);
  Scalar s0_ =  inverseLMSChar(s0);
  ASSERT_PRED2(scalar_has_coherent_value, s0, s0_);
}

TEST(Inverse, LMS_char_Inverse_BLUE) {
  Scalar s0(0, 0, 255);
  Scalar s0_ =  inverseLMSChar(s0);
  ASSERT_PRED2(scalar_has_coherent_value, s0, s0_);
}

TEST(Inverse, LMS_char_Inverse_CYAN) {
  Scalar s0(0, 255, 255);
  Scalar s0_ =  inverseLMSChar(s0);
  ASSERT_PRED2(scalar_has_coherent_value, s0, s0_);
}

TEST(Inverse, LMS_char_Inverse_PURPLE) {
  Scalar s0(255, 0, 255);
  Scalar s0_ =  inverseLMSChar(s0);
  ASSERT_PRED2(scalar_has_coherent_value, s0, s0_);
}

TEST(Inverse, LMS_char_Inverse_YELLOW) {
  Scalar s0(255, 255, 0);
  Scalar s0_ =  inverseLMSChar(s0);
  ASSERT_PRED2(scalar_has_coherent_value, s0, s0_);
}

TEST(Inverse, LMS_char_Inverse_WHITE) {
  Scalar s0(255, 255, 255);
  Scalar s0_ =  inverseLMSChar(s0);
  ASSERT_PRED2(scalar_has_coherent_value, s0, s0_);
}

/**
 * Total inversion
 */
TEST(Inverse, TOTAL_Inverse_BLACK) {
  Scalar s0(0, 0, 0);
  Scalar s0_ =  inverseTotal(s0);
  ASSERT_PRED2(scalar_has_coherent_value, s0, s0_);
}

TEST(Inverse, TOTAL_Inverse_RED) {
  Scalar s0(255, 0, 0);
  Scalar s0_ =  inverseTotal(s0);
  ASSERT_PRED2(scalar_has_coherent_value, s0, s0_);
}

TEST(Inverse, TOTAL_Inverse_GREEN) {
  Scalar s0(0, 255, 0);
  Scalar s0_ =  inverseTotal(s0);
  ASSERT_PRED2(scalar_has_coherent_value, s0, s0_);
}

TEST(Inverse, TOTAL_Inverse_BLUE) {
  Scalar s0(0, 0, 255);
  Scalar s0_ =  inverseTotal(s0);
  ASSERT_PRED2(scalar_has_coherent_value, s0, s0_);
}

TEST(Inverse, TOTAL_Inverse_CYAN) {
  Scalar s0(0, 255, 255);
  Scalar s0_ =  inverseTotal(s0);
  ASSERT_PRED2(scalar_has_coherent_value, s0, s0_);
}

TEST(Inverse, TOTAL_Inverse_PURPLE) {
  Scalar s0(255, 0, 255);
  Scalar s0_ =  inverseTotal(s0);
  ASSERT_PRED2(scalar_has_coherent_value, s0, s0_);
}

TEST(Inverse, TOTAL_Inverse_YELLOW) {
  Scalar s0(255, 255, 0);
  Scalar s0_ =  inverseTotal(s0);
  ASSERT_PRED2(scalar_has_coherent_value, s0, s0_);
}

TEST(Inverse, TOTAL_Inverse_WHITE) {
  Scalar s0(255, 255, 255);
  Scalar s0_ =  inverseTotal(s0);
  ASSERT_PRED2(scalar_has_coherent_value, s0, s0_);
}

