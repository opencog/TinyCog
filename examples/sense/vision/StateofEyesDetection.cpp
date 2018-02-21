/*
  Project: OpenCogER
  File: StateofEyesDetectionTest.cpp
  Author: Dagim Sisay
  License: AGPL
  Date: November 2017
*/

#include <iostream>
#include <fstream>
#include <cstring>
#include <signal.h>
#include <cmath>

#include "sense/vision/CamCapture.hpp"
#include "sense/vision/ITColor2Gray.hpp"
#include "sense/vision/ITEqualizeHist.hpp"
#include "sense/vision/ITDetectFace.hpp"
#include "sense/vision/FacialLandMark.hpp"

#define MODEL_FILE "shape_predictor_68_face_landmarks.dat"

std::ofstream ofile;


void trial()
{
	printf("Just a trial... \n");
}

void sigint_handler(int SIG)
{
	std::cout<<"Caught SIGINT.................."<<std::endl;
	ofile.close();
	exit(1);
}

  
int main(int argc, char** argv)
{    
    CamCapture cc("cam1",320,240,0,20);
    if (!cc.isOk()){std::cout<<std::endl<<cc.getState()<<std::endl;return -1;}
    
    cv::Mat frame, image;
    ITColor2Gray c2g("c2g1");
    ITEqualizeHist eh("eh1");
    ITDetectFace fcd("fc1");
    FacialLandMark flm;
    
    flm.eye_status(&trial);

    std::vector<cv::Rect> faces; 
    
    std::vector<facial_lms> f_lms; //landmarks of faces detected 

    dlib::image_window win;
    
    signal(SIGINT, sigint_handler);

    bool eyes_open = true, prev_status = true; 

    while(true)
    {
    	frame = cc.getCurrentFrame();
	frame.copyTo(image);
	faces = fcd.Transform(eh.Transform(c2g.Transform(frame)));
	
	f_lms = flm.get_lm_points(image, faces); //get lms
	
	for (uint8_t idx = 0; idx < f_lms.size(); idx++)
	{
		facial_lms shape = f_lms[idx];
		//std::cout<<"******************** Face #"<<idx<<std::endl;
		std::vector<double> pe1, pe2;
		//std::cout<<"Points:"<<std::endl;
		for(int i = 36; i <= 41; i++){
			pe1.push_back(std::sqrt((double) std::pow((double)shape.part(i).x(), 2) + 
								std::pow((double)shape.part(i).y(), 2)));
			pe2.push_back(std::sqrt((double) std::pow((double)shape.part(i+6).x(), 2) + 
								std::pow((double)shape.part(i+6).y(), 2)));
		}
		double ear1, ear2, ear;
		ear1 = (std::abs(pe1[1] - pe1[5]) + std::abs(pe1[2] - pe1[4])) / (2 * std::abs(pe1[0] - pe1[3]));
		ear2 = (std::abs(pe2[1] - pe2[5]) + std::abs(pe2[2] - pe2[4])) / (2 * std::abs(pe2[0] - pe2[3]));
		ear = (ear1+ear2)/2;

		eyes_open = (ear < 0.09098 )? false : true;
		if (ear < 0.09)
			eyes_open = false;
		if (ear > 0.12)
			eyes_open = true;

		if(eyes_open != prev_status){
			std::cout<<"Eyes "<<((eyes_open)? "Open" : "Closed")<<std::endl;
			prev_status = eyes_open;
		}
		
	}

	dlib::array2d<dlib::bgr_pixel> img;
	dlib::assign_image(img, dlib::cv_image<dlib::bgr_pixel>(image));
	win.clear_overlay();
	win.set_image(img);
	win.add_overlay(dlib::render_face_detections(f_lms));

    }
    return 0;
}
