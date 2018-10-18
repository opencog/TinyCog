/*
  Project: TinyCog
  File: FileTestFace.cpp
  Author: Dagim Sisay
  License: AGPL
  Date: October, 2018
*/
#include <iostream>
#include <cstring>
#include <vector>

#include "sense/vision/FileCapture.hpp"
#include "sense/vision/VisionCommon.hpp"

using namespace std;
using namespace cv;
 
int main( int argc, char** argv )
{    
    if(argc != 2) { printf("Provide File Name\n"); exit(1); }

	 FileCapture *cc = FileCapture::init("c1", argv[1],50);
    if (!cc->isOk()){cout<<endl<<cc->getState()<<endl;return -1;}
    
    Mat frame;
    vector<Rect> faces, smiles;
    bool face_found = false;
	 size_t idx = 0, jdx = 0;
    frame = cc->getCurrentFrame();
    cout<<endl<<"Press q to exit"<<endl;
    ITColor2Gray c2g("c2g1");
    ITEqualizeHist eh("eh1");
    ITDetectFace fcd("fc1");
    ITDetectSmile sml("sm1");
    while(true)
    {
        frame = cc->getCurrentFrame();
        faces = fcd.Transform(eh.Transform(c2g.Transform(frame)));
		  for(idx = 0; idx < faces.size(); idx++) {
            rectangle(frame, faces[idx], Scalar( 255, 0, 0 ), 2, 1 );
            smiles = sml.Transform(c2g.Transform(Mat(frame, faces[idx])));
            for(jdx = 0; jdx < smiles.size(); jdx++)
				    rectangle(frame, Rect(faces[idx].x + smiles[jdx].x,
					                       faces[idx].y + smiles[jdx].y,
												  smiles[jdx].width,
												  smiles[jdx].height), Scalar( 0, 0, 255 ), 2, 1 );
        }
		  imshow("Tracking Face", frame);
        char c = (char) waitKey( 30 );
        if( c == 'q' )
            break;
    }
    return 0;
}
