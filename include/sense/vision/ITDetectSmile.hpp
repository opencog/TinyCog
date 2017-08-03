/*
  Project: OpenCogER
  File: ITDetectSmile.hpp
  Author: Mandeep Singh Bhatia
  License: AGPL
  Date: May 2017
*/

//This is image to vector<Rect> image transform
//Images are expected in opencv2 MAT format

#ifndef ITDETECTSMILE_H
#define ITDETECTSMILE_H

#include "sense/vision/ImageTransform.hpp"

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/objdetect.hpp"

#include <iostream>
#include <string>

#include <vector>
//#include <memory>//shared_ptr

using namespace std;
using namespace cv;

class ITDetectSmile: public ImageTransform<Mat,vector<Rect>>
{
	public:
	const string cascade_file="haarcascade_smile.xml";
	ITDetectSmile(string t_name);//:ImageTransform(t_name){}
	virtual ~ITDetectSmile(){}
	
	virtual vector<Rect> Transform(Mat in);
	private:
	CascadeClassifier it_cascade;
};

#endif //ITDETECTFACE_H
