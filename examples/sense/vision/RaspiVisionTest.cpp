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
    RaspiCamCapture *rc = RaspiCamCapture::init("RaspiCam", 320, 240, 30);
    if (!rc->isOk()){cout<<endl<<rc->getState()<<endl;return -1;}
    
    Mat frame;
    while(true)
    {
        frame = rc->getCurrentFrame();
        imshow( "Tracking API", frame );
        
	char c = (char) waitKey( 1 );
        if( c == 'q' )
            break;
    }
    return 0;
}
