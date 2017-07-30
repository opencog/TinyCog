/*
  Project: OpenCogER
  File: ITEqualizeHist.cpp
  Author: Mandeep Singh Bhatia
  License: AGPL
  Date: May 2017
*/
#include "sense/vision/ITEqualizeHist.hpp"


Mat ITEqualizeHist::Transform(Mat in)
{
	Mat out;
    if (in.channels()==1)
        equalizeHist(in,out);
        
    return out;	//out is empty if in channels is not 1
}
