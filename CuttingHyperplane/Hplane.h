#pragma once
#ifndef HPLANE_H
#define HPLANE_H
#include <vector>
#include <iostream>
//#include "bounds.h"
using namespace std;
class Hyperplane
{
private:
	vector<double> coeff; /* output coefficients */
	double b;
	vector<double> x; /* Input parameter */
	bool lessZero; /* True if less than zero */
public:
	void print();
	double getB() { return b; }
	void setB(double _b) { b = _b; }
	bool getZeroStatus() { return lessZero; }
	void setZeroStatus(bool flag) { lessZero = flag; }
	vector<double> getCoeff() { return coeff; }
	void setCoeff(vector<double> _coeff) { coeff = _coeff; }
	vector<double> convertToSimplex();
};


#endif
