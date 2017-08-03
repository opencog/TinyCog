/*
  Project: OpenCogER
  File: ITEqualizeHist.hpp
  Author: Mandeep Singh Bhatia
  License: AGPL
  Date: May 2017
*/

//This is gray image to equalized image transform
//Images are expected in opencv2 MAT format

#ifndef ITEQUALIZEHIST_H
#define ITEQUALIZEHIST_H

#include "sense/vision/ImageTransform.hpp"

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>
#include <string>

//#include <vector>
//#include <memory>//shared_ptr

using namespace std;
using namespace cv;

class ITEqualizeHist: public ImageTransform<Mat,Mat>
{
	public:
	ITEqualizeHist(string t_name):ImageTransform(t_name){}
	virtual ~ITEqualizeHist(){}
	
	virtual Mat Transform(Mat in);
};

#endif
