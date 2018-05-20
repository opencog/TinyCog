/*
  File: dr-roboto.cpp
  Author: Dagim Sisay <dagiopia@gmail.com>
  License: AGPL
  Date: May, 2018
*/
#include <libguile.h>

#include <stdio.h>
#include <vector>


#include "sense/vision/CamCapture.hpp"
#include "sense/vision/ITColor2Gray.hpp"
#include "sense/vision/ITEqualizeHist.hpp"
#include "sense/vision/ITDetectFace.hpp"
#include "sense/vision/ITDetectSmile.hpp"
#include "sense/vision/FacialLandMark.hpp"
#include "sense/vision/ITDetectHand.hpp"
#include "sense/vision/FingersCount.hpp"
#include "sense/vision/DSaliency.hpp"
#include "sense/vision/BoxTrackerThread.hpp"

//#include "util/SCMWrapper.hpp"


#define _NEED_GUI_
#define _NEED_TIME_INFO_

#define SCALE 0.1


extern "C" {
  void init_dr_roboto();
};

float avg_time_pf, en_time, acc, avg_oh, avg_nh, avg_nf, avg_of;
uint64_t st_time;
int n_f, nh, oh, nf, of;

CamCapture *cap;
ITColor2Gray *c2g;
ITEqualizeHist *eh;
ITDetectFace *fcd;
ITDetectSmile *smd;
FacialLandMark flm;
ITDetectHand *dh;
FingersCount *fc;
DSaliency *sal_d;

void print_report()
{       
    avg_time_pf = acc / n_f;
    avg_nh /= nh;
    avg_oh /= oh;
    avg_of /= of;
    avg_nf /= nf;
    printf("\nAverage Time per Frame (no Face): %f\n", avg_nf);
    printf("Average Time per Frame (on Face): %f\n", avg_of);
    printf("Average Time per Frame (no Hand): %f\n", avg_nh);
    printf("Average Time per Frame (on Hand): %f\n", avg_oh);
    printf("Average Time per Frame: %f\n\n", avg_time_pf);
}



const char *face_detect()
{
    std::string ret;
    std::vector<cv::Rect> faces;
    cv::Mat frame = cap->getCurrentFrame();
    faces = fcd->Transform(eh->Transform(c2g->Transform(frame)));
    for(size_t i = 0; i < faces.size(); i++)
        ret = std::to_string(faces[i].x) + "," + 
	      std::to_string(faces[i].y) + "," +
	      std::to_string(faces[i].width) + "." +
	      std::to_string(faces[i].height) + ";";
    ret += ";";
    return ret.c_str();
}

SCM scm_face_detect() 
{ 
  return scm_from_locale_string(face_detect()); 
}

void init_dr_roboto()
{
    #ifdef _NEED_TIME_INFO_
    avg_nh = avg_oh = avg_of = avg_nf = avg_time_pf = 0;
    oh = nh = of = nf = n_f = 0;
    #endif //_NEED_TIME_INFO_

    cap = new CamCapture("cap", 320, 240, 0, 20);
    c2g = new ITColor2Gray("c2g");
    eh = new ITEqualizeHist("eh");
    fcd = new ITDetectFace("fcd");
    smd = new ITDetectSmile("smd");
    dh = new ITDetectHand("dh");
    fc = new FingersCount(true);
    sal_d = new DSaliency(SAL_STATIC, SAL_FINE_GRAINED);

    scm_c_define_gsubr("face_detect", 0, 0, 0, (scm_t_subr)scm_face_detect);
 
}
