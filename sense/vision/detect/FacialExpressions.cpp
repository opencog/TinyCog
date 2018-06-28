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

   
    double Leye,Reye;//left eye, right eye
    bool eyes_open;
    Leye = (std::abs(flms->part(LETA).y() - flms->part(LEU1).y())) / (std::abs(flms->part(LEU1).y() - flms->part(LEB1).y()));
    Reye = (std::abs(flms->part(RETA).y() - flms->part(REU1).y())) / (std::abs(flms->part(REU1).y() - flms->part(REB1).y()));     
   return ( Leye == phi || Reye == phi )? true;
}

 uint8_t FacialExpressions::natural_face() 
 {

  double mouth_width;
  bool natural_face = true;
  bool samily_face = false;
   mouth_width = (std::abs(flms->part(MLS).x() - flms->part(MRS).x())) / (std::abs(flms->part(MLS).x() - flms->part(MCU3).x()));
   return ( mouth_width ==phi) ? false: true;
}
