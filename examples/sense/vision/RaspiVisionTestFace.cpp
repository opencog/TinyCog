/*
  Project: OpenCogER
  File: RaspiVisionTestFace.cpp
  Author: Dagim Sisay
  License: AGPL
  Date: September 2017
*/
#include <iostream>
#include <cstring>
#include <vector>

#include "sense/vision/RaspiCamCapture.hpp"
#include "sense/vision/ITColor2Gray.hpp"
#include "sense/vision/ITEqualizeHist.hpp"
#include "sense/vision/ITDetectFace.hpp"
#include "sense/vision/ITDetectSmile.hpp"
#include "sense/vision/BoxTrackerThread.hpp"

using namespace std;
using namespace cv;
 
/* On start image is captured face detector searches for face
   then tracking starts , while tracking smile is looked up
   press "q" to exit
    * In median flow , when tracking is no longer possible the app looks for face
*/
   
int main( int argc, char** argv )
{    
    RaspiCamCapture cc("c1",320,240,30);
    if (!cc.isOk()){cout<<endl<<cc.getState()<<endl;return -1;}
    
    Mat frame;
    namedWindow( "Tracking Face", 1 );
    
    Mat image;
    Rect2d box2d;
    Rect box;
    vector<Rect> boxes;
    bool face_found = false;
    frame = cc.getCurrentFrame();
    cout<<endl<<"Press q to exit"<<endl;
    BoxTrackerThread *bx;
    ITColor2Gray c2g("c2g1");
    ITEqualizeHist eh("eh1");
    ITDetectFace fcd("fc1");
    ITDetectSmile sml("sm1");
    while(true)
    {
        if (!face_found)
        {
            frame = cc.getCurrentFrame();
            imshow( "Tracking Face", frame );
            boxes = fcd.Transform(eh.Transform(c2g.Transform(frame)));
            if (boxes.size()>0)
            {
                box = boxes[0];
                box2d.x = box.x;
                box2d.y = box.y;
                box2d.width = box.width;
                box2d.height = box.height;
                bx = new BoxTrackerThread(&cc,frame,box2d);
                face_found = true;
            }
        } else { // face found
            if (bx->update(frame,box2d))
            {
                box.x = box2d.x; box.y = box2d.y; box.width = box2d.width; box.height = box2d.height;
                frame.copyTo(image);
                if ((box.x+box.width<=frame.cols) && (box.y+box.height<=frame.rows)
                    && (box.x>=0) && (box.y>=0))
                {
                    rectangle( image, box, Scalar( 255, 0, 0 ), 2, 1 );
                    Mat sub (frame,box);
                    boxes = sml.Transform(c2g.Transform(sub));//frame(box)
                    if (boxes.size()>0)
                        {
                            rectangle( image, 
                                Rect(box.x+boxes[0].x,box.y+boxes[0].y,boxes[0].width,boxes[0].height),
                                    Scalar( 0, 0, 255 ), 2, 1 );
                        }
                } else {
                face_found = false;
                delete bx;                    
                }
                imshow( "Tracking Face", image );
            } else {
                face_found = false;
                delete bx;
            }
        }
        char c = (char) waitKey( 30 );
        if( c == 'q' )
            break;
    }
    return 0;
}
