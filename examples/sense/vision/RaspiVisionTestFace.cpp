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
#include <signal.h>

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
   

float avg_time_pf, en_time, acc, avg_nf, avg_of;
uint64_t st_time;
int n_f, of, nf;


void sigint_handler(int sig)
{
    avg_time_pf = acc / n_f;
    avg_of /= of;
    avg_nf /= nf;
    printf("\nAverage Time per Frame (no Face): %f\n", avg_nf);
    printf("Average Time per Frame (on Face): %f\n", avg_of);
    printf("Average Time per Frame: %f\n\n", avg_time_pf);
    exit(0);
}


int main( int argc, char** argv )
{    
    avg_of = avg_nf = avg_time_pf = 0;
    of = nf = n_f = 0;
    signal(SIGINT, sigint_handler);
    RaspiCamCapture cc("c1",320,240,30);
    if (!cc.isOk()){cout<<endl<<cc.getState()<<endl;return -1;}
    
    Mat frame;
   // namedWindow( "Tracking Face", 1 );
    
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
        st_time = getTickCount();
	if (!face_found)
        {
            frame = cc.getCurrentFrame();
//            imshow( "Tracking Face", frame );
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
//                imshow( "Tracking Face", image );
            } else {
                face_found = false;
                delete bx;
            }
        }
//        char c = (char) waitKey( 30 );
//        if( c == 'q' )
//            break;
	en_time = (float)((getTickCount() - st_time) / getTickFrequency());
	printf("Face State: %d, Frame #%d, Time: %f\n", (int)face_found, n_f, en_time);
	if (face_found){
		avg_of += en_time;
		of++;
	}
	else{
		avg_nf += en_time;
		nf++;
	}

	acc += en_time;
	n_f++;
    }
    return 0;
}
