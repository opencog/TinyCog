/*
  Project: OpenCogER
  File: SaliencyTest2.cpp
  Author: Dagim Sisay
  License: AGPL
  Date: April, 2018
*/

#include "sense/vision/CamCapture.hpp"
#include "sense/vision/DSaliency.hpp"

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
	CamCapture cc("c1", 320, 240, 0, 20);
	if (!cc.isOk()) { 
		fprintf(stderr, "%s\n", cc.getState().c_str()); 
		return -1;
	}

	Mat frame, sal;
	DSaliency sal_d(SAL_STATIC, SAL_SPECTRAL_RESIDUAL);
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
		sal.convertTo(sal, CV_8UC1, 255.0);
		threshold(sal, sal, 100, 255, THRESH_BINARY);
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
			circle(frame, cent, rad, CV_RGB(0, 255, 0), 3);
		}
		imshow("saliencytest2", frame);
		if( waitKey(10) == 27)
			break;
	}

return 0;
}
