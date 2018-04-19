/*
  Project: OpenCogER
  File: RaspiFingersCountTest.cpp
  Author: Dagim Sisay <dagiopia@gmail.com>
  License: AGPL
  Date: March, 2018
*/

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <signal.h>

#include "sense/vision/RaspiCamCapture.hpp"
#include "sense/vision/ITColor2Gray.hpp"
#include "sense/vision/ITEqualizeHist.hpp"
#include "sense/vision/ITDetectHand.hpp"
#include "sense/vision/FingersCount.hpp"

#define SCALE 0.1

#define NO_GUI 


float avg_time_pf, en_time, acc, avg_oh, avg_nh;
uint64_t st_time;
int n_f, nh, oh;


void print_report()
{
    avg_time_pf = acc / n_f;
    avg_nh /= nh;
    avg_oh /= oh;
    printf("\nAverage Time per Frame (no Hand): %f\n", avg_nh);
    printf("Average Time per Frame (on Hand): %f\n", avg_oh);
    printf("Average Time per Frame: %f\n\n", avg_time_pf);
}

void singint_handler(int sig)
{
    std::cout<<"Caught SIGINT"<<std::endl;
    print_report();
    exit(0);
}



int main(int argc, char **argv)
{    
    avg_nh = avg_oh = avg_time_pf = 0; 
    oh = nh = n_f = 0;
    signal(SIGINT, singint_handler);
    RaspiCamCapture cc("c", 320, 240, 20);
    if( !cc.isOk()) { fprintf(stderr, "ERROR: Camera Opening : %s\n", 
                             cc.getState().c_str()); return -1; }
    ITColor2Gray c2g("c2g1");
    ITEqualizeHist eh("eh1");
    ITDetectHand dh("dh1");
    FingersCount fc(true);

    cv::Mat frame, img, img2;
    std::vector<cv::Rect> hands;
    std::vector<std::string> snum({"one", "two", "three", "four", "five", "si", "sev"});

    bool hand_found = false;
    int n_fings;

    while (true)
    {
        st_time = getTickCount();
        frame = cc.getCurrentFrame();
        img = eh.Transform(c2g.Transform(frame));
	cv::threshold(img, img, 70, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);
	hands = dh.Transform(img);
	for (size_t i = 0 ; i < hands.size(); i++)
	{
            hand_found = true;
	    img = cv::Mat(img, cv::Rect(hands[i].x - hands[i].x*SCALE,
	                                hands[i].y - hands[i].y*SCALE,
					hands[i].width + hands[i].width*SCALE,
					hands[i].height + hands[i].height*SCALE));
	    n_fings = fc.num_fingers(img);
	    #ifndef NO_GUI
	    	cv::putText(frame, snum[n_fings], cv::Point(20, 20), 
	        	      cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, CV_RGB(50, 50, 50), 2,
		      	      CV_AA);
	    	cv::cvtColor(img, img2, COLOR_GRAY2BGR);
	    	for(size_t j = 0 ; j < fc.f_tips.size(); j++)
	        	cv::circle(img2, fc.f_tips[j], 2, CV_RGB(255, 0, 0), 2);
	    	cv::imshow("hand", img2);
	    #endif //NO_GUI
            
	}
	#ifndef NO_GUI
	    cv::imshow("Fingers Count", frame);
	    if((int)'q' == cv::waitKey(10))
	        break;
	#endif //NO_GUI
	en_time = (float)((getTickCount() - st_time) / getTickFrequency());
	//printf("Hand State: %d, No Fingers: %d. Frame #%d, Time: %f\n", hand_found, n_fings, n_f, en_time);
	if (hand_found){
		avg_oh += en_time;
		oh++;
	}
	else {
		avg_nh += en_time;
		nh++;
	}
	acc += en_time;
	n_f++;
	n_fings = 0;
	hand_found = false;
    }// while true
    print_report();
    return 0;
}
