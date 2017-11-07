/*
  Project: OpenCogER
  File: FaceLandmark.cpp
  Author: Dagim Sisay
  License: AGPL
  Date: 2017
*/
#include <opencv2/core/utility.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_io.h>
#include <dlib/opencv.h>

#include <iostream>
#include <cstring>

#include "sense/vision/CamCapture.hpp"
#include "sense/vision/ITColor2Gray.hpp"
#include "sense/vision/ITEqualizeHist.hpp"
#include "sense/vision/ITDetectFace.hpp"

#define MODEL_FILE "shape_predictor_68_face_landmarks.dat"


using namespace std;
using namespace cv;
using namespace dlib;
 
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
    ITColor2Gray c2g("c2g1");
    ITEqualizeHist eh("eh1");
    ITDetectFace fcd("fc1");
    std::vector<cv::Rect> faces; 
    //imshow( "Tracking API", image );
    
    frontal_face_detector detector = get_frontal_face_detector();
    shape_predictor sp; 
    deserialize(MODEL_FILE) >> sp;

    image_window win, win_faces;
    
    while(true)
    {
    	frame = cc.getCurrentFrame();
	frame.copyTo(image);

	faces = fcd.Transform(eh.Transform(c2g.Transform(frame)));
	
	std::vector<dlib::rectangle> dets;
	for (int idx = 0; idx < faces.size(); idx++){
		cv::Rect tr = faces[idx];
		dets.push_back(dlib::rectangle((long)tr.tl().x, (long)tr.tl().y, 
					(long)tr.br().x - 1, (long)tr.br().y - 1));
	}

	array2d<bgr_pixel> img;
	assign_image(img, dlib::cv_image<bgr_pixel>(image));
	//pyramid_up(img);
	
	//std::vector<dlib::rectangle> dets = detector(img);

	cout<<"Number of Faces Detected: "<<dets.size()<<endl;
	std::vector<full_object_detection> shapes;
	for (unsigned long j = 0; j < dets.size(); ++j)
	{
		full_object_detection shape = sp(img, dets[j]);
		cout << "number of parts: "<< shape.num_parts() << endl;
		cout << "pixel position of first part:  " << shape.part(0) << endl;
		cout << "pixel position of second part: " << shape.part(1) << endl;
		// You get the idea, you can get all the face part locations if
		// you want them.  Here we just store them in shapes so we can
		// put them on the screen.
		shapes.push_back(shape);
	}

	Mat final_image;
	final_image = toMat(img);
	imshow("Tracking API", final_image);
	win.clear_overlay();
	win.set_image(img);
	win.add_overlay(render_face_detections(shapes));

	//dlib::array<array2d<rgb_pixel> > face_chips; 
	//extract_image_chips(img, get_face_chip_details(shapes), face_chips);
	//win_faces.set_image(tile_images(face_chips));
	
	char c = (char) waitKey( 20 );
        if( c == 'q' )
            break;
    }
    return 0;
}
