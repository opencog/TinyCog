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

//eye points
#define LEU1 37 //left eye upper 1
#define LEU2 38
#define LEB1 41 //left eye bottom 1
#define LEB2 40
#define LELS 36 //left eye left side
#define LERS 39 //left eye right side
               
#define REU1 43
#define REU2 44
#define REB1 47
#define REB2 46
#define RELS 42
#define RERS 45

//mouth
#define MCU1 50 // mouth center upper 1
#define MCU2 51
#define MCU3 52
#define MCB1 58 // mouth center bottom 1
#define MCB2 57
#define MCB3 56
#define MRS 54  // mouth right side
#define MLS 48  // mouth left side


/*
XXX bad news! https://github.com/davisking/dlib-models
	the landmarking shape file I used here can't be 
	used for reasons in the link above... 
	find another one.
*/


typedef dlib::full_object_detection facial_lms;


class FacialLandMark 
{
    public:
        const std::string shapes_file="shape_predictor_68_face_landmarks.dat";
	FacialLandMark();
	~FacialLandMark(){}
	
	//a function to return landmarks
	void get_lm_points(cv::Mat, std::vector<cv::Rect>, std::vector<facial_lms> *);
	
	
    private:
        dlib::array2d<dlib::bgr_pixel> d_img;
	std::vector<dlib::rectangle> dl_rect;
	dlib::shape_predictor dsp;
};

#endif //FACIALLANDMARK_H_
