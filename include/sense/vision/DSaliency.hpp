/*
  Project: OpenCogER
  File: DSaliency.hpp
  Author: Dagim Sisay
  License: AGPL
  Date: April, 2018
*/

//Wrapper for Saliency in opencv_contrib

#ifndef DSALIENCY_H_
#define DSALIENCY_H_
#include <opencv2/opencv.hpp>
#include <opencv2/saliency.hpp>

#include <string>
#include <vector>

#include <stdio.h>

#define SAL_DEFAULT 0
#define SAL_STATIC 1
#define SAL_MOTION 2

#define SAL_FINE_GRAINED "FINE_GRAINED"
#define SAL_SPECTRAL_RESIDUAL "SPECTRAL_RESIDUAL"
#define SAL_BING "BING"
#define SAL_BINWANG "BinWangApr2014"

using namespace std;
using namespace cv;
using namespace saliency;

class DSaliency
{
	public:
	DSaliency(uint8_t sal_type, string algorithm_t);
	// the following for BinWangApr2014 Motion Saliency only 
	DSaliency(uint8_t sal_type, string algorithm_t, int cols, int rows); 
	~DSaliency();
	bool update(Mat in, Mat &out);
	Point sal_point(Mat in, Mat &out);
	private:
	Ptr<Saliency> sal_det;
	vector<vector<Point> > cntrs;
	vector<Vec4i> hier;
	Point2f cent;
	int largest_cntr_idx = 0;
	double max = 0, area;
	float rad;
};
#endif //DSaliency_H_
