#include "c_matrix.h"

c_matrix::c_matrix() {
	matrix.resize(1);
	matrix[0].resize(1);
}

c_matrix::c_matrix(int m_size) {
	if (m_size >= 0) {
		matrix.resize(m_size);
		for (auto& row : matrix) {
			row.resize(m_size);
		}
	}
	else
		throw std::invalid_argument("Params can't be negative");
}

c_matrix::c_matrix(int m_rows, int m_cols) {
	if (m_rows >= 0 && m_cols >= 0) {
		matrix.resize(m_rows);
		for (auto& row : matrix) {
			row.resize(m_cols);
		}
	}
	else
		throw std::invalid_argument("Params can't be negative");
}

c_matrix::c_matrix(const c_matrix &matrix) {
	this->matrix = matrix.matrix;
}

c_matrix::c_matrix(std::vector<std::vector<double>> &matrix) {
	this->matrix = matrix;
}

c_matrix::~c_matrix() = default;

double& c_matrix::operator()(int i, int j) {
	if (i < RowsCount() && i >= 0 && j < ColsCount() && j >= 0)
		return matrix[i][j];
	else
		throw std::out_of_range("Out of index exception");
}

double& c_matrix::operator()(int i) {
	if (ColsCount() == 1) {
		if (i < RowsCount() && i >= 0)
			return matrix[i][0];
		else
			throw std::out_of_range("Out of index exception");
	}
	else
		throw std::invalid_argument("ColsCount(): " + std::to_string(ColsCount()) + " not equals 1");
}

c_matrix c_matrix::operator-() {
	std::vector<std::vector<double>> newMatrix(matrix.size());
	for (int i = 0; i < RowsCount(); i++) {
		newMatrix[i].resize(ColsCount());
		for (int j = 0; j < ColsCount(); j++) {
			newMatrix[i].push_back(-matrix[i][j]);
		}
	}
	return c_matrix(newMatrix);
}

c_matrix c_matrix::operator=(const c_matrix& matrix) {
	if (this == &matrix)
		return *this;

	if (RowsCount() == matrix.RowsCount() && ColsCount() == matrix.ColsCount()) {
		this->matrix = matrix.matrix;
		return *this;
	}
	else
		throw std::invalid_argument("Matrices have different dimensions");
}

c_matrix c_matrix::operator*(c_matrix& matrix) {
	if (ColsCount() == matrix.RowsCount()) {
		double sum;
		std::vector<std::vector<double>> newMatrix(RowsCount());
		for (int i = 0; i < RowsCount(); i++) {
			for (int j = 0; j < matrix.ColsCount(); j++) {
				sum = 0;
				newMatrix[i].resize(matrix.ColsCount());
				for (int k = 0; k < ColsCount(); k++)
					sum += (*this)(i, k) * matrix(k, j);
				newMatrix[i][j] = sum;
			}
		}

		return c_matrix(newMatrix);
	}
	else
		throw std::invalid_argument("Can't multiply this matrices");
}

c_matrix c_matrix::operator+(c_matrix& matrix) {
	if (ColsCount() == matrix.ColsCount() && RowsCount() == matrix.RowsCount()) {
		std::vector<std::vector<double>> newMatrix(RowsCount());
		for (int i = 0; i < RowsCount(); i++) {
			newMatrix[i].resize(ColsCount());
			for (int j = 0; j < ColsCount(); j++) {
				newMatrix[i][j] = (*this)(i, j) + matrix(i, j);
			}
		}

		return c_matrix(newMatrix);
	}
	else 
		throw std::invalid_argument("Can't add this matrices");
}

c_matrix c_matrix::operator-(c_matrix& matrix) {
	if (ColsCount() == matrix.ColsCount() && RowsCount() == matrix.RowsCount()) {
		std::vector<std::vector<double>> newMatrix(RowsCount());
		for (int i = 0; i < RowsCount(); i++) {
			newMatrix[i].resize(ColsCount());
			for (int j = 0; j < ColsCount(); j++) {
				newMatrix[i][j] = (*this)(i, j) - matrix(i, j);
			}
		}

		return c_matrix(newMatrix);
	}
	else
		throw std::invalid_argument("Can't subtract this matrices");
}

c_matrix c_matrix::operator-(double num) {
	std::vector<std::vector<double>> newMatrix(RowsCount());
	for (int i = 0; i < RowsCount(); i++) {
		newMatrix[i].resize(ColsCount());
		for (int j = 0; j < ColsCount(); j++) {
			newMatrix[i][j] = (*this)(i, j) - num;
		}
	}

	return c_matrix(newMatrix);
}

c_matrix c_matrix::operator+(double num) {
	std::vector<std::vector<double>> newMatrix(RowsCount());
	for (int i = 0; i < RowsCount(); i++) {
		newMatrix[i].resize(ColsCount());
		for (int j = 0; j < ColsCount(); j++) {
			newMatrix[i][j] = (*this)(i, j) + num;
		}
	}

	return c_matrix(newMatrix);
}

size_t c_matrix::RowsCount() const {
	return matrix.size();
}

size_t c_matrix::ColsCount() const {
	return matrix[0].size();
}

c_matrix c_matrix::Transpose() {
	c_matrix result(ColsCount(), RowsCount());
	for (int i = 0; i < RowsCount(); ++i) {
		for (int j = 0; j < ColsCount(); ++j) {
			result(j, i) = matrix[i][j];
		}
	}
	return result;
}

c_matrix c_matrix::GetRow(int row) {
	if (row < RowsCount() && row >= 0) {
		c_matrix newVector(1, ColsCount());
		for (int j = 0; j < ColsCount(); j++) {
			newVector.matrix[0][j] = matrix[row][j];
		}

		return newVector;
	}
	else 
		throw std::out_of_range("Out of index exception");
}

c_matrix c_matrix::GetRow(int row, int x1, int x2) {
	if (row < RowsCount() && row >= 0 &&
		x1 <= x2 && x1 >= 0 &&
		x2 < ColsCount() && x2 >= 0) {
		c_matrix newVector(1, x2 - x1 + 1);
		for (int j = x1; j <= x2; j++) {
			newVector.matrix[0][j - x1] = (*this)(row, j);
		}

		return newVector;
	}
	else
		throw std::out_of_range("Out of index exception");
}

c_matrix c_matrix::GetCol(int col) {
	if (col < ColsCount() && col >= 0) {
		c_matrix newVector(RowsCount(), 1);
		for (int i = 0; i < RowsCount(); i++) {
			newVector.matrix[i][0] = matrix[i][col];
		}

		return newVector;
	}
	else
		throw std::out_of_range("Out of index exception");
}

c_matrix c_matrix::GetCol(int col, int x1, int x2) {
	if (col < ColsCount() && col >= 0 &&
		x1 <= x2 && x1 >= 0 &&
		x2 < RowsCount() && x2 >= 0) {
		c_matrix newVector(x2 - x1 + 1, 1);
		for (int i = x1; i <= x2; i++) {
			newVector.matrix[i - x1][0] = (*this)(i, col);
		}

		return newVector;
	}
	else
		throw std::out_of_range("Out of index exception");
}

c_matrix c_matrix::RedimMatrix(int rows) {
	matrix.resize(rows);
	for (int i = 0; i < RowsCount(); i++) {
		matrix[i].resize(1);
		matrix[i][0] = 0;
	}
	return *this;
}

c_matrix c_matrix::RedimMatrix(int rows, int cols) {
	matrix.resize(rows);
	for (int i = 0; i < RowsCount(); i++) {
		matrix[i].resize(cols);
		for (int j = 0; j < ColsCount(); j++) {
			matrix[i][j] = 0;
		}
	}
	return *this;
}

c_matrix c_matrix::RedimData(int rows) {
	this->matrix.resize(rows);
	for (auto& row : matrix) {
		row.resize(ColsCount());
	}
	return *this;
}

c_matrix c_matrix::RedimData(int rows, int cols) {
	c_matrix newMatrix(*this);
	this->RedimMatrix(rows, cols);
	int min_rows = std::min(rows, static_cast<int>(newMatrix.RowsCount()));
	int min_cols = std::min(cols, static_cast<int>(newMatrix.ColsCount()));
	for (int i = 0; i < min_rows; i++) {
		for (int j = 0; j < min_cols; j++) {
			matrix[i][j] = newMatrix(i, j);
		}
	}
	return *this;
}

double c_matrix::GetMaxElement() {
	double max = (*this)(0, 0);
	for (const auto& row : matrix) {
		for (const auto& i : row) {
			if (i > max)
				max = i;
		}
	}
	return max;
}

double c_matrix::GetMinElement() {
	double min = (*this)(0, 0);
	for (const auto& row : matrix) {
		for (const auto& i : row) {
			if (i < min)
				min = i;
		}
	}
	return min;
}

std::vector<std::vector<double>> c_matrix::GetMatrix() {
	return matrix;
}

void c_matrix::Show() {
	std::for_each(matrix.begin(), matrix.end(), [](std::vector<double> & ivec) {
			std::for_each(ivec.begin(), ivec.end(), [](double i) {
					std::cout << std::fixed << std::setprecision(2) << std::left << std::setw(10) << i << " ";
				});
			std::cout << std::endl;
		});
	std::cout << std::endl;
}

void c_matrix::Random() {
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<float> dist(1.0, 10.0);

	for (size_t i = 0; i < RowsCount(); i++) {
		for (size_t j = 0; j < ColsCount(); j++) {
			(*this)(i, j) = dist(mt);
		}
	}
}