/*
  Project: OpenCogER
  File: FacialLandMark.hpp
  Author: Dagim Sisay
  License: AGPL
  Date: November 2017
*/


#ifndef FACIALLANDMARK_H_
#define FACIALLANDMARK_H_

#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_io.h>
#include <dlib/opencv.h>

#include <iostream>
#include <vector>
#include <stdint.h>

#include "sense/vision/ITDetectFace.hpp"
#include "sense/vision/ITColor2Gray.hpp"
#include "sense/vision/ITEqualizeHist.hpp"

typedef dlib::full_object_detection facial_lms;


//using namespace std;
//using namespace cv;

class FacialLandMark 
{
	public:
		const std::string shapes_file="shape_predictor_68_face_landmarks.dat";
		FacialLandMark();
		virtual ~FacialLandMark(){}

		//a function to return landmarks
		std::vector<facial_lms> get_lm_points(cv::Mat);
		//a function to indicate when come gesture is active


	private:
		dlib::array2d<dlib::bgr_pixel> d_img;                           
		std::vector<cv::Rect> cv_rect;                           
		std::vector<dlib::rectangle> dl_rect;                    
		dlib::shape_predictor dsp;
		dlib::full_object_detection f_landmark;
		std::vector<dlib::full_object_detection> fs_landmark;
		ITColor2Gray c_2_gray("c_2_g");
		ITEqualizeHist e_hist("e_hist");
		ITDetectFace f_detector("f_detector_lm");
};


#endif //FACIALLANDMARK_H_
