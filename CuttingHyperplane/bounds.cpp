#include "bounds.h"

void Bounds::startConstraints()
{
	vector<double> constraint(size);
	double b = 0;
	constraint[0] = x[0];
	constraint[1] = 3.0 * x[1];
	constraint[2] = 0.0;
	b = 4.0;
	
	/*constraint[0] = 1.0;
	constraint[1] = x[1];
	b = 1.0;
	*/
	bounds[0].setCoeff(constraint);
	bounds[0].setB(b);
	bounds[0].setZeroStatus(false);

	
	constraint[0] = 3.0 * x[0];
	constraint[1] = 0.0;
	constraint[2] = 0.0;
	b = 5.0;
	
	/*
	constraint[0] = -1.0;
	constraint[1] = 1.0;
	b = 0.0;
	*/
	bounds[1].setCoeff(constraint);
	bounds[1].setB(b);
	bounds[1].setZeroStatus(false);

	constraint[0] = x[0];
	constraint[1] = 1.0;
	constraint[2] = -1.0;
	b = 0.0;

	bounds[2].setCoeff(constraint);
	bounds[2].setB(b);
	bounds[2].setZeroStatus(false);

}


double Bounds::phi_x_opt()
{
	vector<double> coeff;
	double result = -100; /* WHY -100 */
	for (int i = 0; i < numHyperplanes; i++)
	{
		coeff = bounds[i].getCoeff();
		double b = bounds[i].getB();
		double temp = vector_dot_vector(coeff, x) - b;
		if (temp >= result)
		{
			result = temp;
		}
	}
	return result;
}

vector<double> Bounds::subGrad()
{
	vector<double> subGrad(size); 
	vector<double> coeff;
	double result = -100; // Minimum of int
	int index = 0;
	for (int i = 0; i < numHyperplanes; i++)
	{
		coeff = bounds[i].getCoeff();
		double b = bounds[i].getB();
		double temp = vector_dot_vector(coeff, x) - b;
		if (temp >= result)
		{
			result = temp;
			index = i;
		}
	}
	if (index == 0)
	{
		subGrad[0] = 2.0 * x[0];
		subGrad[1] = 6.0 * x[1];
		subGrad[2] = 0.0; 
	//	subGrad[0] = 1.0 ;
	//	subGrad[1] = 2.0 * x[1];
		return subGrad;
	}
	else if (index == 1)
	{
		subGrad[0] = 6.0 * x[0];
		subGrad[1] = 0.0;
		subGrad[2] = 0.0;
	//	subGrad[0] = -1.0;
	//	subGrad[1] = 1.0;
		return subGrad;
	}
	else if (index == 2)
	{
		//subGrad[0] = 2.0 * x[0]; // онвелс ме рюй??
		subGrad[0] = 2.0* x[0];
		subGrad[1] = 1.0;
		subGrad[2] = -1.0;
		return subGrad;
	}
	subGrad = bounds[index].getCoeff();
	return subGrad;
}

void Bounds::addHyperplane(Hyperplane hyp)
{
	bounds.push_back(hyp);
}

double Bounds::vector_dot_vector(vector<double> vec1, vector<double> vec2)
{
	double res = 0;
	for(int i = 0; i < size; i++)
	{
		res += vec1[i] * vec2[i];
	}
	return res;
}

Hyperplane Bounds::getNewHyperplane()
{
	startConstraints();
	double phi = phi_x_opt();
	vector<double> a = subGrad();
	double temp = vector_dot_vector(a,x);
	double b = -phi + temp;
	Hyperplane newHyp;
	if (b <= 0)
		newHyp.setZeroStatus(true);
	else newHyp.setZeroStatus(false);
	newHyp.setB(b);
	newHyp.setCoeff(a);
	addHyperplane(newHyp);
	numHyperplanes++;
	return newHyp;
}