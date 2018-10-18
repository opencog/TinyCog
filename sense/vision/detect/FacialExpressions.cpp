/*

  Project: OpenCogER
  File: FacialExpressions.cpp
  Author: Dagim Sisay <dagiopia@gmail.com>
  License: APGL
  Date: March 2018
*/

#include "sense/vision/FacialExpressions.hpp"

double FacialExpressions::distance(dlib::point p1, dlib::point p2)
{
	return abs(sqrt((p1.x()*p1.x()) + (p1.y()*p1.y())) - 
	       sqrt((p2.x()*p2.x()) + (p2.y()*p2.y())));
}


double FacialExpressions::eyes_state(facial_lms flms)
{
	return (distance(flms.part(RE(1)), flms.part(RE(5))) + 
	        distance(flms.part(RE(2)), flms.part(RE(4)))) /
			  (2 * (distance(flms.part(RE(0)), flms.part(RE(3)))));
}
