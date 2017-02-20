#include <iostream>
#include <cmath>

#include "matrix.hpp"

// Error used for comparisons
#define EPSILON 0.0000000001

// BEGIN CLASS

#define index(i, j) ((i) + (m_ * (j)))
#define check_size(mat) if (!((mat).m_ == m_ && (mat).n_ == n_)) throw ERR_INCOMPATIBLE_SIZE
#define for_ij(m, n) for (uint i = 0; i < (m); i++) for (uint j = 0; j < (n); j++)

Matrix::Matrix() {
	m_ = 0;
	n_ = 0;
	buf_ = nullptr;
}

Matrix::Matrix(uint rows, uint cols) {
	m_ = rows;
	n_ = cols;
	buf_ = new double[m_ * n_];
	for (uint k = 0; k < (m_ * n_); k++)
		buf_[k] = 0;
}

Matrix::Matrix(uint rows, uint cols, double *values) {
	m_ = rows;
	n_ = cols;
	buf_ = new double[m_ * n_];
	for (int k = 0; k < (m_ * n_); k++)
		buf_[k] = values[k];
}

Matrix::Matrix(uint rows, uint cols, int *values) {
	m_ = rows;
	n_ = cols;
	buf_ = new double[m_ * n_];
	for (int k = 0; k < (m_ * n_); k++)
		buf_[k] = (double) values[k];
}

Matrix::Matrix(const Matrix& mat) {
	m_ = mat.m_;
	n_ = mat.n_;
	buf_ = new double[m_ * n_];
	(*this) = mat;
}

Matrix::~Matrix() {
	delete[] buf_;
}

uint Matrix::getM() const {
	return m_;
}

uint Matrix::getN() const {
	return n_;
}

double Matrix::get(uint i, uint j) const {
	if (i >= m_ || j >= n_)
		throw ERR_OUT_OF_BOUNDS;
	return buf_[index(i, j)];
}

void Matrix::set(uint i, uint j, double value) {
	if (i >= m_ || j >= n_)
		throw ERR_OUT_OF_BOUNDS * 8;
	buf_[index(i, j)] = value;
}

bool Matrix::isSquare() const {
	return m_ == n_;
}

bool Matrix::isInvertible() const {
	return det() != 0;
}

Matrix Matrix::getRow(uint i) const {
	Matrix a(1, n_);
	for (uint j = 0; j < n_; j++)
		a.set(0, j, get(i, j));
	return a;
}

Matrix Matrix::getCol(uint j) const {
	Matrix a(m_, 1);
	for (uint i = 0; i < m_; i++)
		a.set(i, 0, get(i, j));
	return a;
}

void Matrix::setRow(uint i, const Matrix& row) {
	if (row.m_ != 1 || row.n_ != n_)
		throw ERR_INCOMPATIBLE_SIZE;
	for (int j = 0; j < n_; j++)
		set(i, j, row.get(0, j));
}

void Matrix::setCol(uint j, const Matrix& col) {
	if (col.n_ != 1 || col.m_ != m_)
		throw ERR_INCOMPATIBLE_SIZE;
	for (int i = 0; i < m_; i++)
		set(i, j, col.get(i, 0));
}

#ifndef DET_OLD
#include "permutation.cpp"
double Matrix::det() const {
	if (!isSquare())
		throw ERR_NOT_SQUARE;
	if (m_ < 2)
		throw ERR_INCOMPATIBLE_SIZE;
	if (m_ == 2) // Might as well speed things up
		return (get(0, 0) * get(1, 1)) - (get(0, 1) * get(1, 0));
	
	uint p[n_], v[n_];
	init_permute(n_, p, v);
	double detsum = 0, prod;
	int sgn = 1;
	do {
		prod = 1;
		for (uint i = 0; i < n_; i++)
			prod *= get(i, p[i]);
		prod *= sgn;
		sgn = -sgn;
		detsum += prod;
	} while (permute(n_, p, v));
	return detsum;
}
#else
double Matrix::det() const {
	if (!isSquare())
		throw ERR_NOT_SQUARE;
	if (m_ < 2)
		throw ERR_INCOMPATIBLE_SIZE;
	if (m_ == 2)
		return (get(0, 0) * get(1, 1)) - (get(0, 1) * get(1, 0));
	Matrix mat(m_ - 1, m_ - 1);
	double detsum = 0;
	int alt = 1;
	for (uint k = 0; k < m_; k++) {
		uint js = 0;
		for (uint j = 0; j < m_; j++) {
			if (j == k)
				continue;
			for (uint i = 1; i < m_; i++)
				mat.set(i - 1, js, get(i, j));
			js++;
		}
		detsum += mat.det() * get(0, k) * alt;
		alt *= -1;
	}
	return detsum;
}
#endif

Matrix Matrix::transpose() const {
	Matrix mat(n_, m_);
	for_ij(m_, n_)
		mat.set(j, i, get(i, j));
	return mat;
}

Matrix Matrix::adj() const {
	Matrix mat = cofactorMatrix().transpose();
	return mat;
}

Matrix Matrix::subMatrix(uint i, uint j) const {
	Matrix mat(m_ - 1, n_ - 1);
	uint is = 0, js;
	for (uint ip = 0; ip < m_; ip++) {
		if (ip == i)
			continue;
		js = 0;
		for (uint jp = 0; jp < n_; jp++) {
			if (jp == j)
				continue;
			mat.set(is, js, get(ip, jp));
			++js;
		}
		++is;
	}
	return mat;
}

double Matrix::minordet(uint i, uint j) const {
	if (!isSquare())
		throw ERR_NOT_SQUARE;
	if (m_ < 3)
		throw ERR_INCOMPATIBLE_SIZE;
	return subMatrix(i, j).det();
}

double Matrix::cofactor(uint i, uint j) const {
	double deter = minordet(i, j);
	return ((i + j) % 2) ? -deter : deter;
}

Matrix Matrix::minorMatrix() const {
	if (!isSquare())
		throw ERR_NOT_SQUARE;
	Matrix mat(m_, m_);
	for (uint i = 0; i < m_; i++)
	for (uint j = 0; j < m_; j++)
		mat.set(i, j, minordet(i, j));
	return mat;
}

Matrix Matrix::cofactorMatrix() const {
	Matrix mat = minorMatrix();
	int alt = 1;
	for (uint k = 0; k < (m_ * n_); k++) {
		mat.buf_[k] *= alt;
		alt *= -1;
	}
	return mat;
}

Matrix Matrix::invert() const {
	if (!isSquare())
		throw ERR_NOT_SQUARE;
	double deter = det();
	if (deter == 0)
		throw ERR_NOT_INVERTIBLE;
	Matrix mat;
	if (m_ == 2) {
		mat = Matrix(2, 2);
		mat.set(0, 0, get(1, 1));
		mat.set(1, 1, get(0, 0));
		mat.set(0, 1, -get(0, 1));
		mat.set(1, 0, -get(1, 0));
	} else
		mat = adj();
	mat /= deter;
	return mat;
}

Matrix& Matrix::operator=(const Matrix& a) {
	if (m_ == 0 || n_ == 0) {
		m_ = a.getM();
		n_ = a.getN();
		buf_ = new double[m_ * n_];
	} else
		check_size(a);
	for_ij(m_, n_) set(i, j, a.get(i, j));
	return *this;
}

Matrix Matrix::operator-() {
	Matrix a(m_, n_);
	for_ij(m_, n_) buf_[index(i, j)] *= -1;
	return a;
}

Matrix& Matrix::operator+=(const Matrix& a) {
	check_size(a);
	for_ij(m_, n_) buf_[index(i, j)] += a.buf_[index(i, j)];
	return *this;
}

Matrix& Matrix::operator-=(const Matrix& a) {
	check_size(a);
	for_ij(m_, n_) buf_[index(i, j)] -= a.buf_[index(i, j)];
	return *this;
}

Matrix& Matrix::operator*=(const Matrix& a) {
	Matrix mat = a * *this;
	*this = mat;
	return *this;
}

Matrix& Matrix::operator*=(double a) {
	for_ij(m_, n_) buf_[index(i, j)] *= a;
	return *this;
}

Matrix& Matrix::operator/=(double a) {
	for_ij(m_, n_) buf_[index(i, j)] /= a;
	return *this;
}

bool Matrix::operator==(const Matrix& a) {
	if (a.m_ != m_ || a.n_ != n_)
		return false;
	for (uint k = 0; k < (m_ * n_); k++)
		if (abs(a.buf_[k] - buf_[k]) > EPSILON)
			return false;
	return true;
}

#undef index
#undef check_size
#undef for_ij

// END CLASS

Matrix identityMatrix(uint m) {
	Matrix mat(m, m);
	for (uint k = 0; k < m; k++)
		mat.set(k, k, 1);
	return mat;
}

std::ostream& operator<<(std::ostream& out, const Matrix& a) {
	double val;
	for (uint i = 0; i < a.getM(); i++) {
		out << "|\t";
		for (uint j = 0; j < a.getN(); j++) {
			if (j)
				out << '\t';
			val = a.get(i, j);
			if (std::abs(val) < EPSILON)
				val = 0;
			out << val;
		}
		out << "\t|" << std::endl;
	}
	return out;
}

Matrix operator+(const Matrix& a, const Matrix& b) {
	Matrix mat(a);
	mat += b;
	return mat;
}

Matrix operator-(const Matrix& a, const Matrix& b) {
	Matrix mat(a);
	mat -= b;
	return mat;
}

Matrix operator*(const Matrix& a, double b) {
	Matrix mat(a);
	mat *= b;
	return mat;
}

Matrix operator*(double a, const Matrix& b) {
	return b * a;
}

Matrix operator*(const Matrix& a, const Matrix& b) {
	if (a.getN() != b.getM())
		throw ERR_INCOMPATIBLE_SIZE;
	Matrix mat(a.getM(), b.getN());
	for (uint i = 0; i < a.getM(); i++)
	for (uint j = 0; j < b.getN(); j++) {
		double sum = 0;
		for (uint k = 0; k < a.getN(); k++)
			sum += a.get(i, k) * b.get(k, j);
		mat.set(i, j, sum);
	}
	return mat;
}

Matrix operator/(const Matrix& a, double b) {
	Matrix mat(a);
	mat /= b;
	return mat;
}

