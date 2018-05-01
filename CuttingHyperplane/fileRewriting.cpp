#include "fileRewrite.h"
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdio>
#include <iostream>
using namespace std;

void FileRewrite::openFiles()
{
	f.open("inputDUAL.txt");
	out.open("output.txt");
}

string FileRewrite::changeNumberConstraints()
{
	ostringstream sstream;
	numBoundaries = atof(line.c_str());
	numBoundaries++;
	sstream << numBoundaries;
	line = sstream.str();
	return line;
}

string FileRewrite::changeNumberVariables()
{
	ostringstream sstream;
	numVariables = atof(line.c_str());
	numVariables++;
	sstream << numVariables;
	line = sstream.str();
	return line;
}


string FileRewrite::addHyperplane()
{
	out << endl; /* ���������� �������� ������ */
	vector<double> coeff = hyp.convertToSimplex(); /* �������� ������������ ����� ������ */
	double size = coeff.size(); /* �������� ������ */
	double b = hyp.getB(); /* �������� ��������� ��������� b */
	bool isLessZero = hyp.getZeroStatus(); /* �������� �� ��������� b ������������� */
	ostringstream sstream;
	if (b > 0) /* ���, �� �������� */
	{
		for (int i = 0; i < size; i++) /* � 0 ������� �� ���������� ������������ ������� ������������ */
		{
			sstream << coeff[i];
			sstream << ' ';
		}
		for (int i = size; i < numVariables - 1; i++) /* ����� ������� ���� �� ���������� ������� */
		{
			sstream << '0';
			sstream << ' ';
		}
		sstream << 1; /* � ��������� ������� ������ 1 */
	}
	else /* ����������� b ������������� */
	{
		for (int i = 0; i < size; i++)/* � 0 ������� �� ���������� ������������ ������� ������������ c ��������������� ������ */
		{
			sstream << -coeff[i];
			sstream << ' ';
		}
		for (int i = size; i < numVariables - 1; i++) /* ����� ������� ���� �� ���������� ������� */
		{
			sstream << '0';
			sstream << ' ';
		}
		sstream << -1; /* � ��������� ������� ������ -1 */
	}
	line = sstream.str();
	return line;
}


void FileRewrite::fileRewritingAsDUAL()
{
	numBoundaries = numVariables = 0;
	openFiles();
	for (int lineno = 0; lineno < 2; lineno++) /*  ��������� ���������� ����������� � ����������, ������ ������ */
	{
		getline(f, line);
		switch (lineno)
		{
		case 0: /* ������ ������ ����� - ���������� ����������� */
			//line = changeNumberConstraints(); //�� �������� � ������������
			numBoundaries = atof(line.c_str());
			break;
		case 1: /* ������ ������ ����� - ���������� ���������� */
			line = changeNumberVariables();
			break;
		}
		out << line << endl; /* ������ ���������� ������� � ���� */
	}
	getline(f, line);
	out << line << endl;

	vector<double> coeff = hyp.getCoeff();

	for (int lineno = 0; lineno < numBoundaries; lineno++) /* ��������� � ������ �������, �.� ���������� ����� �������������� */
	{
		getline(f, line);
		ostringstream sstream;
		sstream << ' ';
		sstream << -coeff[lineno];
		line.insert(line.size(), sstream.str());
		out << line << endl; /* ������ ���������� ������� � ���� */
	}
	double b = hyp.getB();
	getline(f, line);
	out << line << endl;
	getline(f, line);
	out << line << endl;
	getline(f, line);
	out << line << endl;
	getline(f, line);
	ostringstream sstream;
	sstream << ' ';
	sstream << b;
	line.insert(line.size(), sstream.str());
	out << line << endl;
	f.close();
	out.close();
	rename("inputDUAL.txt", "temp.txt");
	rename("output.txt", "inputDUAL.txt");
	rename("temp.txt", "output.txt");
}

void FileRewrite::fileRewriting()
{
	numBoundaries = numVariables = 0;
	openFiles();
	for (int lineno = 0; lineno < 3; lineno++) /*  ��������� ���������� ����������� � ����������, ������ ������ */
	{
		getline(f, line);
		switch (lineno)
		{
		case 0: /* ������ ������ ����� - ���������� ����������� */
			line = changeNumberConstraints();
			break;
		case 1: /* ������ ������ ����� - ���������� ���������� */
			line = changeNumberVariables();
			break;
		case 2: /* ������ ������ ����� - ������, ������ ������� ������ � ���������� �������������� */
			line = addHyperplane();
			break;
		}
		out << line << endl; /* ������ ���������� ������� � ���� */
	}

	for (int lineno = 3; lineno < numBoundaries + 2; lineno++) /* ���� ������ ������������ ���������� ������ ���� */
	{
		getline(f, line);
		ostringstream sstream;
		sstream << ' ';
		sstream << 0;
		line.insert(line.size(), sstream.str());
		out << line << endl; /* ������ ���������� ������ � ���� */
	}
	getline(f, line);
	out << line << endl; /* ������� ������ � ������� b */

	getline(f, line);
	ostringstream sstream, sstreamC;
	sstream << abs(hyp.getB());
	sstream << ' ';
	line.insert(0, sstream.str());
	out << line << endl; /* ������ b ������� � ������ ���������� */
	getline(f, line);
	out << line << endl; /* ������� ������ � ������� � */
	getline(f, line);
	sstreamC << ' ';
	sstreamC << 0;
	line.insert(line.size(), sstreamC.str());
	out << line << endl; /* ������ c ������� */

	f.close();
	out.close();
	rename("input.txt", "temp.txt");
	rename("output.txt", "input.txt");
	rename("temp.txt", "output.txt");
}