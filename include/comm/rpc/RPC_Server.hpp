/*
  File: RPC_Server.hpp
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

#include "sense/vision/VisionCommon.hpp"

#include "img_base.grpc.pb.h"


using grpc::Status;
using grpc::Server;
using grpc::ServerContext;
using grpc::ServerBuilder;


class RPC_Server final : public ImageBase::ImageServices::Service 
{    
    public:
      RPC_Server(std::string server_address = "0.0.0.0:50051");
      RPC_Server() {}
      
      Status SalientPoint (ServerContext *, const ImageBase::Image*, ImageBase::Point*);
      Status ImageStreamPoint (ServerContext *, const ImageBase::Image*, ImageBase::Point*);

      Status FaceLandmarks (ServerContext *, const ImageBase::Image*, ImageBase::Landmarks*);
      Status ImageStreamPoints (ServerContext *, const ImageBase::Image*, ImageBase::Points*);

      Status ImageRect (ServerContext *, const ImageBase::Image*, ImageBase::Rect*);
      Status ImageStreamRect (ServerContext *, const ImageBase::Image*, ImageBase::Rect*);

      Status DetectFaces (ServerContext *, const ImageBase::Image*, ImageBase::Faces*);
      Status ImageStreamRects (ServerContext *, const ImageBase::Image*, ImageBase::Rects*);

      Status ImageCirc (ServerContext *, const ImageBase::Image*, ImageBase::Circ*);

    private:
      cv::Mat cvimg;
      cv::Point pt;
      cv::Rect rct;
      std::vector<cv::Point> pts;
      std::vector<cv::Rect> rcts;
      std::vector<facial_lms> flms;

      ITColor2Gray c2g("c2g1");
      ITEqualizeHist eh("eh1");
      ITDetectFace fcd("fcd1");
      ITDetectSmile smd("smd1");
      FacialLandMark flm;
      ITDetectHand dh("dh1");
      FingersCount fc(true);
      DSaliency sal_d(SAL_STATIC, SAL_FINE_GRAINED);

};


#endif //_RPC_SERVER_HPP_
