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

/*
	For multiple faces
*/
void FacialLandMark::get_lm_points(cv::Mat &img, std::vector<cv::Rect> &cv_rect, std::vector<facial_lms> &dest)
{
	dl_rects.clear();
	
	for(uint8_t idx = 0; idx < cv_rect.size(); idx++)
	{
		cv::Rect tr = cv_rect[idx];
		dl_rects.push_back(dlib::rectangle((long)tr.tl().x, (long)tr.tl().y, 
														(long)tr.br().x-1, (long)tr.br().y-1));
	}
	
	dlib::assign_image(d_img, dlib::cv_image<dlib::bgr_pixel>(img));
	
	for(uint8_t idx = 0; idx < dl_rects.size(); idx++) //iterate on faces
		dest.push_back(dsp(d_img, dl_rects[idx]));
}

/*
	For a single face
*/
void FacialLandMark::get_lm_points(cv::Mat &img, cv::Rect &cv_rect, facial_lms &dest)
{
	dl_rect = dlib::rectangle((long)cv_rect.tl().x, (long)cv_rect.tl().y, 
										(long)cv_rect.br().x-1, (long)cv_rect.br().y-1);
	
	dlib::assign_image(d_img, dlib::cv_image<dlib::bgr_pixel>(img));
	dest = dsp(d_img, dl_rect);
}
