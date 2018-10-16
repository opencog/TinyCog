/*

  Project: OpenCogER
  File: FacialExpressions.hpp
  Author: Dagim Sisay <dagiopia@gmail.com>
  License: APGL
  Date: March 2018
*/

#ifndef FACIALEXPRESSIONS_H_
#define FACIALEXPRESSIONS_H_

#include "FacialLandMark.hpp"

#include <stdlib.h>
#include <vector>

#define EAR_THRESH 0.09



class FacialExpressions
{
public:
	FacialExpressions() {}
	double distance(dlib::point p1, dlib::point p2);
	double eyes_state(facial_lms flms);

	
    private:
 //       facial_lms flms;
		  double ear;
};


#endif // FACIALEXPRESSIONS_H_
