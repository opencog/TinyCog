/*
  Project: OpenCogER
  File: DSaliency.cpp
  Author: Dagim Sisay
  License: AGPL
  Date: April, 2018 
*/
#include "sense/vision/DSaliency.hpp"

DSaliency::DSaliency(int algorithm_t)
{
	switch(algorithm_t)
	{
		case SAL_FINE_GRAINED:
			sal_det = StaticSaliencyFineGrained::create();
			break;
		case SAL_SPECTRAL_RESIDUAL:
			sal_det = StaticSaliencySpectralResidual::create();
			break;
		case SAL_BING:
			sal_det = ObjectnessBING::create();
			break;
		case SAL_BINWANG:
			sal_det = MotionSaliencyBinWangApr2014::create();
			break;
		defualt:
			sal_det = StaticSaliencyFineGrained::create();
	}
}


Point DSaliency::sal_point(Mat in, Mat &out)
{
	cent.x = 0; cent.y = 0;
	bool salb = sal_det->computeSaliency(in, out);
	if(salb){
		threshold(out, out, 100, 255, CV_THRESH_BINARY);
		findContours(out, cntrs, hier, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
		if(cntrs.size() > 0) {
			largest_cntr_idx = 0;
			max = 0;
			for(size_t idx = 0; idx < cntrs.size(); idx++) {
				area = contourArea(cntrs[idx]);
				if(area > max) {
					max = area;
					largest_cntr_idx = idx;
				}
			}
			minEnclosingCircle(cntrs[largest_cntr_idx], cent, rad);
		}
	}
	return cent;
}


DSaliency::~DSaliency() {}

bool DSaliency::update(Mat in, Mat &out)
{
	return sal_det->computeSaliency(in, out);
}
