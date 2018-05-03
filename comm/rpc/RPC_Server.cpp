/*
  File: RPC_Server.cpp
  Date: May, 2018
  Author: Dagim Sisay <dagiopia@gmail.com>
  License: AGPL
*/


#include "RPC_Server.hpp"

RPC_Server::RPC_Server(std::string server_address)
{
    /*
    c2g = new ITColor2Gray("c2g");
    eh = new ITEqualizeHist("eh");
    fcd = new ITDetectFace("fcd");
    smd = new ITDetectSmile("smd");
    flm = new FacialLandMark();
    dh = new ITDetectHand("dh");
    fc = new FingersCount(true);
    */
    
    ServerBuilder srvrb;
    srvrb.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    srvrb.RegisterService(this);
    std::unique_ptr<Server> srvr(srvrb.BuildAndStart());
    std::cout<<"Server Listening on "<<server_address<<std::endl;
    srvr->Wait();
}


Status RPC_Server::SalientPoint (ServerContext *ctxt, const ImageBase::Image *img, ImageBase::Point *p)
override
{
    std::vector<unsigned char> dim(img->data().begin(), img->data().end());
    cv_img = cv::imdecode(dim, cv::IMREAD_COLOR);
    pt = sal_d.sal_point(frame, frame);
    p->set_x(pt.x);
    p->set_y(pt.y);
    return Status::OK;
}


Status RPC_Server::DetectFaces (ServerContext *ctxt, const ImageBase::Image *img, ImageBase::Faces *fs)
override
{
    std::vector<unsigned char> dim(img->data().begin(), img->data().end());
    cv_img = cv::imdecode(dim, cv::IMREAD_COLOR);
    rcts = fcd.Transform(eh.Transform(c2g.Transform(cv_img)));
    for(i = 0; i < rcts.size(); i++) {
        ImageBase::Rect *r = fs->add_faces();
	r->set_x(rcts[i].x); r->set_y(rcts[i].y);
	r->set_w(rcts[i].width); r->set_h(rcts[i].height);
    }
    return Status::OK;
}

Status RPC_Server::FaceLandMarks (ServerContext *ctxt, const ImageBase::Image *img, ImageBase::Landmarks *ls)
override
{
    std::vector<unsigned char> dim(img->data().begin(), img->data().end());
    cv_img = cv::imdecode(dim, cv::IMREAD_COLOR);
    rcts = fcd.Transform(eh.Transform(c2g.Transform(cv_img)));
    flm.get_lm_points(cv_img, rcts, &flms);
    for(int f_idx = 0 ; f_idx < flms.size(); f_idx++) { // on faces
        ImageBase::Points *pts = ls->add_landmarks();
        for(int p_idx = 0 ; p_idx < NO_FLM_PTS; p_idx++){ // on lm points (parts)
	    ImageBase::Point *pt = pts->add_points();
	    pt->set_x(flms[f_idx].part(p_idx).x());
	    pt->set_y(flms[f_idx].part(p_idx).y());
	}
    }
    return Status::OK;
}
