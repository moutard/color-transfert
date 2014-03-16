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

const float INV_SQRT2 = 0.70711f;
const float INV_SQRT3 = 0.57735f;
const float INV_SQRT6 = 0.40825f;
// Constant that reprents id of channels, easyer to read in my opinion.
// Note that RGB is stored in the opposite order.
// I could use an enum?
enum channelsRGB {
  BLUE, GREEN, RED
};
enum channelsLAB {
  LAMBDA, ALPHA, BETHA
};

/**
 * RGB -> LMS
 */
inline float rgbToL(const uchar red, const uchar green, const uchar blue)
{
  float a11 = 0.3811f;
  float a12 = 0.5783f;
  float a13 = 0.0402f;
  return ((a11 * red) + (a12 * green) + (a13 * blue));
}
inline float rgbToM(const uchar red, const uchar green, const uchar blue)
{
  float a21 = 0.1967f;
  float a22 = 0.7244f;
  float a23 = 0.0782f;
  return ((a21 * red) + (a22 * green) + (a23 * blue));
}
inline float rgbToS(const uchar red, const uchar green, const uchar blue)
{
  float a31 = 0.0241f;
  float a32 = 0.1288f;
  float a33 = 0.8444f;
  return ((a31 * red) + (a32 * green) + (a33 * blue));
}

/**
 * LMS -> lambdaAlphaBetha
 */
inline float lmsToLambda(const float l, const float m, const float s)
{
  return INV_SQRT3 * (l + m + s);
}
inline float lmsToAlpha(const float l, const float m, const float s)
{
  return INV_SQRT6 * (l + m - 2*s);
}
inline float lmsToBetha(const float l, const float m, const float s)
{
  return INV_SQRT2 * (l - m);
}

/**
 * Reverse
 *
 * lambdaAlphaBetha -> LMS
 */
inline float labToL(const float l, const float a, const float b)
{
  return (INV_SQRT3 * l + INV_SQRT6 * a + INV_SQRT2 * b);
}
inline float labToM(const float l, const float a, const float b)
{
  return (INV_SQRT3 * l + INV_SQRT6 * a - INV_SQRT2 * b);
}
inline float labToS(const float l, const float a, const float b)
{
  return (INV_SQRT3 * l - 2 * INV_SQRT6 * a);
}

/**
 * LMS -> RGB
 */
inline float lmsToR(const float l, const float m, const float s)
{
  float a11 = 4.4679f;
  float a12 = 3.5873f;
  float a13 = 0.1193f;
  return ((a11 * l) - (a12 * m) + (a13 * s));
}
inline float lmsToG(const float l, const float m, const float s)
{
  float a21 = -1.2186f;
  float a22 = 2.3809f;
  float a23 = 0.1624f;
  return ((a21 * l) + (a22 * m) - (a23 * s));
}
inline float lmsToB(const float l, const float m, const float s)
{
  float a31 = 0.0497f;
  float a32 = 0.2511f;
  float a33 = 1.2045f;
  return ((a31 * l) - (a32 * m) + (a33 * s));
}

inline uchar scaleForRGB (const float f) {
  return (uchar)min(max(f, 0.0f), 255.0f);
}

/**
 * oSrc : image you want to change.
 * oClr: image used to extract color.
 * oDst: an image that will contains the result.
 */
void SwitchColor(Mat& oSrc, Mat& oClr, Mat& oDst)
{
    // accept only char type matrices
    CV_Assert(oSrc.depth() != sizeof(uchar));
    Mat oSrcLabels, oClrLabels;
    const unsigned int K = 3;
    mykmean(oSrc, oSrcLabels, K);
    mykmean(oClr, oClrLabels, K);
    displayLabels(oSrcLabels, K);
    displayLabels(oClrLabels, K);

    const int channels = oSrc.channels();
    if (channels == 3)
    {
      // Create a LAB matrix.
      Mat oSrcLAB = Mat::zeros(oSrc.rows, oSrc.cols, CV_64FC3);
      Mat oClrLAB = Mat::zeros(oClr.rows, oClr.cols, CV_64FC3);

      // Vec3b -> 3 channels of uchar.
      MatIterator_<Vec3b> itSrc, endSrc, itClr, endClr;
      MatIterator_<Vec3b> itDst, endDst;
      // Vec3d -> 3 channels of float.
      MatIterator_<Vec3d> itlab, endlab;
      MatIterator_<Vec3d> itClrlab, endClrlab;

      // oSrc -> LMS -> LAB
      itlab = oSrcLAB.begin<Vec3d>();
      for (itSrc = oSrc.begin<Vec3b>(), endSrc = oSrc.end<Vec3b>(); itSrc != endSrc; ++itSrc)
      {
          // Stored in BGR not RGB !
          uchar b = (*itSrc)[BLUE];
          uchar g = (*itSrc)[GREEN];
          uchar r = (*itSrc)[RED];
          float L = rgbToL(r, g, b);
          float M = rgbToM(r, g, b);
          float S = rgbToS(r, g, b);
          (*itlab)[LAMBDA] = lmsToLambda(L, M, S);
          (*itlab)[ALPHA] = lmsToAlpha( L, M, S);
          (*itlab)[BETHA] = lmsToBetha( L, M, S);
          ++itlab;
      }
      // oClr -> LMS -> LABinputMat.copyTo(outputMat, maskMat);
      itClrlab = oClrLAB.begin<Vec3d>();
      for ( itClr = oClr.begin<Vec3b>(), endClr = oClr.end<Vec3b>(); itClr != endClr; ++itClr)
      {
          uchar b = (*itClr)[BLUE];
          uchar g = (*itClr)[GREEN];
          uchar r = (*itClr)[RED];

          float L = rgbToL(r, g, b);
          float M = rgbToM(r, g, b);
          float S = rgbToS(r, g, b);
          (*itClrlab)[LAMBDA] = lmsToLambda(L, M, S);
          (*itClrlab)[ALPHA] = lmsToAlpha( L, M, S);
          (*itClrlab)[BETHA] = lmsToBetha( L, M, S);
          ++itClrlab;
      }

      // For the source: Compute the mean and standard deviation for each cluster.
      vector<Scalar> meanSrcForCluster, stdDevSrcForCluster;
      meanSrcForCluster.reserve(K), stdDevSrcForCluster.reserve(K);
      for (unsigned int i = 0; i < K; i++)
      {
        Mat oMaskClusterSrc(oSrcLabels.size(), CV_8UC1, Scalar(0)); // clone constructor.
        MatIterator_<Vec3b> itMask, itLabels;
        itMask = oMaskClusterSrc.begin<Vec3b>();
        for (itLabels = oSrcLabels.begin<Vec3b>(); itLabels != oSrcLabels.end<Vec3b>(); ++itLabels)
        {
            if ((*itLabels)[0] == i) {
              (*itMask)[0] = 1;
            }
            ++itMask;
        }
        meanStdDev(oSrcLAB, meanSrcForCluster[i], stdDevSrcForCluster[i], oMaskClusterSrc);
        printMeanAndStdDev(meanSrcForCluster[i], stdDevSrcForCluster[i], i, "Source");
      }

      // For the color: Compute the mean and standard deviation for eah cluster.
      vector<Scalar> meanClrForCluster, stdDevClrForCluster;
      meanClrForCluster.reserve(K), stdDevClrForCluster.reserve(K);
      for (unsigned int i = 0; i < K; i++)
      {
        Mat oMaskClusterClr(oClrLabels.size(), CV_8UC1, Scalar(0)); // clone constructor.
        MatIterator_<Vec3b> itMask, itLabels;
        itMask = oMaskClusterClr.begin<Vec3b>();
        for (itLabels = oClrLabels.begin<Vec3b>(); itLabels != oClrLabels.end<Vec3b>(); ++itLabels)
        {
            if ((*itLabels)[0] ==  i) {
              (*itMask)[0] = 1;
            }
            ++itMask;
        }
        meanStdDev(oClrLAB, meanClrForCluster[i], stdDevClrForCluster[i], oMaskClusterClr);
        printMeanAndStdDev(meanClrForCluster[i], stdDevClrForCluster[i], i, "Color");
      }

      // Associate each src cluster with the closest clr cluster.
      vector<Scalar> quotientSrcForCluster;
      quotientSrcForCluster.reserve(K);
      for (unsigned int i = 0; i < K; ++i)
      {
        quotientSrcForCluster[i][LAMBDA] = (stdDevClrForCluster[i][LAMBDA] / stdDevSrcForCluster[i][LAMBDA]);
        quotientSrcForCluster[i][ALPHA] = (stdDevClrForCluster[i][ALPHA] / stdDevSrcForCluster[i][ALPHA]);
        quotientSrcForCluster[i][BETHA] = (stdDevClrForCluster[i][BETHA] / stdDevSrcForCluster[i][BETHA]);
      }

      // Do the modification.
      MatIterator_<Vec3b> itLabels = oSrcLabels.begin<Vec3b>();
      for (itlab = oSrcLAB.begin<Vec3d>(), endlab = oSrcLAB.end<Vec3d>(); itlab != endlab; ++itlab)
      {
        unsigned int iClusterId = (*itLabels)[0];
        (*itlab)[LAMBDA] = quotientSrcForCluster[iClusterId][LAMBDA] *
            ((*itlab)[LAMBDA] - meanSrcForCluster[iClusterId][LAMBDA])
            + meanClrForCluster[iClusterId][LAMBDA];
        (*itlab)[ALPHA] = quotientSrcForCluster[iClusterId][LAMBDA] *
            ((*itlab)[ALPHA] - meanSrcForCluster[iClusterId][ALPHA])
            + meanClrForCluster[iClusterId][ALPHA];
        (*itlab)[BETHA] = quotientSrcForCluster[iClusterId][BETHA] *
            ((*itlab)[BETHA] - meanSrcForCluster[iClusterId][BETHA])
            + meanClrForCluster[iClusterId][BETHA];
        ++itLabels;
      }

      // Reverse
      // LAB -> LMS -> oDst
      itDst = oDst.begin<Vec3b>();
      for (itlab = oSrcLAB.begin<Vec3d>(), endlab = oSrcLAB.end<Vec3d>(); itlab != endlab; ++itlab)
      {
          float l = (*itlab)[LAMBDA];
          float a = (*itlab)[ALPHA];
          float b = (*itlab)[BETHA];
          float L = labToL(l, a, b);
          float M = labToM(l, a, b);
          float S = labToS(l, a, b);
          (*itDst)[RED] = scaleForRGB(lmsToR(L, M, S));
          (*itDst)[GREEN] = scaleForRGB(lmsToG(L, M, S));
          (*itDst)[BLUE] = scaleForRGB(lmsToB(L, M, S));
          ++itDst;
      }

    }
}

int mykmean(Mat& img, Mat& _labels, const int k)
{
    if (k <= 1) {
      _labels = Mat::zeros(img.rows, img.cols, CV_8UC1);
    } else {
      std::vector<cv::Mat> lImg_RGB_channels;
      cv::split(img, lImg_RGB_channels);
      int n = img.rows * img.cols;
      // kmeans opencv algorithm need an mat of float.
      cv::Mat img5xN(n, 5, CV_32F);
      for (int iRow = 0; iRow < img.rows; iRow++) {
        for (int iCol = 0; iCol < img.cols; iCol++) {
          for (int iChannel = 0; iChannel != 3; ++iChannel) {
            img5xN.at<Vec3f>(iRow*img.cols + iCol, iChannel)[0] = (float)img.at<Vec3b>(iRow, iCol)[iChannel];
          }
            img5xN.at<Vec3f>(iRow*img.cols + iCol, 3)[0] = (float) 0.2f * iRow;
            img5xN.at<Vec3f>(iRow*img.cols + iCol, 4)[0] = (float) 0.2f * iCol;
        }
      }
      cv::kmeans(img5xN, k, _labels, cv::TermCriteria(), 3, cv::KMEANS_RANDOM_CENTERS);
      _labels = _labels.reshape(0, img.rows);
    }

    return k;
}

void printMeanAndStdDev(const Scalar & mean, const Scalar & stdDev, int i, string name)
{
    cout << "Cluster " << i << ":" << endl;
    cout << name << " mean LAB: "
        << mean[LAMBDA] << " "
        << mean[ALPHA] << " "
        << mean[BETHA] << endl;
    cout << name << " stdDev LAB: "
        << stdDev[LAMBDA] << " "
        << stdDev[ALPHA] << " "
        << stdDev[BETHA] << endl;

}

void displayLabels(const Mat & _labels, int k)
{
  Mat imgLabels;
  cv::convertScaleAbs(_labels, imgLabels, int(255/k));
  cv::imshow("result", imgLabels);
  imwrite("labels.jpg", imgLabels);
  waitKey(0);
}
