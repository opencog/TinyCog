/*
  Project: OpenCogER
  File: VisionTest.cpp
  Author: Mandeep Singh Bhatia
  License: AGPL
  Date: 2017
*/
#include <opencv2/core/utility.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <cstring>

#include "sense/vision/CamCapture.hpp"
#include "sense/vision/BoxTrackerThread.hpp"

using namespace std;
using namespace cv;
 
/* On start image is captured and user selects roi to track with mouse and press space bar
   then tracking starts 
   press "q" to exit
    * In median flow , when tracking is no longer possible the app exits
*/
   
int main( int argc, char** argv )
{    
    CamCapture cc("c1",320,240,0,30);
    if (!cc.isOk()){cout<<endl<<cc.getState()<<endl;return -1;}
    
    Mat frame;
    namedWindow( "Tracking API", 1 );
    
    Mat image;
    Rect2d boundingBox;
    bool paused = false;
    frame = cc.getCurrentFrame();
    frame.copyTo(image);
    boundingBox = selectROI("Tracking API", image);
    cout<<endl<<"Press q to exit"<<endl;
    cout<<endl<<boundingBox.x<<","<<boundingBox.y<<","<<boundingBox.width<<","<<boundingBox.height<<endl;
    imshow( "Tracking API", image );
    BoxTrackerThread bx(&cc,frame,boundingBox,BoxTracker::tracker_type::MEDIAN_FLOW);
    while(true)
    {
	if (bx.update(frame,boundingBox))
        {
            frame.copyTo(image);
            rectangle( image, boundingBox, Scalar( 255, 0, 0 ), 2, 1 );
            imshow( "Tracking API", image );
        }
        else break;
        char c = (char) waitKey( 20 );
        if( c == 'q' )
            break;
    }
    return 0;
}
