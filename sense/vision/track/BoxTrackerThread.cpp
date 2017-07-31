/*
  Project: OpenCogER
  File: BoxTrackerThread.cpp
  Author: Mandeep Singh Bhatia
  License: AGPL
  Date: May 2017
*/
#include "sense/vision/BoxTrackerThread.hpp"
#include "unistd.h"

BoxTrackerThread::BoxTrackerThread(ImageSource *img_src,Mat init_frame,
    Rect2d init_box,BoxTracker::tracker_type tt =
	        BoxTracker::tracker_type::MEDIAN_FLOW):imgs(img_src)
{
	trk = new BoxTracker(init_frame,init_box,tt);
	if (!(ok=trk->isOk())) return;
	th = new thread(thread_loop,this);
}


BoxTrackerThread::~BoxTrackerThread()
{
	ok = false;
	//thread join
	th->join();
	delete th;
	delete trk;
}

bool BoxTrackerThread::update(Mat& image,Rect2d& box)
{
	mtx.lock();
	image = currentImage;
	box = currentBox;
	mtx.unlock();
	return ok;
}

bool BoxTrackerThread::thread_loop(BoxTrackerThread* btt)
{
	while(ok)
	{
		btt->mtx.lock();
		btt->currentImage = btt->imgs->getCurrentFrame();
		ok = btt->trk->update(btt->currentImage,btt->currentBox);
		btt->mtx.unlock();
		usleep(10*1000);//10 ms delay
	}
}
