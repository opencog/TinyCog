/*
  Project: OpenCogER
  File: ITColor2Gray.hpp
  Author: Mandeep Singh Bhatia
  License: AGPL
  Date: May 2017
*/

//This is color 2 gray image transform
//Images are expected in opencv2 MAT format

#ifndef ITCOLOR2GRAY_H
#define ITCOLOR2GRAY_H

#include "sense/vision/ImageTransform.hpp"

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <string>

//#include <vector>
//#include <memory>//shared_ptr

using namespace std;
using namespace cv;

class ITColor2Gray: public ImageTransform<Mat,Mat>
{
	public:
	ITColor2Gray(string t_name):ImageTransform(t_name){}
	virtual ~ITColor2Gray(){}
	
	virtual Mat Transform(Mat in);
};

#endif

