/*
  Project: OpenCogER
  File: FaceLandmarkTest.cpp
  Author: Dagim Sisay
  License: AGPL
  Date: November 2017
*/

#include <iostream>
#include <cstring>
#include <signal.h>

#include "sense/vision/CamCapture.hpp"
#include "sense/vision/ITColor2Gray.hpp"
#include "sense/vision/ITEqualizeHist.hpp"
#include "sense/vision/ITDetectFace.hpp"
#include "sense/vision/FacialLandMark.hpp"

#define MODEL_FILE "shape_predictor_68_face_landmarks.dat"

void sigint_handler(int SIG)
{
    std::cout<<"Caught SIGINT"<<std::endl;
    exit(0);
}
   
int main(int argc, char** argv)
{    
    signal(SIGINT, sigint_handler);
    CamCapture cc("cam1",320,240,0,20);
    if (!cc.isOk()){std::cout<<std::endl<<cc.getState()<<std::endl;return -1;}
    
    cv::Mat frame, image;
    ITColor2Gray c2g("c2g1");
    ITEqualizeHist eh("eh1");
    ITDetectFace fcd("fc1");
    FacialLandMark flm;

    std::vector<cv::Rect> faces; 
    
    std::vector<facial_lms> f_lms; //landmarks of faces detected 

    dlib::image_window win;
    
    while(true)
    {
    	frame = cc.getCurrentFrame();
	frame.copyTo(image);

	/*
		It's best to first detect the faces using haar cascade rather than dlib's
		face detector. 
		After getting cv::Rect of faces pass that to the get_lm_points function
		along with the frame so that the cv::Rect gets converted to dlib::rectangle
		and lms can be obtained. 
	*/
	
	faces = fcd.Transform(eh.Transform(c2g.Transform(frame)));
	
	flm.get_lm_points(image, faces, &f_lms); //get lms

	for (uint8_t idx = 0; idx < f_lms.size(); idx++)
	{
		facial_lms shape = f_lms[idx];
		std::cout<<"******************** Face #"<<idx<<std::endl;
		std::cout<<"pixel position of first part:  " << shape.part(0) << std::endl;
		std::cout<<"pixel position of second part: " << shape.part(1) << std::endl;
	}

	dlib::array2d<dlib::bgr_pixel> img;
	dlib::assign_image(img, dlib::cv_image<dlib::bgr_pixel>(image));
	win.clear_overlay();
	win.set_image(img);
	win.add_overlay(dlib::render_face_detections(f_lms));

    }
    return 0;
}
