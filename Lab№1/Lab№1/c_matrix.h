#pragma once
#include <vector>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <random>
#include <string>
#include <stdexcept>

class c_matrix {
private:
	std::vector<std::vector<double>> matrix;

public:
	c_matrix();
	c_matrix(int);
	c_matrix(int, int);
	c_matrix(const c_matrix&);
	c_matrix(std::vector<std::vector<double>>&);

	~c_matrix();
	double& operator()(int, int);
	double& operator()(int);
	c_matrix operator-();
	c_matrix operator=(const c_matrix&);
	c_matrix operator*(c_matrix&);
	c_matrix operator+(c_matrix&);
	c_matrix operator-(c_matrix&);
	c_matrix operator+(double);
	c_matrix operator-(double);

	size_t RowsCount() const;
	size_t ColsCount() const;
	c_matrix Transpose();
	c_matrix GetRow(int);
	c_matrix GetRow(int, int, int);
	c_matrix GetCol(int);
	c_matrix GetCol(int, int, int);

	c_matrix RedimMatrix(int);
	c_matrix RedimMatrix(int, int);
	c_matrix RedimData(int);
	c_matrix RedimData(int, int);

	double GetMaxElement();
	double GetMinElement();

	std::vector<std::vector<double>> GetMatrix();

	void Show();
	void Random();
};