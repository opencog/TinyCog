/*
  File: RPC_Client_Test.cpp
  Date: May, 2018
  Author: Dagim Sisay <dagiopia@gmail.com>
  License: AGPL
*/

#include "comm/rpc/RPC_Client.hpp"
#include "sense/vision/Vision.hpp"

#include <opencv2/opencv.hpp>
#include <vector>

int main(int argc, char **argv)
{
    RPC_Client rpcc;
    cv::VideoCapture cap(0);
    cv::Mat frame;
    std::vector<cv::Rect> faces;
    cap >> frame;
    rpcc.detect_faces(frame, faces);
    for(int i = 0; i < faces.size(); i++)
        rectangle(frame, faces[i], cv::Scalar(0, 255, 0), 2, 1);
    cv::imshow("FaceRPC", frame);
    cv::waitKey(0);
return 0;
}
