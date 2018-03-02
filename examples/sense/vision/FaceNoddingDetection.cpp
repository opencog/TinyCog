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

#define MODEL_FILE "shape_predictor_68_face_landmarks.dat"

//#define DEBUG 
 
#define FRAME_RATE 20
#define WINDOW FRAME_RATE*3
#define SPACE FRAME_RATE/10
#define SAMPLE_VER FRAME_RATE*5


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

void normalize(std::vector<double> *s, int a = 0, int b = 1)
{
    int c = b - a;
    auto mm = std::minmax_element(s->begin(), s->end());
    double mi = s->at(mm.first - s->begin());
    double mx = s->at(mm.second - s->begin());
    double mxi = mx - mi;

    std::vector<double>::iterator i = s->begin(); 
    for( ; i != s->end(); i++)
    	*i = a + (((*i - mi)*c) / mxi);
}

void cherry_pick(std::vector<double> *x, std::vector<double> *y, uint8_t space = 2)
{
    for(size_t i = 0; i < x->size(); i++)
        if(i % space == 0)
	    y->push_back(x->at(i));
}

void make_equal(std::vector<double> *x, size_t size)
{
    size_t xs = x->size();
    std::vector<double> t(*x);
    std::vector<double>::const_iterator itb = t.begin(); 
    std::vector<double>::iterator itbx = x->begin(),
				itex = x->end();
    if(xs < size)
        for(size_t i = xs; i < size; i++)
        {
            if(i % xs == 0)
	        itb = t.begin();
	    x->push_back(*itb);
	    itb++;
        }
    else
        x->erase(itbx+size, x->end());

}
 
void conv(std::vector<double> *x, std::vector<double> *h, std::vector<double> *y)
{
    //std::vector<double>::const_iterator xi = x->begin(), hi = h->begin();
    //std::vector<double>::iterator yi = y->begin();
    size_t xs = x->size(), hs=h->size();
    size_t xh = xs-hs;
    double temp = 0;
    for (int i = 0; i <= xh; i++)
    {
        temp=0;
	for(int k = 0; k < hs; k++)
	    temp += x->at(i+k) * h->at(hs-k-1);
	y->push_back(temp);
    }
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

    int p = 0, q = 0;

    std::list<int> ver(WINDOW, 0);
    std::list<int> hor(WINDOW, 0);
    
    double vrc = 0, upping = 0, downing = 0, siding = 0;
    std::vector<double> vvrc;
    std::vector<double> vver;
    double arr[] = {123,123,120,120,116,114,114,116,121,121,126,126,125,125,120,120,116,116,116,116,120,120,127,127,130,130,128,128,122,122,117,117,117,117,120,120,127,129,129,127,127,122,122,120,120,121,121,123,123,124,124,122};
    double arr2[] = {125, 125, 122, 116, 116, 110, 110, 107, 109, 109, 115, 115, 122, 127, 127, 120, 113, 110, 110, 111, 111, 117, 117, 129, 129, 138, 138, 138, 138, 129, 129, 121, 121, 117, 117, 118, 118, 128, 128137, 137, 143, 140, 140, 132, 124, 120, 120, 121, 121, 126, 132, 135, 132, 132, 127, 127, 120, 118, 118, 119, 119, 123, 130, 130, 131, 131, 127, 124, 124, 122, 122, 123, 123, 124, 124, 125, 125, 125, 125, 124, 124};
    std::vector<double> cdata(std::begin(arr), std::end(arr));
    std::vector<double> cdata2(std::begin(arr2), std::end(arr2));
    std::vector<double> template1;
    std::vector<double> template2;
    cherry_pick(&cdata, &template1);
    cherry_pick(&cdata2, &template2);
    
    while(true)
    {
    	frame = cc.getCurrentFrame();
	frame.copyTo(image);
	faces = fcd.Transform(eh.Transform(c2g.Transform(frame)));
	f_lms = flm.get_lm_points(image, faces); //get lms

	for (uint8_t idx = 0; idx < f_lms.size(); idx++)
	{
		facial_lms shape = f_lms[idx];
		
		#ifdef DEBUG
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
	
	#ifdef DEBUG
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
