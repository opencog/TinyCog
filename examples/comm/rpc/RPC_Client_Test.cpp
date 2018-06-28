/*
  File: RPC_Client_Test_All.cpp
  Date: May, 2018
  Author: Dagim Sisay <dagiopia@gmail.com>
  License: AGPL
*/

#include "comm/rpc/RPC_Client.hpp"
#include "sense/vision/RaspiVision.hpp"

#include <opencv2/opencv.hpp>
#include <vector>
#include <unistd.h>

int main(int argc, char **argv)
{
    RaspiCamCapture rcap("rcap", 320, 240, 20);
    RPC_Client rpcc;
    cv::Mat frame; 
    std::vector<cv::Rect> faces;
    std::vector<std::vector<cv::Point> > f_lms;
    cv::Point spt;
    frame = rcap.getCurrentFrame();
    std::cout<<"Read Frame\n";
    bool fd = rpcc.detect_faces(frame, faces);
    bool fl = rpcc.detect_face_lms(frame, f_lms);
    bool sp = rpcc.salient_point(frame, spt);
    printf("Done!\n");
    if (fd) {
        for(int i = 0; i < faces.size(); i++) {
            cv::rectangle(frame, faces[i], cv::Scalar(0, 255, 0), 2, 1);
	    for (int j = 0; j < f_lms[i].size(); j++)
	        cv::circle(frame, f_lms[i][j], 1.5, cv::Scalar(0, 255, 255), 1.5);
	}
	cv::circle(frame, spt, 2, cv::Scalar(0, 0, 255), 3);
        cv::imshow("FaceRPC", frame);
        cv::waitKey(0);
    }
return 0;
}
