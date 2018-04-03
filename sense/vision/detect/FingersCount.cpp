/*
  Project: OpenCogER
  File: FingersCount.cpp
  Author: Kidist Abraham <kidistabraham@gmail.com>
          Dagim Sisay <dagiopia@gmail.com>
  License: AGPL
  Date: March, 2018
*/


#include "sense/vision/FingersCount.hpp"


FingersCount::FingersCount(bool debug)
{
	this->debug = debug;
}

size_t FingersCount::largest_cntr_idx()
{
    std::vector<double> areas;
    for(size_t i = 0; i < cntrs.size(); i++)
        areas.push_back(cv::contourArea(cntrs[i]));
    return std::max_element(areas.begin(), areas.end()) - areas.begin();
}


uint8_t FingersCount::num_fingers(cv::Mat hand)
{
    f_tips.clear();
    cv::GaussianBlur(hand, hand, cv::Size(5, 5), 1.5);
    cv::findContours(hand, cntrs, hier, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
    if(cntrs.size() > 0){
        cntr = cntrs[largest_cntr_idx()];
	cv::convexHull(cntr, hull, false);
	if(hull.size() > 3)
	    cv::convexityDefects(cntr, hull, defects);
	count = 0;
	VV4i::const_iterator d_it = defects.begin();
	if(debug) printf("-----------------\n");
	while(d_it != defects.end())
	{
            st_idx = (*d_it)[0];
	    st_pt = cntr[st_idx];
            en_idx = (*d_it)[1];
	    en_pt = cntr[en_idx];
            far_idx = (*d_it)[2];
	    far_pt = cntr[far_idx];
	    depth = (*d_it)[3] / 256.0;
	    

	    if(debug) printf("Depth = %f\n", depth);
	    if (MIN_DEPTH < depth && MAX_DEPTH > depth){
	        ++count; 
		if(debug){
	            f_tips.push_back(st_pt);
		    f_tips.push_back(en_pt);
		    f_tips.push_back(far_pt);
		}
	    }
	    ++d_it;
	} //while d_it ! end
    }// if cntrs > 0
    return count;
}

