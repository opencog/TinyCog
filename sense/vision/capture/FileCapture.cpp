/*
  Project: OpenCogER
  File: FileCapture.cpp
  Author: Mandeep Singh Bhatia
  License: AGPL
  Date: May 2017
*/
#include "sense/vision/FileCapture.hpp"
#include "unistd.h"


#define MAX_WAIT (1000*1000)
#define MIN_WAIT (10*1000)
//wait in micro seconds


FileCapture *FileCapture::_file_cap = NULL;


FileCapture *FileCapture::init(string src_name, string fileName, double max_fps)
{
    if(_file_cap == NULL)
        _file_cap = new FileCapture(src_name, fileName, max_fps);
    return _file_cap;
}


FileCapture::FileCapture(string src_name,string fileName,
    double max_fps): ImageSource(src_name),fName(fileName)
{
    //ctor
    wait_time = MIN_WAIT;
    running=false;
    ok=0;
    state="starting";
    double wt=(1.0/max_fps)*(1000*1000);//micro seconds
    if ((wt<=(double)MAX_WAIT) && (wt>=(double)MIN_WAIT)) 
        wait_time = (int) wt;
         
    capture.open(fName);
    if( capture.isOpened() )
        {
			state="capture open";
            run = new thread(FileCapture::thread_loop,this);
        }
    else
        {
			ok=-1;
            state="error: Could not open "+fName;
        }

}

FileCapture::~FileCapture()
{
    //dtor
    running = false;
    run->join();
    delete run;
}

bool FileCapture::isOn()
{
	return running;
}

bool FileCapture::isOk()
{
	return (ok==0);
}

string FileCapture::getState()
{
	string r;
	rd.lock();
	r=state;
	rd.unlock();
	return r;
}


Mat FileCapture::getCurrentFrame()
{
    Mat res;
    rd.lock();
    if (!current.empty())
    res = current.clone();
    //current.copyTo(res);
    rd.unlock();
    return res;
}

void FileCapture::thread_loop(FileCapture* cc)
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
