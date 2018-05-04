/*
  File: RPC_Client.cpp
  Date: May, 2018
  Author: Dagim Sisay <dagiopia@gmail.com>
  License: AGPL
*/


#include "RPC_Client.hpp"


void RPC_Client::encode_img(cv::Mat in)
{
    cv::imencode(IMG_ENCODING, in, vbuff);
    ucbuff = &vbuff[0];
}
