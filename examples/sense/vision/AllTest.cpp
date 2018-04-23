/*
  Project: OpenCogER
  File: AllTest.cpp
  Author: Dagim Sisay
  License: AGPL
  Date: April, 2018
*/


#include <iostream>
#include <string>
#include <signal.h>

#include "sense/vision/CamCapture.hpp"
#include "sense/vision/ITColor2Gray.hpp"
#include "sense/vision/ITEqualizeHist.hpp"
#include "sense/vision/ITDetectFace.hpp"
#include "sense/vision/ITDetectSmile.hpp"
#include "sense/vision/FacialLandMark.hpp"
#include "sense/vision/ITDetectHand.hpp"
#include "sense/vision/FingersCount.hpp"
#include "sense/vision/DSaliency.hpp"
#include "sense/vision/BoxTrackerThread.hpp"


#define _NEED_GUI_
#define _NEED_TIME_INFO_

#define NO_FLMS 26
#define SCALE 0.1

using namespace std;
using namespace cv;

void sigint_handler(int sig)
{
	cout<<"Caught SIGINT... Exiting!\n";
	exit(0);
}


int main(int argc, char **argv)
{
	signal(SIGINT, sigint_handler);
	CamCapture cc("cam", 320, 240, 0, 20);
	if (!cc.isOk()){std::cout<<std::endl<<cc.getState()<<std::endl;return -1;}

	Mat frame, img, img2;

	ITColor2Gray c2g("c2g1");
	ITEqualizeHist eh("eh1");
	ITDetectFace fcd("fcd1");
	ITDetectSmile smd("smd1");
	FacialLandMark flm;
	ITDetectHand dh("dh1");
	FingersCount fc(true);
	DSaliency sal_d(SAL_STATIC, SAL_FINE_GRAINED);

	vector <Rect> faces, hands;
	vector <facial_lms> f_lms;
	facial_lms shape;
	Rect2d box2d;
	Rect box;
	vector<vector<Point> > cntrs;
	vector<Vec4i> hier;


	BoxTrackerThread *bt;

	while (true)
	{
		frame = cc.getCurrentFrame();
		img = eh.Transform(c2g.Transform(frame));
		faces = fcd.Transform(img);
		
		f_lms.clear();
		flm.get_lm_points(frame, faces, &f_lms); 
		for (int i = 0; i < f_lms.size(); ++i){
			shape = f_lms[i];
			for (int j = 0; j < NO_FLMS; ++j)
				circle(frame, Size(shape.part(j).x(), shape.part(j).y()), 1, 
				       CV_RGB(0, 255, 0), 1.5);
		}
		threshold(img, img, 70, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);
		hands = dh.Transform(img);
		for (int i = 0; i < hands.size(); ++i){
			img2 = Mat(img, Rect(hands[i].x - hands[i].x*SCALE,
					    hands[i].y - hands[i].y*SCALE,
					    hands[i].width + hands[i].width*SCALE,
					    hands[i].height + hands[i].height*SCALE));
			putText(frame, to_string(fc.num_fingers(img2)), Point(20, 20),
				FONT_HERSHEY_COMPLEX_SMALL, 1.5, CV_RGB(255, 0, 0), 2, CV_AA);
			cvtColor(img2, img2, COLOR_GRAY2BGR);
			for(size_t j = 0; j < fc.f_tips.size(); ++j) 
				circle(img2, fc.f_tips[j], 2, CV_RGB(255, 0, 0), 2);
			imshow("hand", img2);
		}
		imshow("feed", frame);
		if(27 == waitKey(10))
			break;
	} // while (true)


return 0;
}
