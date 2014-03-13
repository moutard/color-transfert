#ifndef TRANSFERT_H
#define TRANSFERT_H
#include <string>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
/**
 * RGB -> LMS
 */
float rgbToL(const uchar red, const uchar green, const uchar blue);
float rgbToM(const uchar red, const uchar green, const uchar blue);
float rgbToS(const uchar red, const uchar green, const uchar blue);

/**
 * LMS -> lambdaAlphaBetha
 */
float lmsToLambda(const float l, const float m, const float s);
float lmsToAlpha(const float l, const float m, const float s);
float lmsToBetha(const float l, const float m, const float s);

/**
 * Reverse
 *
 * lambdaAlphaBetha -> LMS
 */
float labToL(const float l, const float a, const float b);
float labToM(const float l, const float a, const float b);
float labToS(const float l, const float a, const float b);

/**
 * LMS -> RGB
 */
float lmsToR(const float l, const float m, const float s);
float lmsToG(const float l, const float m, const float s);
float lmsToB(const float l, const float m, const float s);

void SwitchColor(cv::Mat& oSrc, cv::Mat& oClr, cv::Mat& oDst);
void compare(cv::Mat& oSrc, cv::Mat& oDst);
void test(cv::Mat& oSrc, cv::Mat& oDst);
int mykmean(cv::Mat& img, cv::Mat& labels, const int k);
int displayLabels(const cv::Mat & labels, int k);
void printMeanAndStdDev(const cv::Scalar & mean, const cv::Scalar & stdDev, int i, std::string name);

void tests();
void testMatrix(const uchar x, const uchar y, const uchar z);
#endif

