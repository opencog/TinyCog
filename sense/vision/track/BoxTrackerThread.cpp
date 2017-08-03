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
    Rect2d init_box,BoxTracker::tracker_type tt):imgs(img_src)
{
	trk = new BoxTracker(init_frame,init_box,tt);
	if (!(ok=trk->isOk())) return;
    currentImage=init_frame;
    currentBox=init_box;
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

void BoxTrackerThread::thread_loop(BoxTrackerThread* btt)
{
	while(btt->ok)
	{
		btt->mtx.lock();
		btt->currentImage = btt->imgs->getCurrentFrame();
		//check if blank frame is handled
		btt->ok = btt->trk->update(btt->currentImage,btt->currentBox);
		btt->mtx.unlock();
		usleep(20*1000);//20 ms delay
	}
}
