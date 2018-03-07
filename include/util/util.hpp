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

double variance(std::list<int>);
void normalize(std::vector<double> *, int, int);
void cherry_pick(std::vector<double> *, std::vector<double>*, uint8_t);
void make_equal(std::vector<double> *, size_t);
