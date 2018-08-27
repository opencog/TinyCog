/*
  Project: OpenCogER
  File: CamCapture.hpp
  Author: Mandeep Singh Bhatia
  License: AGPL
  Date: May 2017
*/
#ifndef CAMCAPTURE_H
#define CAMCAPTURE_H


#include "sense/vision/ImageSource.hpp"

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>
#include <string>
#include <cstdio>
#include <thread>
#include <mutex>

using namespace std;
using namespace cv;

class CamCapture: public ImageSource
{
    public:
        static CamCapture *init(string src_name,int width=320,int height=240,
                                int cam = 0,double max_fps=30.0);
        virtual ~CamCapture();
        bool isOn();
        bool isOk();
        string getState();
        Mat getCurrentFrame();//Mat is like shared_ptr, make deep copy here and use mutex
    
    private:
        CamCapture(string src_name,int width=320,int height=240,
                   int cam = 0,double max_fps=30.0); //max fps is not guaranteed
        static CamCapture *_cam_cap;
        int w,h;
        int ccam;
        Mat current;
        Mat *c_img;
        VideoCapture capture;
        static void thread_loop(CamCapture* cc);
        thread* run;
        mutex rd;
        int wait_time;
        
};

#endif // CAMCAPTURE_H
