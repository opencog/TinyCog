/*
  Project: OpenCogER
  File: DSaliency.cpp
  Author: Dagim Sisay
  License: AGPL
  Date: April, 2018 
*/
#include "sense/vision/DSaliency.hpp"

DSaliency::DSaliency(uint8_t sal_type, string algorithm_t)
{
	switch(sal_type)
	{
		case SAL_DEFAULT:
			sal_det = Saliency::create(algorithm_t);
			break;
		case SAL_STATIC:
			sal_det = StaticSaliency::create(algorithm_t);
			break;
	}
}

DSaliency::DSaliency(uint8_t sal_type, string algorithm_t, int cols, int rows)
{
	if(SAL_MOTION == sal_type && SAL_BINWANG == algorithm_t)
	{
		sal_det = MotionSaliency::create(algorithm_t);
		sal_det.dynamicCast<MotionSaliencyBinWangApr2014>()->setImagesize(cols, rows);
		sal_det.dynamicCast<MotionSaliencyBinWangApr2014>()->init();
	}
	else
	{
		fprintf(stderr, "ERROR: Constructor only for BinWangApr2014 Motion Saliency\n");
	}
}

DSaliency::~DSaliency() {}

bool DSaliency::update(Mat in, Mat &out)
{
	return sal_det->computeSaliency(in, out);
}
