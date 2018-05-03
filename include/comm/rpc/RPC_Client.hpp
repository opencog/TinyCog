/*
  File: RPC_Client.hpp
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

#include <opencv2/opencv.hpp>

#include "img_base.grpc.pb.h"


#define IMG_ENCODING ".jpg"

using grpc::Status;
using grpc::Channel;
using grpc::ClientContext;


class RPC_Client
{    
    public:
      RPC_Client(std::string server_address="localhost:50051") : stub_(get_channel()) {}


    private:
      std::string server_address;
      std::shared_ptr<Channel> get_channel();

};


#endif //_RPC_CLIENT_HPP_
