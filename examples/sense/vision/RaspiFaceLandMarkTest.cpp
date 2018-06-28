/*
  Project: OpenCogER
  File: RaspiFaceLandMarkTest.cpp
  Author: Dagim Sisay
  License: AGPL
  Date: April 2018
*/

#include <iostream>
#include <cstring>
#include <signal.h>

#include "sense/vision/RaspiCamCapture.hpp"
#include "sense/vision/ITColor2Gray.hpp"
#include "sense/vision/ITEqualizeHist.hpp"
#include "sense/vision/ITDetectFace.hpp"
#include "sense/vision/FacialLandMark.hpp"


float avg_of, avg_nf, avg_time_pf, en_time, acc;
uint64_t st_time;
int n_f, of, nf;


void sigint_handler(int SIG)
{
    std::cout<<"Caught SIGINT"<<std::endl;
    avg_time_pf = acc / n_f;
    avg_nf /= nf;
    avg_of /= of;
    printf("\nAverage Time per Frame (no Face): %f\n", avg_nf);
    printf("Average Time per Frame (on Face): %f\n", avg_of);
    printf("Average Time per Frame: %f\n\n", avg_time_pf);
    exit(0);
}
   
int main(int argc, char** argv)
{    
    avg_nf = avg_of = avg_time_pf = 0;
    of = nf = n_f = 0;
    signal(SIGINT, sigint_handler);
    RaspiCamCapture cc("Raspi",320,240,20);
    if (!cc.isOk()){std::cout<<std::endl<<cc.getState()<<std::endl;return -1;}
    
    cv::Mat frame, image;
    ITColor2Gray c2g("c2g1");
    ITEqualizeHist eh("eh1");
    ITDetectFace fcd("fc1");
    FacialLandMark flm;

    std::vector<cv::Rect> faces; 
    
    std::vector<facial_lms> f_lms; //landmarks of faces detected 

    bool face_found = false;
    
    while(true)
    {
        st_time = getTickCount();
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
	f_lms.clear();
	flm.get_lm_points(image, faces, f_lms); //get lms
	for (uint8_t idx = 0; idx < f_lms.size(); idx++)
	{
		face_found = true;
		facial_lms shape = f_lms[idx];
//		for(uint8_t i = 0; i < 26; i++)
//		    cv::circle(frame, cv::Size(shape.part(i).x(), shape.part(i).y()), 
//		               2, CV_RGB(0, 255, 0), 2);
	}
//        cv::imshow("face landmark", frame);
//	if(27 == cv::waitKey(10))
//	    break;
    	en_time = (float)((getTickCount() - st_time) / getTickFrequency());
	printf("Face State: %d, No_Parts: %d, Frame #%d, Time: %f\n", (int)face_found, f_lms.size(),n_f, en_time);
	if (face_found){
		avg_of += en_time;
		of++;
	}
	else{
		avg_nf += en_time;
		nf++;
	}
	acc += en_time;
	n_f++;
	face_found = false;
    }
    return 0;
}
