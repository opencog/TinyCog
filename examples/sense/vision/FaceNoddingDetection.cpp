/*
  Project: OpenCogER
  File: FaceLandmarkTest.cpp
  Author: Dagim Sisay
  License: AGPL
  Date: November 2017
*/

#include <iostream>
#include <cstring>
#include <list>

#include <dlib/statistics/statistics_abstract.h>

#include "sense/vision/CamCapture.hpp"
#include "sense/vision/ITColor2Gray.hpp"
#include "sense/vision/ITEqualizeHist.hpp"
#include "sense/vision/ITDetectFace.hpp"
#include "sense/vision/FacialLandMark.hpp"

#define MODEL_FILE "shape_predictor_68_face_landmarks.dat"

 
#define FRAME_RATE 20
#define WINDOW 3*FRAME_RATE
#define SPACE FRAME_RATE/2


double variance(std::list<int> d)
{
    int m = 0;
    double v = 0;
    std::list<int>::const_iterator i = d.begin();
    for( ; i != d.end(); i++)
        m += *i; 
    m = m/d.size();
    for(i = d.begin() ; i != d.end(); i++)
        v += std::pow((double)(*i - m), 2.0);
    return (v/d.size());
}

   
int main(int argc, char** argv)
{    
    CamCapture cc("cam1",320,240,0,FRAME_RATE);
    if (!cc.isOk()){std::cout<<std::endl<<cc.getState()<<std::endl;return -1;}
    
    cv::Mat frame, image;
    ITColor2Gray c2g("c2g1");
    ITEqualizeHist eh("eh1");
    ITDetectFace fcd("fc1");
    FacialLandMark flm;

    std::vector<cv::Rect> faces; 
    
    std::vector<facial_lms> f_lms; //landmarks of faces detected 

    dlib::image_window win;

    int p = 0, q, t = 0;

    std::list<int> ver(WINDOW, 0);
    std::list<int> hor(WINDOW, 0);
    
    
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
	
	f_lms = flm.get_lm_points(image, faces); //get lms

	/*
		get a few sample points from landmark, compare values with subsecuent frames.
	*/
	
	
	for (uint8_t idx = 0; idx < f_lms.size(); idx++)
	{
		facial_lms shape = f_lms[idx];
		//std::cout<< shape.part(0).y() << " " << shape.part(16).y() << " " << shape.part(48).y() << " " 
		//	<< shape.part(54).y() << " " << shape.part(33).y() << " " << shape.part(36).y() << " " 
		//	<< shape.part(45).y() << " " << shape.part(19).y() << " " << shape.part(24).y() << std::endl;
			

		ver.pop_front(); hor.pop_front();
		ver.push_back(shape.part(35).y());
		hor.push_back(shape.part(35).x());

		//std::vector<int> vver{ std::begin(ver), std::end(ver)};
	}


	if(t % SPACE == 0)
	{
	    double cov = variance(ver);
	    printf("%f\n", cov);
	    t = 0;
	}


	dlib::array2d<dlib::bgr_pixel> img;
	dlib::assign_image(img, dlib::cv_image<dlib::bgr_pixel>(image));
	win.clear_overlay();
	win.set_image(img);
	win.add_overlay(dlib::render_face_detections(f_lms));
	t++;
    }
    return 0;
}
