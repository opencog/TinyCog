/*
  Project: OpenCogER
  File: ITColor2Gray.cpp
  Author: Mandeep Singh Bhatia
  License: AGPL
  Date: May 2017
*/
#include "sense/vision/ITColor2Gray.hpp"


Mat ITColor2Gray::Transform(Mat in)
{
	Mat out;
    if (in.channels()>1)//later do detailed check and return issues
        cvtColor(in,out,COLOR_BGR2GRAY);
    else
        out=in;
        
    return out;	
}
