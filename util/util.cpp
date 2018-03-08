/*
  Project: OpenCogER
  File: util.cpp
  Author: Dagim Sisay
  License: AGPL
  Date: March 2018
*/

#include "util/util.hpp"


double variance(std::list<int> d)
{
    int m = 0;
    double v = 0;
    std::list<int>::const_iterator i = d.begin();
    for( ; i != d.end(); i++)
        m += *i; 
    m = m/d.size();
    for(i = d.begin() ; i != d.end(); i++)
        v += std::pow((double)(*i - m), 2.0);
    return (v/d.size());
}

void normalize(std::vector<double> *s, int a, int b)
{
    int c = b - a;
    auto mm = std::minmax_element(s->begin(), s->end());
    double mi = s->at(mm.first - s->begin());
    double mx = s->at(mm.second - s->begin());
    double mxi = mx - mi;

    std::vector<double>::iterator i = s->begin(); 
    for( ; i != s->end(); i++)
    	*i = a + (((*i - mi)*c) / mxi);
}

void cherry_pick(std::vector<double> *x, std::vector<double> *y, uint8_t space)
{
    for(size_t i = 0; i < x->size(); i++)
        if(i % space == 0)
	    y->push_back(x->at(i));
}

void make_equal(std::vector<double> *x, size_t size)
{
    size_t xs = x->size();
    std::vector<double> t(*x);
    std::vector<double>::const_iterator itb = t.begin(); 
    std::vector<double>::iterator itbx = x->begin(),
				itex = x->end();
    if(xs < size)
        for(size_t i = xs; i < size; i++)
        {
            if(i % xs == 0)
	        itb = t.begin();
	    x->push_back(*itb);
	    itb++;
        }
    else
        x->erase(itbx+size, x->end());

}
 
