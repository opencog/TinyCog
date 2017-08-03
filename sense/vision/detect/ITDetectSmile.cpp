/*
  Project: OpenCogER
  File: ITDetectSmile.cpp
  Author: Mandeep Singh Bhatia
  License: AGPL
  Date: May 2017
*/
#include "sense/vision/ITDetectSmile.hpp"


ITDetectSmile::ITDetectSmile(string t_name):ImageTransform(t_name)
{
    //ctor
    if( !it_cascade.load( cascade_file ) ){ cout<<"--(!)Error loading smile cascade xml\n"; };
}

vector<Rect> ITDetectSmile::Transform(Mat in)
{
    vector<Rect> out;
    it_cascade.detectMultiScale( in, out,1.7, 8, 0|CV_HAAR_SCALE_IMAGE, Size(20, 20) );
    return out;
}
