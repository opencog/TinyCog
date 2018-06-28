/*
  Project: OpenCogER
  File: BoxTracker.cpp
  Author: Mandeep Singh Bhatia
  License: AGPL
  Date: May 2017
*/
#include "sense/vision/BoxTracker.hpp"

BoxTracker::BoxTracker(Mat init_frame,Rect2d init_box,
    tracker_type tt)
{
	ok=false;
	switch(tt)
	{
		case BOOSTING: tracker = cv::TrackerBoosting::create("BOOSTING"); break;
		case TLD: tracker = cv::TrackerTLD::create("TLD"); break;
		case MEDIAN_FLOW:
		default: 
		    tracker = cv::TrackerMedianFlow::create("MEDIANFLOW");
	}
	if (tracker == NULL) return;
	if (!tracker->init(init_frame,init_box)) return;
	ok=true;
}

BoxTracker::~BoxTracker()
{
}

bool BoxTracker::update(Mat image,Rect2d& box)
{
	//will this check for blank frame?
	if (ok) ok = tracker->update(image,box);
	return ok;
}
