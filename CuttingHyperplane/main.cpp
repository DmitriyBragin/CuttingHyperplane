#include "main.h"
#include "hyperplane.h"
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdio>
#include <iostream>
using namespace std;
vector<double> convertFromSimplex(vector<double> u)
{
	vector<double> x(u.size());
	x[0] = u[0] - u[1]; /* x1 = u1 - u2 */
	x[1] = u[2] - u[3]; /* x2 = u3 - u4 */
	x[2] = u[4] - u[5];
	return x;
}
vector<double> convertToSimplex(vector<double> hyp)
{
	vector<double> u(2 * (hyp.size() -1) - 1);
	/* x1 = u1 - u2*/
	u[0] = hyp[0];
	u[1] = -hyp[0];
	u[2] = hyp[1];
	u[3] = -hyp[1];
	u[4] = hyp[2];
	u[5] = -hyp[2];
	u[6] = hyp[4];
	return u;
}

void fileRewriting(vector<double> newHyperplane, double b)
{
	ifstream f;
	f.open("input.txt");
	ofstream out("output.txt");
	string line;
	double num_variables = 0;
	double num_boundaries = 0;
	int flag = 0;
	int newflag = 0;
	for (int lineno = 0; getline(f, line); lineno++)
	{
		/* Changing number of ogranicheniya */
		if (lineno == 0)
		{
			std::ostringstream sstream;
			num_boundaries = atof(line.c_str());
			num_boundaries++;
			sstream << num_boundaries;
			line = sstream.str();
		}
		if (lineno == 1)
		{
			std::ostringstream sstream;
			num_variables = atof(line.c_str());
			num_variables++;
			sstream << num_variables;
			line = sstream.str();
		}
		/* adding ogranicheniya */
		if (lineno == 2)
		{
			out << endl;
			std::ostringstream sstream;
			
			if (newHyperplane[newHyperplane.size() - 1] == 2510) /* OMG 2510 */
			{
				for (int i = 0; i < newHyperplane.size() - 1; i++)
				{
					sstream << -newHyperplane[i];
					sstream << ' ';
				}
				for (int i = newHyperplane.size() - 1; i < num_variables - 1; i++) /* WHY 9 */
				{
					sstream << '0';
					sstream << ' ';
				}
				sstream << -1;
			}
			else
			{
				for (int i = 0; i < newHyperplane.size() - 1; i++)
				{
					sstream << newHyperplane[i];
					sstream << ' ';
				}
				for (int i = newHyperplane.size() - 1; i < num_variables - 1; i++) /* WHY 9 */
				{
					sstream << '0';
					sstream << ' ';
				}
				sstream << 1;
			}
			line = sstream.str();
		}
		/*adding new right vector */
		if (flag == 1)
		{
			std::ostringstream sstream;
			sstream << b;
			sstream << ' ';
			line.insert(0, sstream.str());
			flag = 2;
			out << line << endl;
			continue;
		}
		/* Adding new variables to all old ogranicheniya */
		if ((lineno >= 3)) /* WHY 3*/
		{
			if (line != "")
			{
				std::ostringstream sstream;
				sstream << ' ';
				sstream << 0;
				line.insert(line.size(), sstream.str());
			}
			else 
			{
				if(flag != 2)
					flag = 1;
				out << line << endl;
				continue;
			}
		}
		out << line << endl;
	}
	f.close();
	out.close();
	rename("input.txt", "temp.txt");
	rename("output.txt", "input.txt");
	rename("temp.txt", "output.txt");
}
int main(void)
{
	vector<double> x_prev(3);
	vector<double> x;
	vector<double> hyp;
	x_prev[0] = 100;
	x_prev[1] = 100;
	x_prev[2] = 100;
	double stopper = 99; 
	double count = 0;
	while (stopper > 0)
	{
		x = simplexModule(); /* x_opt has initial vector */
		x = convertFromSimplex(x); /* initial vector in standart form */
		x.resize(3); /* WHY 2*/
		double norm = sqrt((x[0] - x_prev[0])*(x[0] - x_prev[0]) + (x[1] - x_prev[1])*(x[1] - x_prev[1]) + (x[2] - x_prev[2])*(x[2] - x_prev[2]));
		if ((norm < 0.001))
		{
			break;
		}
		x_prev[0] = x[0];
		x_prev[1] = x[1];
		x_prev[2] = x[2];
		hyp = hyperplaneModule(x); /* got coefficients in standart form */
		double b = -hyp[hyp.size() - 2]; /* last coefficient in hyp is right vector b */
		std::cout << hyp[0] << " * x1 + " << hyp[1] << "* x2 -" << hyp[2] << "* x3 -" << b << endl;
		vector<double> newHyperplane;
		newHyperplane = convertToSimplex(hyp);
		if (newHyperplane[newHyperplane.size() - 1] == 2510)
			b = -b;
		fileRewriting(newHyperplane, b);
		stopper--;
		count++;
	} 
	cout << "Optimum x: <" << x[0] << ";" << x[1] << ";" << x[2] <<  "> count steps: " << count <<endl;

	return 0;
}