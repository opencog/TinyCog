/*
  Project: OpenCogER
  File: FacialLandMark.hpp
  Author: Dagim Sisay
  License: AGPL
  Date: November 2017
*/


#ifndef FACIALLANDMARK_H_
#define FACIALLANDMARK_H_

#include <dlib/image_processing.h>
#include <dlib/image_io.h>
#include <dlib/opencv.h>

#include <iostream>
#include <vector>
#include <stdint.h>


/*
	Eye lm position macro
	Left eye landmark starts from 36 to 41
	so LE(0) would be the first point and 
	LE(5) the last.
*/
#define LES 36 
#define LE(x) (LES+x)

#define RES 42
#define RE(x) (RES+x)


typedef dlib::full_object_detection facial_lms;


class FacialLandMark 
{
	public:
		const std::string shapes_file="shape_predictor_68_face_landmarks.dat";
		FacialLandMark();
		~FacialLandMark(){}

		void get_lm_points(cv::Mat &, std::vector<cv::Rect> &, std::vector<facial_lms> &);
		void get_lm_points(cv::Mat &, cv::Rect &, facial_lms &);



	private:
		dlib::array2d<dlib::bgr_pixel> d_img;
		std::vector<dlib::rectangle> dl_rects;
		dlib::rectangle dl_rect;
		dlib::shape_predictor dsp;
};

#endif //FACIALLANDMARK_H_
