/*
  Project: OpenCogER
  File: FacialExpressions.cpp
  Author: Dagim Sisay <dagiopia@gmail.com>
  License: APGL
  Date: March 2018
*/

#include "sense/vision/FacialExpressions.hpp"
/*
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
    return (ear < EAR_THRESH) ? false : true; 
}
*/
bool FacialExpressions::eyes_open()
{

   
    double Leye,Reye;//left eye, right eye
    Leye = (std::abs(flms->part(LETA).y() - flms->part(LEU1).y())) / (std::abs(flms->part(LEU1).y() - flms->part(LEB1).y()));
    Reye = (std::abs(flms->part(RETA).y() - flms->part(REU1).y())) / (std::abs(flms->part(REU1).y() - flms->part(REB1).y()));     
    
    bool eyes_open;
	 if (1.6 <= Leye && Leye <= 1.62 || 1.6 <= Reye && Reye >= 1.62) 
        	return  true;
}

 uint8_t FacialExpressions::natural_face() 
 {

  double mouth_width;
  bool natural_face;
   mouth_width = (std::abs(flms->part(MLS).x() - flms->part(MRS).x())) / (std::abs(flms->part(MLS).x() - flms->part(MCR).x()));
   if (1.6 <= mouth_width && mouth_width <= 1.62 )
	return true;      

 /*   double mvd, mar; //mouth vertical distance and mouth aspect ratio
    mvd = std::abs(flms->part(MCU).y() - flms->part(MCB).y());
    mar = mvd / std::abs(flms->part(MRS).y() - flms->part(MLS).y());
    return mar;
*/
}
