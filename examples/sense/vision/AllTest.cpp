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

#include "sense/vision/Vision.hpp"


#define _NEED_GUI_
#define _NEED_TIME_INFO_

#define SCALE 0.1

using namespace std;
using namespace cv;


float avg_time_pf, en_time, acc, avg_oh, avg_nh, avg_nf, avg_of;
uint64_t st_time;
int n_f, nh, oh, nf, of;


void print_report()
{
	avg_time_pf = acc / n_f;
	avg_nh /= nh;
	avg_oh /= oh;
	avg_of /= of;
	avg_nf /= nf;
	printf("\nAverage Time per Frame (no Face): %f\n", avg_nf);
	printf("Average Time per Frame (on Face): %f\n", avg_of);
	printf("Average Time per Frame (no Hand): %f\n", avg_nh);
	printf("Average Time per Frame (on Hand): %f\n", avg_oh);
	printf("Average Time per Frame: %f\n\n", avg_time_pf);
}


void sigint_handler(int sig)
{
	cout<<"Caught SIGINT... Exiting!\n";
	#ifdef _NEED_TIME_INFO_
	print_report();
	#endif //_NEED_TIME_INFO_
	exit(0);
}


int main(int argc, char **argv)
{
	#ifdef _NEED_TIME_INFO_
	avg_nh = avg_oh = avg_of = avg_nf = avg_time_pf = 0;
	oh = nh = of = nf = n_f = 0;
	#endif //_NEED_TIME_INFO_
	bool face_found = false;
	bool hand_found = false;

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

	int nfings = 0;

	while (true)
	{
		#ifdef _NEED_TIME_INFO_ 
		st_time = getTickCount();
		#endif //_NEED_TIME_INFO_
		frame = cc.getCurrentFrame();
		img = eh.Transform(c2g.Transform(frame));
		faces = fcd.Transform(img);
		
		f_lms.clear();
		flm.get_lm_points(frame, faces, f_lms); 
		for (int i = 0; i < f_lms.size(); ++i){
			face_found = true;
			shape = f_lms[i];
			#ifdef _NEED_GUI_
			for (int j = 0; j < shape.num_parts(); ++j)
				circle(frame, Size(shape.part(j).x(), shape.part(j).y()), 1, 
				       CV_RGB(0, 255, 0), 1.5);
			#endif //_NEED_GUI_
		}
		threshold(img, img, 70, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);
		hands = dh.Transform(img);
		for (int i = 0; i < hands.size(); ++i){
			hand_found = true;
			img2 = Mat(img, Rect(hands[i].x - hands[i].x*SCALE,
					    hands[i].y - hands[i].y*SCALE,
					    hands[i].width + hands[i].width*SCALE,
					    hands[i].height + hands[i].height*SCALE));
			nfings = fc.num_fingers(img2);
			#ifdef _NEED_GUI_
			putText(frame, to_string(nfings), Point(20, 20),
				FONT_HERSHEY_COMPLEX_SMALL, 1.5, CV_RGB(255, 0, 0), 2, CV_AA);
			cvtColor(img2, img2, COLOR_GRAY2BGR);
			for(size_t j = 0; j < fc.f_tips.size(); ++j) 
				circle(img2, fc.f_tips[j], 2, CV_RGB(255, 0, 0), 2);
			imshow("hand", img2);
			#endif //_NEED_GUI_
		}
		#ifdef _NEED_GUI_
		imshow("feed", frame);
		if(27 == waitKey(10))
			break;
		#endif //_NEED_GUI_
		
		#ifdef _NEED_TIME_INFO_
		en_time = (float)((getTickCount() - st_time) / getTickFrequency());
		if (hand_found){ avg_oh += en_time; oh++; }
		else { avg_nh += en_time; nh++; }
		if (face_found){ avg_of += en_time; of++; }
		else { avg_nf += en_time; nf++; }
		acc += en_time;
		n_f++;
		#endif //_NEED_TIME_INFO_

		face_found = false;
		hand_found = false;
	} // while (true)

	#ifdef _NEED_TIME_INFO_
	print_report();
	#endif //_NEED_TIME_INFO_

return 0;
}
