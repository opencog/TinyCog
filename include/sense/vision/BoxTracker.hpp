/*
  Project: OpenCogER
  File: BoxTracker.hpp
  Author: Mandeep Singh Bhatia
  License: AGPL
  Date: May 2017
*/

//This is tracker wrapper
//Images are expected in opencv2 Mat format

#ifndef BOXTRACKER_H
#define BOXTRACKER_H
#include <opencv2/core/utility.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/videoio.hpp>

#include <iostream>
#include <string>

using namespace std;
using namespace cv;

class BoxTracker
{
	public:
	enum tracker_type{BOOSTING=0,MEDIAN_FLOW,TLD};
	BoxTracker(Mat init_frame,Rect2d init_box,tracker_type tt=tracker_type::MEDIAN_FLOW);
	~BoxTracker();
	bool isOk(){return ok;}
	bool update(Mat image,Rect2d& box);
	private:
	bool ok;
	Ptr<Tracker> tracker;
};
#endif
