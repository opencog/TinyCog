/*
  Project: OpenCogER
  File: CamCapture.cpp
  Author: Mandeep Singh Bhatia
  License: AGPL
  Date: May 2017
*/
#include "sense/vision/CamCapture.hpp"
#include "unistd.h"


#define MAX_WAIT (1000*1000)
#define MIN_WAIT (10*1000)
//wait in micro seconds

CamCapture::CamCapture(string src_name,int width,int height,int cam,
    double max_fps): ImageSource(src_name),w(width),h(height),ccam(cam)
{
    //ctor
    cout<<endl<<"w="<<w<<",h="<<h<<",cam="<<ccam<<endl;
    wait_time = MIN_WAIT;
    running=false;
    ok=0;
    state="starting";
    double wt=(1.0/max_fps)*(1000*1000);//micro seconds
    if ((wt<=(double)MAX_WAIT) && (wt>=(double)MIN_WAIT)) 
        wait_time = (int) wt;
         
    capture.open(ccam);
    if( capture.isOpened() )
        {
			state="capture open";
            capture.set(CV_CAP_PROP_FRAME_WIDTH,w);
            capture.set(CV_CAP_PROP_FRAME_HEIGHT,h);
            run = new thread(CamCapture::thread_loop,this);
        }
    else
        {
			ok=-1;
            state="error: Could not open device "+to_string(cam);
        }

}




CamCapture::~CamCapture()
{
    //dtor
    running = false;
    run->join();
    delete run;
}

bool CamCapture::isOn()
{
	return running;
}

bool CamCapture::isOk()
{
	return (ok==0);
}

string CamCapture::getState()
{
	string r;
	rd.lock();
	r=state;
	rd.unlock();
	return r;
}


Mat CamCapture::getCurrentFrame()
{
    Mat res;
    rd.lock();
    if (!current.empty())
    res = current.clone();
    //current.copyTo(res);
    rd.unlock();
    return res;
}

void CamCapture::thread_loop(CamCapture* cc)
{
    cc->state = "running";
    cc->running=true;
    while(cc->running)
    {
        cc->rd.lock();
        (cc->capture)>>(cc->current);
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


