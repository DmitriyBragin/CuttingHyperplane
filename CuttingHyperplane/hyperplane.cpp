#include "hyperplane.h"
#include <vector>
using namespace std;
vector<vector<double>> bounds(3,vector<double>(5)); /*WHY 2, WHY 3*/
bool flag = true;
/* linear function cTx */
double function(vector<double> x)
{
	return 4 * x[0] - x[1];
}
/* calculating subgradient */
vector<double> subGradient(vector<double> x)
{
	vector<double> subGrad(3); /* WHY 2? */

	/* end of starting bounds */
	double result = -100; /* WHY -100 */
	int index = 0;
	for (int i = 0; i < bounds.size(); i++)
	{
		double temp = x[0] * bounds[i][0] + x[1] * bounds[i][1] + x[2] * bounds[i][2] + bounds[i][3];
		if (temp >= result)
		{
			result = temp;
			index = i;
		}
	}
	if (index == 0)
	{
		subGrad[0] = 2 * x[0];
		subGrad[1] = 6 * x[1];
		subGrad[2] = 0;
		return subGrad;
	}
	else if (index == 1)
	{
		subGrad[0] = 6 * x[0];
		subGrad[1] = 0;
		subGrad[2] = 0;
		return subGrad;
	}
	else if (index == 2)
	{
		subGrad[0] =  x[0];
		subGrad[1] = 1;
		subGrad[2] = -1;

		return subGrad;
	}
	subGrad[0] = bounds[index][0];
	subGrad[1] = bounds[index][1];
	subGrad[2] = bounds[index][2];
	return subGrad;
}
/*phi(x_opt)*/

double phi_x_opt(vector<double> x_opt)
{
	/* starting bounds */
	bounds[0][0] = x_opt[0];
	bounds[0][1] = 3 * x_opt[1]; /* == x2 */
	bounds[0][2] = 0; /* x3 */
	bounds[0][3] = -4;

	bounds[1][0] = 3 * x_opt[0]; /* x1 */
	bounds[1][1] = 0; /* x2 */
	bounds[1][2] = 0; /* x3 */
	bounds[1][3] = -5; /* b */

	bounds[2][0] = x_opt[0]; /* x1 */
	bounds[2][1] = 1; /* x2 */
	bounds[2][2] = -1; /* x3 */
	bounds[2][3] = 0; /* b */

	/* end of starting bounds */
	double result = -100; /* WHY -100 */
	for(int i = 0; i < bounds.size(); i++)
	{
		double temp = x_opt[0] * bounds[i][0] + x_opt[1] * bounds[i][1] + x_opt[2] * bounds[i][2] + bounds[i][3];
		if (temp >= result)
		{
			result = temp;
		}
	}
	return result;
}



vector<double> hyperplaneModule(vector<double> x_opt)
{
	vector<double> nullVec(1,0);
	double phi0 = function(x_opt);

	double phi = phi_x_opt(x_opt);
	vector<double> a = subGradient(x_opt);
	double temp = a[0] * x_opt[0] + a[1] * x_opt[1] + a[2] * x_opt[2];
	double b = - phi + temp;
	vector<double> newHyperplane(5,0);
	if (b < 0)
	{
	//	a[0] = -a[0];
	//	a[1] = -a[1];
	//	b = -b;
		newHyperplane[4] = 2510;
	}
	newHyperplane[0] = a[0];
	newHyperplane[1] = a[1];
	newHyperplane[2] = a[2];
	newHyperplane[3] = -b;
	//for (int i = 0; i < bounds.size(); i++)
	//{
	//	if ((bounds[i][0] == a[0]) && (bounds[i][1] == a[1]) && (bounds[i][2] == -b))
	//		return nullVec;
	//}
	bounds.push_back(vector<double>(5));
	/* add to phi_x_opt! */
	bounds[bounds.size() - 1][0] = a[0];
	bounds[bounds.size() - 1][1] = a[1];
	bounds[bounds.size() - 1][2] = a[2];
	bounds[bounds.size() - 1][3] = -b;
	return newHyperplane;
}