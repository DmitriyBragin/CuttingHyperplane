#pragma once
#ifndef UTILS_H
#define UTILS_H
#include <vector>

using namespace std;
class Utils
{
public:
	double calculateNorm(vector<double> x, vector<double> x_prev);
	vector<double> convertFromSimplex(vector<double> u);

};
#endif
