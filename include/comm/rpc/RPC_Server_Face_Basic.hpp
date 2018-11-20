/*
  File: RPC_Server_Face_Basic.hpp
  Date: May, 2018
  Author: Dagim Sisay <dagiopia@gmail.com>
  License: AGPL
*/

#ifndef _RPC_SERVER_HPP_
#define _RPC_SERVER_HPP_

#include <iostream>
#include <string>
#include <vector>

#include <grpcpp/grpcpp.h>

#include "VisionCommon.hpp"

#include "img_base.grpc.pb.h"


using grpc::Status;
using grpc::Server;
using grpc::ServerContext;
using grpc::ServerBuilder;


class RPC_Server_Face_Basic final : public ImageServices::Service 
{    
    public:
      RPC_Server_Face_Basic(std::string server_address = "0.0.0.0:50051") : 
        c2g("c2g") , eh("eh") , fcd("fcd"), smd("smd"), 
        flm(), dh("dh"), fc(true), sal_d(SAL_STATIC, SAL_FINE_GRAINED)
      {
          ServerBuilder srvrb;
          srvrb.AddListeningPort(server_address, grpc::InsecureServerCredentials());
          srvrb.RegisterService(this);
          std::unique_ptr<Server> srvr(srvrb.BuildAndStart());
          std::cout<<"Server Listening on "<<server_address<<std::endl;
          srvr->Wait();
      }
      
      Status SalientPoint (ServerContext *ctxt, const ImageBase::Image *img, ImageBase::Point *p)
      override 
      {
          std::vector<unsigned char> dim(img->data().begin(), img->data().end());
	  cv_img = cv::imdecode(dim, cv::IMREAD_COLOR);
	  pt = sal_d.sal_point(cv_img, cv_img);
	  p->set_x(pt.x);
	  p->set_y(pt.y);
	  return Status::OK;
      }
//      Status ImageStreamPoint (ServerContext *, const ImageBase::Image*, ImageBase::Point*)
//      override {}

      Status FaceLandmarks (ServerContext *ctxt, const ImageBase::Image *img, ImageBase::Landmarks *ls)
      override {
          std::vector<unsigned char> dim(img->data().begin(), img->data().end());
	  cv_img = cv::imdecode(dim, cv::IMREAD_COLOR);
	  rcts = fcd.Transform(eh.Transform(c2g.Transform(cv_img)));
	  flm.get_lm_points(cv_img, rcts, flms);
	  for(int f_idx = 0 ; f_idx < flms.size(); f_idx++) { // on faces
              ImageBase::Points *pts = ls->add_landmarks();
	      for(int p_idx = 0 ; p_idx < flms[f_idx].num_parts(); p_idx++){ // on lm points (parts)
                  ImageBase::Point *pt = pts->add_points();
                  pt->set_x(flms[f_idx].part(p_idx).x());
                  pt->set_y(flms[f_idx].part(p_idx).y());
              }
           }
           return Status::OK;
      }

//      Status ImageStreamPoints (ServerContext *, const ImageBase::Image*, ImageBase::Points*);

//      Status ImageRect (ServerContext *, const ImageBase::Image*, ImageBase::Rect*);
//      Status ImageStreamRect (ServerContext *, const ImageBase::Image*, ImageBase::Rect*);

      Status DetectFaces (ServerContext *ctxt, const ImageBase::Image *img, ImageBase::Faces *fs)
      override {
          std::vector<unsigned char> dim(img->data().begin(), img->data().end());
          cv_img = cv::imdecode(dim, cv::IMREAD_COLOR);                          
          rcts = fcd.Transform(eh.Transform(c2g.Transform(cv_img)));             
          for(size_t i = 0; i < rcts.size(); i++) {                                     
              ImageBase::Rect *r = fs->add_faces();                              
              r->set_x(rcts[i].x); r->set_y(rcts[i].y);                          
              r->set_w(rcts[i].width); r->set_h(rcts[i].height);
          }
          return Status::OK;                                                     
      }
//      Status ImageStreamRects (ServerContext *, const ImageBase::Image*, ImageBase::Rects*);

//      Status ImageCirc (ServerContext *, const ImageBase::Image*, ImageBase::Circ*);

    private:
      cv::Mat cv_img;
      cv::Point pt;
      cv::Rect rct;
      std::vector<cv::Point> pts;
      std::vector<cv::Rect> rcts;
      std::vector<facial_lms> flms;

      ITColor2Gray c2g;
      ITEqualizeHist eh;
      ITDetectFace fcd;
      ITDetectSmile smd;
      FacialLandMark flm;
      ITDetectHand dh;
      FingersCount fc;
      DSaliency sal_d;

};


#endif //_RPC_SERVER_HPP_
