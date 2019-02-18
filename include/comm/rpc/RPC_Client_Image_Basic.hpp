/*
  File: RPC_Client_Image_Basic.hpp
  Date: May, 2018
  Author: Dagim Sisay <dagiopia@gmail.com>
  License: AGPL
*/

#ifndef _RPC_CLIENT_HPP_
#define _RPC_CLIENT_HPP_

#include <iostream>
#include <string>
#include <vector>

#include <grpcpp/grpcpp.h>

#include  "VisionCommon.hpp"

#include "img_base.grpc.pb.h"

#define SERVER_ADDRESS "localhost:50051"
#define IMG_ENCODING ".jpg"

using grpc::Status;
using grpc::Channel;
using grpc::CreateChannel;
using grpc::ClientContext;


class RPC_Client_Image_Basic
{    
    public:
      RPC_Client_Image_Basic() : stub_(ImageBase::ImageServices::NewStub(grpc::CreateChannel(
      				SERVER_ADDRESS, grpc::InsecureChannelCredentials()))) {}
      ~RPC_Client_Image_Basic() {}

      bool detect_faces(cv::Mat &in, std::vector<cv::Rect> &out);
      bool detect_face_lms(cv::Mat &in, std::vector<std::vector<cv::Point> > &out);
      bool salient_point(cv::Mat &in, cv::Point &out);

    private:
      std::unique_ptr<ImageBase::ImageServices::Stub> stub_;

      Status status;
      std::vector<unsigned char> vbuff;
      unsigned char *ucbuff;
      cv::Rect rct;

      void encode_img(cv::Mat);

};


#endif //_RPC_CLIENT_HPP_
