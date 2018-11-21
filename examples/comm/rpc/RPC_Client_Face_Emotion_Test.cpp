/*
  File: RPC_Client_Face_Emotion_Test.cpp
  Date: November, 2018
  Author: Dagim Sisay <dagiopia@gmail.com>
  License: AGPL
*/

#include "comm/rpc/RPC_Client_Face_Emotion.hpp"
#include "sense/vision/RaspiVision.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <unistd.h>


int main(int argc, char **argv)
{
	RaspiCamCapture *rcap = RaspiCamCapture::init("rcap", 320, 240, 20);
	ITColor2Gray c2g("c2g");
	
	RPC_Client_Face_Emotion rpcc;

	cv::Mat frame;
	while(true) {
		frame = rcap->getCurrentFrame();
		std::vector<std::string> emo_preds;
		std::vector<cv::Rect> emo_boxes;
		bool ed = rpcc.detect_emotion(frame, emo_preds, emo_boxes);
		printf("Done! %d \n", (int)ed);
		if (ed) {
	    	for(uint32_t idx = 0; idx < emo_preds.size(); idx++) {
				cv::rectangle(frame, emo_boxes[idx], cv::Scalar(0, 255, 0), 2, 1);
				cv::putText(frame, emo_preds[idx], cv::Point(emo_boxes[idx].x, emo_boxes[idx].y),
				            FONT_HERSHEY_COMPLEX_SMALL, 2, cv::Scalar(0, 255, 0), 1, CV_AA);
			}
		}
		cv::imshow("FaceRPC", frame);
		if(27 == cv::waitKey(10)) break;
		usleep(2000); //break;
	}
	return 0;
}
