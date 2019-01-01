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

#define SAL_FINE_GRAINED 0
#define SAL_SPECTRAL_RESIDUAL 1
#define SAL_BING 3
#define SAL_BINWANG 4

using namespace std;
using namespace cv;
using namespace saliency;

class DSaliency
{
public:
	DSaliency(int algorithm_t=SAL_FINE_GRAINED);
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
