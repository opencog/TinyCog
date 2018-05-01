/*
  File: RPC_Server.hpp
  Date: May, 2018
  Author: Dagim Sisay <dagiopia@gmail.com>
  License: AGPL
*/

#include <iostream>
#include <string>
#include <vector>

#include <opencv2/opencv.hpp>

#include <grpcpp/grpcpp.h>

#include "img_base.grpc.pb.h"

class RPC_Server final : public ImageBase::ImageServices::Service {
  


};
