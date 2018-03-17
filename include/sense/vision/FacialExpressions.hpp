/*
  Project: OpenCogER
  File: FacialExpressions.hpp
  Author: Dagim Sisay <dagiopia@gmail.com>
  License: APGL
  Date: March 2018
*/

#ifndef FACIALEXPRESSIONS_H_
#define FACIALEXPRESSIONS_H_

#include "sense/vision/FacialLandMark.hpp"

#include <stdlib.h>
#include <vector>


class FacialExpressions
{
    public:
        bool eyes_open();
	uint8_t mouth_open();
	uint8_t mouth_width();
	

    private:
        facial_lms *flms;
};

#endif // FACIALEXPRESSIONS_H_
