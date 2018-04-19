/*
  Project: OpenCogER
  File: ITDetectHand.cpp
  Author: Dagim Sisay
  License: AGPL
  Date: March, 2018
*/
#include "sense/vision/ITDetectHand.hpp"


ITDetectHand::ITDetectHand(std::string t_name):ImageTransform(t_name)
{
    if( !it_cascade.load(cascade_file))
    { 
        fprintf(stderr, "Error Loading Hand Cascade File\n"); 
	exit(1); 
    }
}

std::vector<cv::Rect> ITDetectHand::Transform(cv::Mat in)
{
    std::vector<cv::Rect> out;
    /* XXX 
       will only detect one hand even if multiple in view
       because of CV_FIND_BIGGEST_OBJECT
       this is done to minimize errors but if the detector
       can be improved enough to not force detecting only 
       one, that would be great
    */
    it_cascade.detectMultiScale( in, out, 1.1, 4, 
    		cv::CASCADE_DO_ROUGH_SEARCH | cv::CASCADE_FIND_BIGGEST_OBJECT, 
		cv::Size(40, 40) );
    return out;
}
