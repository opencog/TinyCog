/*
  Project: OpenCogER
  File: RaspiCamCapture.hpp
  Author: Dagim Sisay
  License: AGPL
  Date: September 2017
*/
#ifndef RASPICAMCAPTURE_H
#define RASPICAMCAPTURE_H


#include "ImageSource.hpp"

#include <raspicam/raspicam_cv.h>

#include <iostream>
#include <string>
#include <cstdio>
#include <thread>
#include <mutex>

using namespace std;
using namespace cv;

class RaspiCamCapture: public ImageSource
{
    public:
        RaspiCamCapture(string src_name, int width=320, int height=240, double max_fps=30.0);
        
        virtual ~RaspiCamCapture();
        bool isOn();
        bool isOk();
        string getState();
        Mat getCurrentFrame();//Mat is like shared_ptr, make deep copy here and use mutex
    protected:
    private:
        int w,h;
        Mat current;
        raspicam::RaspiCam_Cv capture;
        static void thread_loop(RaspiCamCapture* cc);
        thread* run;
        mutex rd;
        int wait_time;
};

#endif // RASPICAMCAPTURE_H
