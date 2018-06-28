/*
  Project: OpenCogER
  File: FingersCount.hpp
  Author: Kidist Abraham <kidistabraham@gmail.com>
          Dagim Sisay<dagiopia@gmail.com>
  License: AGPL
  Date: March, 2018
*/


#ifndef FINGERSCOUNT_H_
#define FINGERSCOUNT_H_

#include <opencv2/opencv.hpp>

#include "sense/vision/ITColor2Gray.hpp"
#include "sense/vision/ITEqualizeHist.hpp"
#include "sense/vision/ITDetectHand.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <stdint.h>

#define DEBUG

/* 
  TODO find a better way for depth range
  something that's calculated from values
  at current hand based on size or distance
  from other points or whatever
*/
#define MIN_DEPTH 15.0
#define MAX_DEPTH 70.0

typedef std::vector<cv::Point> VP;
typedef std::vector<VP> VVP;
typedef std::vector<cv::Vec4i> VV4i;
typedef VP CONTOUR;
typedef VVP CONTOURS;

class FingersCount
{
    public:
	FingersCount(bool debug=false); //XXX remove param
	~FingersCount(){}
	
	/* 
	    a function to return number of fingers
	    supply a mat that's ROId on the hand (a small one)
	    since it would be brought here after doing hand detection
	    which requires the a binarized image, this should do fine
	*/
	uint8_t num_fingers(cv::Mat);
	std::vector<cv::Point> f_tips;  //XXX remove
	
	
    private:
        size_t largest_cntr_idx();
	CONTOURS cntrs;
	CONTOUR cntr;
	VV4i hier;
	std::vector<int> hull;
	VV4i defects;
	cv::Point st_pt, en_pt, far_pt; //XXX remove
	int st_idx, en_idx, far_idx;    //XXX remove
	uint8_t count;
	double depth;  //XXX remove

	bool debug;
};

#endif //FINGERSCOUNT_H_
