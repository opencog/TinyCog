/*
  Project: OpenCogER
  File: SaliencyTest.cpp
  Author: Dagim Sisay
  License: AGPL
  Date: April, 2018
*/

#include "sense/vision/CamCapture.hpp"
#include "sense/vision/DSaliency.hpp"

#include <signal.h>

using namespace std;
using namespace cv;


void sigint_handler(int sig)
{
	printf("Caught SIGINT... Exiting\n");
	exit(0);
}


int main(int argc, char **argv)
{
	signal(SIGINT, sigint_handler);
	CamCapture cc("c1", 320, 240, 0, 20);
	if (!cc.isOk()) { 
		fprintf(stderr, "%s\n", cc.getState().c_str()); 
		return -1;
	}

	Mat frame, sal;
	DSaliency sal_d(SAL_STATIC, SAL_FINE_GRAINED);
	vector<vector<Point> > cntrs;
	vector<Vec4i> hier;
	Point2f cent;
	int largest_cntr = 0;
	double max = 0, area;
	float rad;
	while (true)
	{
		frame = cc.getCurrentFrame();
		sal_d.update(frame, sal);
		imshow("sal", sal);
		threshold(sal, sal, 100, 255, CV_THRESH_BINARY);
		imshow("t", sal);
		findContours(sal, cntrs, hier, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
		if(cntrs.size() > 0){
			largest_cntr = 0;
			max = 0;
			for(size_t idx = 0; idx < cntrs.size(); idx++){
				area = contourArea(cntrs[idx]);
				if(area > max){
					max = area;
					largest_cntr = idx;
				}
			}
			
			minEnclosingCircle(cntrs[largest_cntr], cent, rad);
			printf("Loc: %f, %f\n", cent.x, cent.y);
			circle(frame, cent, rad, CV_RGB(0, 255, 0), 3);
		}
		imshow("saliencytest", frame);
		if( waitKey(10) == 27)
			break;
	}

return 0;
}
