/*
  Project: OpenCogER
  File: FacialExpressions.cpp
  Author: Dagim Sisay <dagiopia@gmail.com>
  License: APGL
  Date: March 2018
*/

#include "sense/vision/FacialExpressions.hpp"

bool FacialExpressions::eyes_open()
{
    double ear1, ear2, ear;
    ear1 = (std::abs(flms->part(LEU1).y() - flms->part(LEB1).y()) + 
    		std::abs(flms->part(LEU2).y() - flms->part(LEB2).y())) / 
		(2 * std::abs(flms->part(LERS).x() - flms->part(LELS).x()));
    ear2 = (std::abs(flms->part(REU1).y() - flms->part(REB1).y()) + 
    		std::abs(flms->part(REU2).y() - flms->part(REB2).y())) / 
		(2 * std::abs(flms->part(RERS).x() - flms->part(RELS).x()));
    ear = (ear1+ear2) / 2;
    return (ear < 0.09) ? false : true; 
}

uint8_t FacialExpressions::mouth_open()
{
    double mvd1, mvd2, mar; //mouth vertical distance and mouth aspect ratio
   
}
