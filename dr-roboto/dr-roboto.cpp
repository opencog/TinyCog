/*
  File: dr-roboto.cpp
  Author: Dagim Sisay <dagiopia@gmail.com>
  License: AGPL
  Date: May, 2018
*/
#include <libguile.h>

#include <stdio.h>
#include <vector>

#include "sense/vision/RaspiVision.hpp"
#include "act/audio/FestivalTTS.hpp"

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
std::string ret;

RaspiCamCapture *rcap;
ITColor2Gray *c2g;
ITEqualizeHist *eh;
ITDetectFace *fcd;
ITDetectSmile *smd;
FacialLandMark flm;
ITDetectHand *dh;
FingersCount *fc;
DSaliency *sal_d;
FestivalTTS tts;

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


// return FACE_LOC[X,Y,W,H];;
const char *face_detect()
{
    ret = "";
    std::vector<cv::Rect> faces;
    cv::Mat frame = rcap->getCurrentFrame();
    faces = fcd->Transform(eh->Transform(c2g->Transform(frame)));
    for(size_t i = 0; i < faces.size(); i++)
        ret = std::to_string(faces[i].x) + "," + 
	      std::to_string(faces[i].y) + "," +
	      std::to_string(faces[i].width) + "." +
	      std::to_string(faces[i].height) + ";";
    ret += ";";
    cv::imshow("image", frame);
    return ret.c_str();
}

SCM scm_face_detect() 
{ 
  return scm_from_locale_string(face_detect()); 
}

// return FACE_LOC[X,Y,W,H];SMILE(TRUE/FALSE);;
const char *face_smile_detect()
{
    ret = "";
    std::vector<cv::Rect> faces, smile;
    cv::Mat frame = rcap->getCurrentFrame();
    faces = fcd->Transform(eh->Transform(c2g->Transform(frame)));
    for(size_t i = 0; i < faces.size(); i++) {
        ret = std::to_string(faces[i].x) + "," + 
	      std::to_string(faces[i].y) + "," +
	      std::to_string(faces[i].width) + "." +
	      std::to_string(faces[i].height) + ";";
	smile = smd->Transform(c2g->Transform(frame(faces[i])));
	ret += (smile.empty())? "false;" : "true;";
    }
    ret += ";";
    cv::imshow("image", frame);
    std::cout<<ret<<std::endl;
    cv::imwrite("face_smile.jpg", frame);
    return ret.c_str();
}

SCM scm_face_smile_detect() 
{ 
  return scm_from_locale_string(face_smile_detect()); 
}

// return SALIENT_POINT[X,Y];
const char *salient_point()
{
    cv::Mat frame = rcap->getCurrentFrame();
    cv::Point2d cent = sal_d->sal_point(frame, frame);
    ret = std::to_string(cent.x) + "," + std::to_string(cent.y);
    return ret.c_str();
}

SCM scm_salient_point() 
{ 
  return scm_from_locale_string(salient_point()); 
}


void say(char *data)
{
    tts.setSpeaker(FestivalTTS::speaker::DON);
    tts.speak(data);
}
SCM scm_say(SCM txt)
{
    say(scm_to_locale_string(txt));
}


void init_dr_roboto()
{
    #ifdef _NEED_TIME_INFO_
    avg_nh = avg_oh = avg_of = avg_nf = avg_time_pf = 0;
    oh = nh = of = nf = n_f = 0;
    #endif //_NEED_TIME_INFO_

    rcap = new RaspiCamCapture("cap", 320, 240, 20);
    c2g = new ITColor2Gray("c2g");
    eh = new ITEqualizeHist("eh");
    fcd = new ITDetectFace("fcd");
    smd = new ITDetectSmile("smd");
    dh = new ITDetectHand("dh");
    fc = new FingersCount(true);
    sal_d = new DSaliency(SAL_STATIC, SAL_FINE_GRAINED);

    scm_c_define_gsubr("face_detect", 0, 0, 0, (scm_t_subr)scm_face_detect);
    scm_c_define_gsubr("face_smile_detect", 0, 0, 0, (scm_t_subr)scm_face_smile_detect);
    scm_c_define_gsubr("salient_point", 0, 0, 0, (scm_t_subr)scm_salient_point);
    scm_c_define_gsubr("say", 1, 0, 0, (scm_t_subr)scm_say);
 
}
