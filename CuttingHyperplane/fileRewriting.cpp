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
	out << endl; /* Добавление перевода строки */
	vector<double> coeff = hyp.convertToSimplex(); /* Получаем коэффициенты перед иксами */
	double size = coeff.size(); /* Получаем размер */
	double b = hyp.getB(); /* Получаем свободную константу b */
	bool isLessZero = hyp.getZeroStatus(); /* Является ли константа b отрицательной */
	ostringstream sstream;
	if (b > 0) /* Нет, не является */
	{
		for (int i = 0; i < size; i++) /* с 0 позиции до последнего коэффициента выводим коэффициенты */
		{
			sstream << coeff[i];
			sstream << ' ';
		}
		for (int i = size; i < numVariables - 1; i++) /* Затем выводим нули до последнего стоблца */
		{
			sstream << '0';
			sstream << ' ';
		}
		sstream << 1; /* В последнем столбце ставим 1 */
	}
	else /* Коэффициент b Отрицательный */
	{
		for (int i = 0; i < size; i++)/* с 0 позиции до последнего коэффициента выводим коэффициенты c противоположным знаком */
		{
			sstream << -coeff[i];
			sstream << ' ';
		}
		for (int i = size; i < numVariables - 1; i++) /* Затем выводим нули до последнего стоблца */
		{
			sstream << '0';
			sstream << ' ';
		}
		sstream << -1; /* В последнем столбце ставим -1 */
	}
	line = sstream.str();
	return line;
}


void FileRewrite::fileRewritingAsDUAL()
{
	numBoundaries = numVariables = 0;
	openFiles();
	for (int lineno = 0; lineno < 2; lineno++) /*  Изменения количества ограничений и переменных, запись нового */
	{
		getline(f, line);
		switch (lineno)
		{
		case 0: /* Первая строка файла - количество ограничений */
			//line = changeNumberConstraints(); //НЕ ИЗМЕНЯЕМ В ДВОЙСТВЕННОЙ
			numBoundaries = atof(line.c_str());
			break;
		case 1: /* Вторая строка файла - количество переменных */
			line = changeNumberVariables();
			break;
		}
		out << line << endl; /* Запись полученной строчки в файл */
	}
	getline(f, line);
	out << line << endl;

	vector<double> coeff = hyp.getCoeff();

	for (int lineno = 0; lineno < numBoundaries; lineno++) /* Изменение в каждой строчке, т.е добавление новой гиперплоскости */
	{
		getline(f, line);
		ostringstream sstream;
		sstream << ' ';
		sstream << -coeff[lineno];
		line.insert(line.size(), sstream.str());
		out << line << endl; /* Запись полученной строчки в файл */
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
	for (int lineno = 0; lineno < 3; lineno++) /*  Изменения количества ограничений и переменных, запись нового */
	{
		getline(f, line);
		switch (lineno)
		{
		case 0: /* Первая строка файла - количество ограничений */
			line = changeNumberConstraints();
			break;
		case 1: /* Вторая строка файла - количество переменных */
			line = changeNumberVariables();
			break;
		case 2: /* Третья строка файла - пустая, делаем перевод строки и записываем гиперплоскость */
			line = addHyperplane();
			break;
		}
		out << line << endl; /* Запись полученной строчки в файл */
	}

	for (int lineno = 3; lineno < numBoundaries + 2; lineno++) /* Всем старым ограничениям дописываем справа ноль */
	{
		getline(f, line);
		ostringstream sstream;
		sstream << ' ';
		sstream << 0;
		line.insert(line.size(), sstream.str());
		out << line << endl; /* Запись полученной строки в файл */
	}
	getline(f, line);
	out << line << endl; /* Перевод строки к вектору b */

	getline(f, line);
	ostringstream sstream, sstreamC;
	sstream << abs(hyp.getB());
	sstream << ' ';
	line.insert(0, sstream.str());
	out << line << endl; /* Вектор b дописан и строка переведена */
	getline(f, line);
	out << line << endl; /* Перевод строки к вектору с */
	getline(f, line);
	sstreamC << ' ';
	sstreamC << 0;
	line.insert(line.size(), sstreamC.str());
	out << line << endl; /* Вектор c записан */

	f.close();
	out.close();
	rename("input.txt", "temp.txt");
	rename("output.txt", "input.txt");
	rename("temp.txt", "output.txt");
}