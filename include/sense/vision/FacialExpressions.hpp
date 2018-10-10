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
#define phi 1.6182 // The golden ratio

class FacialExpressions
{
  public:
	bool eyes_open();
        uint8_t natural_face();
	
    private:
        facial_lms *flms;
};


#endif // FACIALEXPRESSIONS_H_
