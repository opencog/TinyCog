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


bool RPC_Client::detect_faces(cv::Mat &in, std::vector<cv::Rect> &out)
{
    ImageBase::Image img;
    ImageBase::Faces faces;
    encode_img(in);
    img.set_data(ucbuff, vbuff.size());
    img.set_operation(0); //unused for now
    status = stub_->DetectFaces(&ctxt, img, &faces);
    if(!status.ok())
        return false;
    for(size_t idx = 0; idx < faces.faces_size(); idx++) {
        rct.x = faces.faces(idx).x();
        rct.y = faces.faces(idx).y();
        rct.width = faces.faces(idx).w();
        rct.height = faces.faces(idx).h();
	out.push_back(rct);
    }
    return true;
}


//the output param is only a vvp because I don't know how to insert into full_object_detection obj
bool RPC_Client::detect_face_lms(cv::Mat &in, std::vector<std::vector<cv::Point> > &out)
{
    ImageBase::Image img;
    ImageBase::Landmarks lms;
    encode_img(in);
    img.set_data(ucbuff, vbuff.size());
    img.set_operation(0); //unused for now
    status = stub_->FaceLandmarks(&ctxt, img, &lms);
    if(!status.ok())
        return false;

    cv::Point pt;
    for(size_t idx = 0; idx < lms.landmarks_size(); idx++) { //on faces
        facial_lms shape; 
	std::vector<cv::Point> flm;
	for (size_t jdx = 0; jdx < lms.landmarks(idx).points_size(); jdx++) {
	    pt.x = lms.landmarks(idx).points(idx).x();
	    pt.y = lms.landmarks(idx).points(idx).y();
	    flm.push_back(pt);
	}
	out.push_back(flm);
    }
    
    return true;
}


bool RPC_Client::salient_point(cv::Mat &in, cv::Point &out)
{
    ImageBase::Image img;
    ImageBase::Point pt;
    encode_img(in);
    img.set_data(ucbuff, vbuff.size());
    img.set_operation(0); //unused for now
    status = stub_->SalientPoint(&ctxt, img, &pt);
    if(!status.ok())
        return false;
    out.x = pt.x();
    out.y = pt.y();
    return true;
}
