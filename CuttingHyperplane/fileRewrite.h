#pragma once
#ifndef FILEREWRITE_H
#define FILEREWRITE_H
#include "Hplane.h"
#include <fstream>
class FileRewrite
{
private:
	int numBoundaries;
	int numVariables;
	string line;
	Hyperplane hyp;
	ifstream f;
	ofstream out;
public:
	FileRewrite(Hyperplane _hyp) : hyp(_hyp) {}
	void fileRewriting();
	void fileRewritingAsDUAL();
	void openFiles();
	string changeNumberConstraints();
	string changeNumberVariables();
	string addHyperplane();
};


#endif
