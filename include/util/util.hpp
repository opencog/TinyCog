/*
  Project: OpenCogER
  File: util.hpp
  Author: Dagim Sisay
  License: AGPL
  Date: March 2018
*/

#include <stdlib.h>
#include <vector>
#include <list>
#include <algorithm>
#include <stdint.h>
#include <cmath>
double variance(std::list<int>);
void normalize(std::vector<double> *s, int a=0, int b=1);
void cherry_pick(std::vector<double> *, std::vector<double> *, uint8_t space=2);
void make_equal(std::vector<double> *, size_t);
