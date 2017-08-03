/*
  Project: OpenCogER
  File: BoxTrackerThread.hpp
  Author: Mandeep Singh Bhatia
  License: AGPL
  Date: May 2017
*/

//This is tracker wrapper
//Images are expected in opencv2 Mat format

#ifndef BOXTRACKERTHREAD_H
#define BOXTRACKERTHREAD_H

#include "sense/vision/ImageSource.hpp"
#include "sense/vision/BoxTracker.hpp"

#include <iostream>
#include <string>
#include <thread>
#include <mutex>
//#include <vector>

using namespace std;
using namespace cv;

class BoxTrackerThread
{
	public:
	BoxTrackerThread(ImageSource *img_src,Mat init_frame,
	    Rect2d init_box,
	    BoxTracker::tracker_type tt =
	        BoxTracker::tracker_type::MEDIAN_FLOW);
	~BoxTrackerThread();
	bool update(Mat& image,Rect2d& box);
	bool isOk(){return ok;}
	
	private:
	BoxTracker *trk;
	ImageSource *imgs;
	thread* th;
	mutex mtx;
	Mat currentImage;
	Rect2d currentBox;
	bool ok;
	bool running;
	static void thread_loop(BoxTrackerThread* btt);
};

#endif
