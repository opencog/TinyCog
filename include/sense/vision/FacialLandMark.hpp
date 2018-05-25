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

#define LEU1 18 
#define LEU2 19
#define LEB1 22
#define LEB2 21
#define LELS 17
#define LERS 20

#define REU1 4
#define REU2 5
#define REB1 8
#define REB2 7
#define RELS 3
#define RERS 6


#define MCU 14
#define MCB 13
#define MRS 15
#define MLS 12

#define NO_FLM_PTS 26 //shape_predictor_26_face_landmarks.dat

typedef dlib::full_object_detection facial_lms;


class FacialLandMark 
{
    public:
        const std::string shapes_file="shape_predictor_26_face_landmarks.dat";
	FacialLandMark();
	~FacialLandMark(){}
	
	//a function to return landmarks
	void get_lm_points(cv::Mat &, std::vector<cv::Rect> &, std::vector<facial_lms> &);
	
	
    private:
        dlib::array2d<dlib::bgr_pixel> d_img;
	std::vector<dlib::rectangle> dl_rect;
	dlib::shape_predictor dsp;
};

#endif //FACIALLANDMARK_H_
