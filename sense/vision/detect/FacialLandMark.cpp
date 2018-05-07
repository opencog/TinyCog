/*
  Project: OpenCogER
  File: FacialLandMark.cpp
  Author: Dagim Sisay
  License: AGPL
  Date: November 2017
*/


#include "sense/vision/FacialLandMark.hpp"


FacialLandMark::FacialLandMark()
{
	//instantiate stuff
	dlib::deserialize(shapes_file) >> dsp;
}


void FacialLandMark::get_lm_points(cv::Mat &img, std::vector<cv::Rect> &cv_rect, std::vector<facial_lms> &dest)
{
	dl_rect.clear();
	
	for(uint8_t idx = 0; idx < cv_rect.size(); idx++)
	{
		cv::Rect tr = cv_rect[idx];
		dl_rect.push_back(dlib::rectangle((long)tr.tl().x, (long)tr.tl().y, 
						(long)tr.br().x-1, (long)tr.br().y-1));
	}
	
	dlib::assign_image(d_img, dlib::cv_image<dlib::bgr_pixel>(img));
	
	for(uint8_t idx = 0; idx < dl_rect.size(); idx++) //iterate on faces
		dest.push_back(dsp(d_img, dl_rect[idx]));
}

