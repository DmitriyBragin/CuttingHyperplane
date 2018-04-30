#pragma once
#ifndef BOUNDS_H
#define BOUNDS_H
#include "hplane.h"

class Bounds
{
private:
	vector<Hyperplane> bounds;
	vector<double> x;
	int size;
	int numHyperplanes;
public:
	Bounds(int _size) : size(_size) { numHyperplanes = _size;
	for (int i = 0; i < numHyperplanes; i++)
	{
		Hyperplane hyp;
		bounds.push_back(hyp);
	}
	}
	Bounds() {}
	void startConstraints();
	void addHyperplane(Hyperplane hyp);
	double phi_x_opt();
	vector<double> subGrad();
	void setX(vector<double> _x) { x = _x; }
	Hyperplane getNewHyperplane();
	int getSize() { return size; }
	double vector_dot_vector(vector<double> vec1, vector<double> vec2);
};
#endif
