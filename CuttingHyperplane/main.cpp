#include "main.h"
#include "Hplane.h"
#include "hyperplane.h"
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdio>
#include <iostream>
#include "bounds.h"
#include "Utils.h"
#include "fileRewrite.h"

using namespace std;
int main(void)
{
	Utils utils;
	int size = 3; /* num of variables */
	Bounds B(size);
	vector<double> x_prev(size, rand() % RAND_MAX);
	vector<double> x(size, rand() % RAND_MAX);
	double epsilon = 0.000001;
	int countSteps = 0;
	double norm = utils.calculateNorm(x, x_prev);
	while (norm > epsilon)
	{
		x = simplexModule(); /* x_opt has initial vector */
		x = utils.convertFromSimplex(x); /* initial vector in standart form */
		x.resize(size); 
		norm = utils.calculateNorm(x, x_prev);
		x_prev = x;

		B.setX(x);
		Hyperplane newHyp = B.getNewHyperplane();
		double b = newHyp.getB();
		newHyp.print();

		vector<double> newHyperplane;
		newHyperplane = newHyp.convertToSimplex();
		FileRewrite FR(newHyp);
		FR.fileRewriting();
		countSteps++;
	} 
	cout << "Optimum x: <" << x[0] << ";" << x[1] << ";" << x[2] <<  "> count steps: " << countSteps <<endl;

	return 0;
}