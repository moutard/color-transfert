#include <iostream>
#include <math.h>
#include <opencv2/ts/ts.hpp>
#include <transfert.h>

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
    float EPS = 0.5f;
    bResult = bResult && (a[0] >= 0.0f && a[0] <= 255.0f) && (b[0] >= 0.0f && b[0] <= 255.0f);
    bResult = bResult && (a[1] >= 0.0f && a[1] <= 255.0f) && (b[1] >= 0.0f && b[1] <= 255.0f);
    bResult = bResult && (a[2] >= 0.0f && a[2] <= 255.0f) && (b[2] >= 0.0f && b[2] <= 255.0f);
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

Scalar inverseTotal(const Scalar & scalar) {
  float l,m,s;
  float lambda, alpha, betha;
  float r, g, b;
  r = scalar[0], g = scalar[1], b = scalar[2];
  uchar R, G, B;
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

  R = (uchar) r;
  G = (uchar) g;
  B = (uchar) b;
  cout << (unsigned int)R << " " << (unsigned int)G << " " << (unsigned int)B;
  cout << " || " <<  (unsigned int) scalar[0] << " " << (unsigned int) scalar[1] << " " << (unsigned int) scalar[2];
  cout << " || " <<   r << " " << g << " " << b << endl;
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
