/*
  Project: OpenCogER
  File: FaceNoddingDetection.cpp
  Author: Dagim Sisay
  License: AGPL
  Date: November 2017
*/

#include <iostream>
#include <cstring>
#include <list>


#include <dlib/statistics/statistics.h>
#include <dlib/statistics/running_gradient.h>

#include "sense/vision/CamCapture.hpp"
#include "sense/vision/ITColor2Gray.hpp"
#include "sense/vision/ITEqualizeHist.hpp"
#include "sense/vision/ITDetectFace.hpp"
#include "sense/vision/FacialLandMark.hpp"

#include "util/util.hpp"

#define MODEL_FILE "shape_predictor_68_face_landmarks.dat"

//#define DBG 
 
#define FRAME_RATE 20
#define WINDOW FRAME_RATE*3
#define SPACE FRAME_RATE/10
#define SAMPLE_VER FRAME_RATE*5


int main(int argc, char** argv)
{    
    CamCapture *cc = CamCapture::init("cam1",320,240,0,FRAME_RATE);
    if (!cc->isOk()){std::cout<<std::endl<<cc->getState()<<std::endl;return -1;}
    
    cv::Mat frame, image;
    ITColor2Gray c2g("c2g1");
    ITEqualizeHist eh("eh1");
    ITDetectFace fcd("fc1");
    FacialLandMark flm;

    std::vector<cv::Rect> faces; 
    
    std::vector<facial_lms> f_lms; //landmarks of faces detected 

    #ifdef DBG 
    	dlib::image_window win;
    #endif

    int p = 0, q = 0;

    std::list<int> ver(WINDOW, 0);
    std::list<int> hor(WINDOW, 0);
    
    double vrc = 0, upping = 0, downing = 0, siding = 0;
    std::vector<double> vvrc;
    std::vector<double> vver;
    double arr[] = {123,123,120,120,116,114,114,116,121,121,126,126,125,125,120,
                    120,116,116,116,116,120,120,127,127,130,130,128,128,122,122,
		    117,117,117,117,120,120,127,129,129,127,127,122,122,120,120,
		    121,121,123,123,124,124,122};
    double arr2[] = {125, 125, 122, 116, 116, 110, 110, 107, 109, 109, 115, 115,
                     122, 127, 127, 120, 113, 110, 110, 111, 111, 117, 117, 129, 
		     129, 138, 138, 138, 138, 129, 129, 121, 121, 117, 117, 118, 
		     118, 128, 128, 137, 137, 143, 140, 140, 132, 124, 120, 120, 
		     121, 121, 126, 132, 135, 132, 132, 127, 127, 120, 118, 118, 
		     119, 119, 123, 130, 130, 131, 131, 127, 124, 124, 122, 122, 
		     123, 123, 124, 124, 125, 125, 125, 125, 124, 124};

    std::vector<double> cdata(std::begin(arr), std::end(arr));
    std::vector<double> cdata2(std::begin(arr2), std::end(arr2));
    std::vector<double> template1;
    std::vector<double> template2;
    cherry_pick(&cdata, &template1);
    cherry_pick(&cdata2, &template2);
    
    while(true)
    {
    	frame = cc->getCurrentFrame();
	frame.copyTo(image);
	faces = fcd.Transform(eh.Transform(c2g.Transform(frame)));
	flm.get_lm_points(image, faces, f_lms); //get lms

	for (uint8_t idx = 0; idx < f_lms.size(); idx++)
	{
		facial_lms shape = f_lms[idx];
		
		#ifdef DBG
			//if(upping > 0.4)
			std::cout<< shape.part(0).x() << "," << shape.part(16).x() << "," << shape.part(48).x() << "," 
				<< shape.part(54).x() << "," << shape.part(33).x() << "," << shape.part(36).x() << "," 
				<< shape.part(45).x() << "," << upping << "," << siding << ","
				<<vrc<<std::endl;
		#endif

		ver.pop_front(); hor.pop_front();
		ver.push_back(shape.part(35).y());
		hor.push_back(shape.part(35).x());
	}


	if(q % SPACE == 0)
	{
	    std::vector<double> ver_temp(std::begin(ver), std::end(ver));
	    std::vector<double> hor_temp(std::begin(hor), std::end(hor));
	    std::vector<double> vdata;
	    std::vector<double> hdata;
	    cherry_pick(&ver_temp, &vdata);
	    cherry_pick(&hor_temp, &hdata);
	    make_equal(&template1, vdata.size());
	    make_equal(&template2, vdata.size());
	    normalize(&vdata); normalize(&hdata);
	    upping = dlib::correlation(vdata, template1);
	    downing = dlib::correlation(vdata, template2);
	    upping = (upping+downing)/2.0;
	    if(upping > 0.42)
	    	printf("Vertical Nodding %.4f\n", upping);
	    upping = dlib::correlation(hdata, template1);
	    downing = dlib::correlation(hdata, template2);
	    siding = (upping+downing)/2.0;
	    if(siding > 0.4)
	    	printf("Horizontal Nodding %.4f\n", siding);
	    	
	    q = 0;
	}
	
	#ifdef DBG
		dlib::array2d<dlib::bgr_pixel> img;
		dlib::assign_image(img, dlib::cv_image<dlib::bgr_pixel>(image));
		win.clear_overlay();
		win.set_image(img);
		win.add_overlay(dlib::render_face_detections(f_lms));
	#endif
	q++;
    }
    return 0;
}
