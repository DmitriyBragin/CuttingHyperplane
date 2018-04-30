#include "Utils.h"

double Utils::calculateNorm(vector<double> x, vector<double> x_prev)
{
	double norm = 0;
	for (int i = 0; i < x.size(); i++)
	{
		norm += (x[i] - x_prev[i]) * (x[i] - x_prev[i]);
	}
	norm = sqrt(norm);
	return norm;
}

vector<double> Utils::convertFromSimplex(vector<double> u)
{
	vector<double> x(u.size() / 2);
	for (int i = 0; i < u.size() / 2; i+=2)
	{
		x[i / 2] = u[i] - u[i + 1];
	}
	return x;
}
