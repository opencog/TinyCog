/*
  Project: OpenCogER
  File: RaspiCamCapture.cpp
  Author: Dagim Sisay
  License: AGPL
  Date: September 2017
*/
#include "sense/vision/RaspiCamCapture.hpp"
#include "unistd.h"


#define MAX_WAIT (1000*1000)
#define MIN_WAIT (10*1000)
//wait in micro seconds

RaspiCamCapture::RaspiCamCapture(string src_name,int width,int height,
	double max_fps): ImageSource(src_name),w(width),h(height)
{
    //ctor
    cout<<endl<<"w="<<w<<",h="<<h<<endl;
    wait_time = MIN_WAIT;
    running=false;
    ok=0;
    state="starting";
    double wt=(1.0/max_fps)*(1000*1000);//micro seconds
    if ((wt<=(double)MAX_WAIT) && (wt>=(double)MIN_WAIT)) 
        wait_time = (int) wt;
         
    capture.set(CV_CAP_PROP_FRAME_WIDTH, w);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT, h);
    capture.set(CV_CAP_PROP_BRIGHTNESS, 55);
    capture.set(CV_CAP_PROP_CONTRAST, 55);
    capture.set(CV_CAP_PROP_SATURATION, 60);

    if(capture.open())
        {
	    state="capture open";
            run = new thread(RaspiCamCapture::thread_loop,this);
        }
    else
        {
	    ok=-1;
            state="error: Could not open RaspiCam ";
        }

}

RaspiCamCapture::~RaspiCamCapture()
{
    //dtor
    running = false;
    run->join();
    delete run;
}

bool RaspiCamCapture::isOn()
{
	return running;
}

bool RaspiCamCapture::isOk()
{
	return (ok==0);
}

string RaspiCamCapture::getState()
{
	string r;
	rd.lock();
	r=state;
	rd.unlock();
	return r;
}


Mat RaspiCamCapture::getCurrentFrame()
{
    Mat res;
    rd.lock();
    if (!current.empty())
    res = current.clone();
    rd.unlock();
    return res;
}

void RaspiCamCapture::thread_loop(RaspiCamCapture* cc)
{
    cc->state = "running";
    cc->running=true;
    while(cc->running)
    {
        cc->rd.lock();
        cc->capture.grab();
	cc->capture.retrieve(cc->current);
        //add error check code here like blank frame
        if (cc->current.empty()) 
        {
            if (cc->ok==0) {cc->ok=-2; cc->state = "error: blank frame";}
        } 
        else 
        { 
            if (cc->ok!=0) {cc->ok=0; cc->state = "ok";}
        }
        cc->rd.unlock();
        //don't use waitkey here
        usleep(cc->wait_time);//10ms min sleep between capture, adjust fps
    }
    cc->state = "stopped";
}
