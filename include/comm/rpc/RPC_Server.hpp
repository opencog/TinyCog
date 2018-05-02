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


#include "img_base.grpc.pb.h"


#define DETECT_FACE 0
#define DETECT_FACE_LM 1
#define DETECT_SMILE 2
#define DETECT_HAND 3
#define COUNT_FINGERS 4
#define DETECT_SALIENCY_POINT 5

using grpc::Status;
using grpc::Server;
using grpc::ServerContext;
using grpc::ServerBuilder;


class RPC_Server final : public ImageBase::ImageServices::Service 
{    
    public:
      RPC_Server(std::string server_address = "0.0.0.0:50051");
      RPC_Server() {}
      
      Status ImagePoint (ServerContext *, const ImageBase::Image*, ImageBase::Point*);
      Status ImageStreamPoint (ServerContext *, const ImageBase::Image*, ImageBase::Point*);

      Status ImagePoints (ServerContext *, const ImageBase::Image*, ImageBase::Points*);
      Status ImageStreamPoints (ServerContext *, const ImageBase::Image*, ImageBase::Points*);

      Status ImageRect (ServerContext *, const ImageBase::Image*, ImageBase::Rect*);
      Status ImageStreamRect (ServerContext *, const ImageBase::Image*, ImageBase::Rect*);

      Status ImageRects (ServerContext *, const ImageBase::Image*, ImageBase::Rects*);
      Status ImageStreamRects (ServerContext *, const ImageBase::Image*, ImageBase::Rects*);

      Status ImageCirc (ServerContext *, const ImageBase::Image*, ImageBase::Circ*);

    private:
      cv::Mat cvimg;
      cv::Point pt;
      cv::Rect rct;
      std::vector<cv::Point> pts;
      std::vector<cv::Rect> rcts;

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
