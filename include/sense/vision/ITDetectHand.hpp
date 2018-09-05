/*
  Project: OpenCogER
  File: ITDetectHand.hpp
  Author: Dagim Sisay
  License: AGPL
  Date: March, 2018
*/

#ifndef ITDETECTHAND_H_
#define ITDETECTHAND_H_

#include "ImageTransform.hpp"

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/objdetect.hpp"

#include <vector>

class ITDetectHand: public ImageTransform<cv::Mat,std::vector<cv::Rect> >
{
	public:
	const std::string cascade_file="haarcascade_hand.xml";
	ITDetectHand(std::string t_name);//:ImageTransform(t_name){}
	virtual ~ITDetectHand(){}
	
	virtual std::vector<cv::Rect> Transform(cv::Mat in);
	private:
	cv::CascadeClassifier it_cascade;
};

#endif //ITDETECTHAND_H_
