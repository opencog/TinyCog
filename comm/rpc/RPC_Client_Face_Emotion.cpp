/*
  File: RPC_Client_Face_Emotion.cpp
  Date: November, 2018
  Author: Dagim Sisay <dagiopia@gmail.com>
  License: AGPL
*/


#include "comm/rpc/RPC_Client_Face_Emotion.hpp"


std::string RPC_Client_Face_Emotion::encode_img_b64(cv::Mat in)
{
	vbuff.clear();
	cv::imencode(IMG_ENCODING, in, vbuff);
	//unsigned char *enc_data = new unsigned char[vbuff.size()];
	//std::copy(vbuff.begin(), vbuff.end(), enc_data);
	return base64_encode(vbuff.data(), vbuff.size()).c_str();
}


bool RPC_Client_Face_Emotion::detect_emotion(cv::Mat &in, 
                                             std::vector<std::string> &out_emos,
                                             std::vector<cv::Rect> &out_boxes)
{
	ClientContext ctxt;
	RecognizeRequest img;
	RecognizeResponse resp;
	 

	out_emos.clear();
	out_boxes.clear();
	img.set_image(encode_img_b64(in));
	img.set_image_type(IMG_ENCODING);
	status = stub_->classify(&ctxt, img, &resp);
	 
	if(!status.ok()) { 
		fprintf(stderr, "Error: %s\n", status.error_message().c_str()); 
		return false; 
	}

	for(size_t idx = 0; idx < resp.predictions_size(); idx++) {
		printf("Emo: %s\n", resp.predictions(idx).c_str()); //XXX Remove
		out_emos.push_back(resp.predictions(idx));
		out_boxes.push_back(cv::Rect(resp.bounding_boxes(idx).x(),
		                             resp.bounding_boxes(idx).y(),
		                             resp.bounding_boxes(idx).w(),
		                             resp.bounding_boxes(idx).h()));
	}
	return true;
}


