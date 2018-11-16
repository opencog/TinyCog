/*
  File: RPC_Client.cpp
  Date: May, 2018
  Author: Dagim Sisay <dagiopia@gmail.com>
  License: AGPL
*/


#include "comm/rpc/RPC_Client.hpp"


void RPC_Client::encode_img(cv::Mat in)
{
    vbuff.clear();
    cv::imencode(IMG_ENCODING, in, vbuff);
    ucbuff = &vbuff[0];
}


std::string RPC_Client::encode_img_b64(cv::Mat in)
{
    vbuff.clear();
    cv::imencode(IMG_ENCODING, in, vbuff);
    unsigned char *enc_msg = new unsigned char[vbuff.size()];
    for(int i=0; i < vbuff.size(); i++) enc_msg[i] = vbuff[i];
    std::string encoded = base64_encode(enc_msg, vbuff.size());
    return encoded;
}


bool RPC_Client::detect_emotion(cv::Mat &in, std::vector<std::string> &out_emos, 
                                             std::vector<cv::Rect> &out_boxes)
{
    Status status;
    ClientContext ctxt;
    RecognizeRequest img;
    RecognizeResponse resp;

    out_emos.clear();
    out_boxes.clear();
    img.set_image(encode_img_b64(in).c_str());
    img.set_image_type("jpg");
    status = stub_->classify(&ctxt, img, &resp);
    if(!status.ok()) {
        printf("Error: %s\n", status.error_message().c_str());
        return false;
    }
    for(size_t idx = 0; idx < resp.predictions_size(); idx++) {
        printf("Emo: %s\n", resp.predictions(idx).c_str());
		  out_emos.push_back(resp.predictions(idx));
	out_boxes.push_back(cv::Rect(resp.bounding_boxes(idx).x(),
	                          resp.bounding_boxes(idx).y(),
			          resp.bounding_boxes(idx).w(),
			          resp.bounding_boxes(idx).h()));
    }
    return true;
}


