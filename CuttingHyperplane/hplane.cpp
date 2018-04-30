#include "Hplane.h"

void Hyperplane::print()
{
	std::cout << " Hyperplane: ";
	for (int i = 0; i < coeff.size(); i++)
		std::cout << " (" << coeff[i] << ") * x" << i << " + ";
	std::cout << -b << " < 0" << std::endl;
}

vector<double> Hyperplane::convertToSimplex()
{
	vector<double> u(2 * coeff.size());
	/* x1 = u1 - u2*/
	for (int i = 0; i < u.size() - 1; i+=2)
	{
		u[i] =  coeff[i / 2];
		u[i + 1] = -coeff[i / 2];
	}
	return u;
}