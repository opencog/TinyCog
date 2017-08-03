/*
  Project: OpenCogER
  File: ImageTransform.hpp
  Author: Mandeep Singh Bhatia
  License: AGPL
  Date: May 2017
*/

//This is the base class for image transformers
//Images are expected in opencv2 MAT format

#ifndef IMAGETRANSFORM_H
#define IMAGETRANSFORM_H

#include <string>

using namespace std;

template <class inp, class outp>
class ImageTransform
{
	public:
	
	ImageTransform(string t_name):name(t_name){};
	virtual ~ImageTransform();
	
	string getName(){return name;}
		
	virtual outp Transform(inp in)=0;
	
	private:
	string name;
};

#endif //IMAGETRANSFORM_H
