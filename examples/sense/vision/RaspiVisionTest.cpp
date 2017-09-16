/*
  Project: OpenCogER
  File: RaspiVisionTest.cpp
  Author: Dagim Sisay
  License: AGPL
  Date: September 2017
*/
#include <iostream>
#include <cstring>

#include "sense/vision/RaspiCamCapture.hpp"

using namespace std;
using namespace cv;
 
   
int main( int argc, char** argv )
{    
    RaspiCamCapture cc("RaspiCam", 320, 240, 30);
    if (!cc.isOk()){cout<<endl<<cc.getState()<<endl;return -1;}
    
    Mat frame;
    Mat image;
    while(true)
    {
        frame = cc.getCurrentFrame();
	frame.copyTo(image);
        imshow( "Tracking API", image );
        
	char c = (char) waitKey( 1 );
        if( c == 'q' )
            break;
    }
    return 0;
}
