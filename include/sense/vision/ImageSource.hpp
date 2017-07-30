/*
  Project: OpenCogER
  File: ImageSource.hpp
  Author: Mandeep Singh Bhatia
  License: AGPL
  Date: May 2017
*/

//This is the base class for image sources
//Images are expected in opencv2 MAT format

#ifndef IMAGESOURCE_H
#define IMAGESOURCE_H

#include "opencv2/core/core.hpp"
#include <string>

using namespace std;
using namespace cv;

//to start a source create object
//to stop source destroy object
class ImageSource
{
	public:
	ImageSource(string src_name):name(src_name){};
	virtual ~ImageSource();
	
	virtual bool isOn()=0;
	virtual bool isOk()=0;
	virtual string getState()=0;
	virtual string getName(){return name;}
	
	virtual Mat getCurrentFrame()=0;
	
	private:
	string name;
	string state;
	bool running;//isOn
	int ok; //maybe 0= okay, -1=error
};

#endif //IMAGESOURCE_H
