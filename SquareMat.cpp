// adar101101@gmail.com

#include <stdexcept>
#include <iostream>
#include <cmath>
#include "SquareMat.hpp"

namespace st = std;

namespace Matrix {

// Constructor: create a square matrix with given size, initializing all elements to zero.

SquareMat::SquareMat(int rows, int columns) {
    if (rows != columns) {
        throw st::invalid_argument("Matrix must be square");
    }
    if (rows <= 0 || columns <= 0) {
        throw st::invalid_argument("Matrix dimensions must be positive");
    }
    this->rows = rows;
    this->columns = columns;
    this->size = rows * columns;
    data = new double*[rows];
    for (int i = 0; i < rows; ++i) {
        data[i] = new double[columns]();
    }

}

// Copy constructor: deep copy of another SquareMat.
SquareMat::SquareMat(const SquareMat& other) {
    rows = other.rows;
    columns = other.columns;
    size = other.size;
    data = new double*[rows];
    for (int i = 0; i < rows; ++i) {
        data[i] = new double[columns];
        for (int j = 0; j < columns; ++j) {
            data[i][j] = other.data[i][j];
        }
    }
}

// Move constructor: transfer ownership from another SquareMat (rvalue).
SquareMat::SquareMat(SquareMat&& other) noexcept
    : rows(other.rows), columns(other.columns), data(other.data), size(other.size) {
    other.data = nullptr;
    other.rows = 0;
    other.columns = 0;
    other.size = 0;
}

// Move assignment operator: transfer ownership from another SquareMat (rvalue).
SquareMat& SquareMat::operator=(SquareMat&& other) noexcept {
    if (this != &other) {
        for (int i = 0; i < rows; ++i) delete[] data[i];
        delete[] data;
        rows = other.rows;
        columns = other.columns;
        size = other.size;
        data = other.data;
        other.data = nullptr;
        other.rows = 0;
        other.columns = 0;
        other.size = 0;
    }
    return *this;
}

// Destructor: free allocated memory of matrix.
SquareMat::~SquareMat() {
    for (int i = 0; i < rows; ++i) { delete[] data[i]; }
    delete[] data;
}

// Copy assignment operator: deep copy from another SquareMat.
SquareMat& SquareMat::operator=(const SquareMat& other) {
    if (this == &other) return *this;
    for (int i = 0; i < rows; ++i) {
        delete[] data[i];
    }
    delete[] data;
    rows = other.rows;
    columns = other.columns;
    size = other.size;
    data = new double*[rows];
    for (int i = 0; i < rows; ++i) {
        data[i] = new double[columns];
        for (int j = 0; j < columns; ++j){
            data[i][j] = other.data[i][j];
        }
    }
    return *this;
}

// In-place matrix addition: add other to this matrix.
SquareMat& SquareMat::operator+=(const SquareMat& other) {
    *this = *this + other;
    return *this;
}

// In-place matrix subtraction: subtract other from this matrix.
SquareMat& SquareMat::operator-=(const SquareMat& other) {
    *this = *this - other;
    return *this;
}

// In-place matrix multiplication: multiply this matrix by other.
SquareMat& SquareMat::operator*=(const SquareMat& other) {
    *this = *this * other;
    return *this;
}

// In-place scalar multiplication: multiply this matrix by scalar.
SquareMat& SquareMat::operator*=(double scalar) {
    *this = *this * scalar;
    return *this;
}

// In-place scalar division: divide this matrix by scalar.
SquareMat& SquareMat::operator/=(double scalar) {
    *this = *this / scalar;
    return *this;
}

// In-place scalar modulo: apply modulo for each element with given scalar.
SquareMat& SquareMat::operator%=(const int scalar) {
    *this = *this % scalar;
    return *this;
}

// In-place element-wise modulo: apply element-wise modulo operation with other matrix.

SquareMat& SquareMat::operator%=(const SquareMat& other) {
    *this = *this % other;
    return *this;
}

// Prefix increment: increase each element by 1.

SquareMat& SquareMat::operator++() {
    for (int i = 0; i < rows; ++i){
        for (int j = 0; j < columns; ++j){
            data[i][j]++;
        }
    }
    return *this;
}

// Postfix increment: increase each element by 1, returns copy before increment.
SquareMat SquareMat::operator++(int) {
    SquareMat tmp(*this);
    ++(*this);
    return tmp;
}

// Prefix decrement: decrease each element by 1.

SquareMat& SquareMat::operator--() {
    for (int i = 0; i < rows; ++i){
        for (int j = 0; j < columns; ++j){
            data[i][j]--;
        }
    }
    return *this;
}

// Postfix decrement: decrease each element by 1, returns copy before decrement.
SquareMat SquareMat::operator--(int) {
    SquareMat tmp(*this);
    --(*this);
    return tmp;
}

// Access element at (row, col) with bounds checking.
double& SquareMat::operator()(int row, int col) {
    if (row < 0 || row >= rows || col < 0 || col >= columns) {
        throw std::out_of_range("Index out of range of matrix");
    }
    return data[row][col];
}

// Access element at (row, col) with bounds checking (const version).
const double& SquareMat::operator()(int row, int col) const {
    if (row < 0 || row >= rows || col < 0 || col >= columns) {
        throw std::out_of_range("Index out of range of matrix");
    }
    return data[row][col];
}

// Compare matrices for equality (all elements and size).
bool SquareMat::operator==(const SquareMat& other) const {
    if (rows != other.rows || columns != other.columns) {
        return false;
    }
    for (int i = 0; i < rows; ++i){
        for (int j = 0; j < columns; ++j){
            if (data[i][j] != other.data[i][j])
                return false;
        }
    }
    return true;
}

// Compare matrices for inequality.
bool SquareMat::operator!=(const SquareMat& other) const {
    return !(*this == other);
}

// Compare matrices: true if sum of this matrix > other.
bool SquareMat::operator>(const SquareMat& other) const {
    return this->countSum() > other.countSum();
}

// Compare matrices: true if sum of this matrix >= other.
bool SquareMat::operator>=(const SquareMat& other) const {
    return countSum() >= other.countSum();
}

// Compare matrices: true if sum of this matrix < other.
bool SquareMat::operator<(const SquareMat& other) const {
    return countSum() < other.countSum();
}

// Compare matrices: true if sum of this matrix <= other.
bool SquareMat::operator<=(const SquareMat& other) const {
    return countSum() <= other.countSum();
}

// Get number of rows in the matrix.
int SquareMat::getRows() const { return rows; }

// Get number of columns in the matrix.
int SquareMat::getCols() const { return columns; }

// Fill all elements of the matrix with given value.
void SquareMat::fill(double value) {
    for (int i = 0; i < rows; ++i){
        for (int j = 0; j < columns; ++j){
            data[i][j] = value;
        }
    }
}


// Calculate sum of all elements in the matrix.
double SquareMat::countSum() const {
    double sum = 0;
    for (int i = 0; i < rows; ++i){
        for (int j = 0; j < columns; ++j){
            sum += data[i][j];
        }
    }
    return sum;
}

// Matrix exponentiation: raise the matrix to an integer non-negative power.
SquareMat SquareMat::operator^(int scalar) const {
    if (scalar < 0) {
        throw std::invalid_argument("Negative exponents are not supported for matrices");
    }
    SquareMat result(rows, columns);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j)
            result[i][j] = (i == j) ? 1 : 0;
    }
    if (scalar == 0) { return result; }
    SquareMat helper(*this);
    for (int i = 0; i < scalar; i++){
            result *= *this;}

    return result;
}

// Calculate determinant of a square matrix (recursive for size > 2).
double getDeterminant(const SquareMat& mat)  {
    if (mat.getRows() == 2) {
        return (mat)(0, 0) * (mat)(1, 1) - (mat)(0, 1) * (mat)(1, 0);
    }
    double det = 0;
    for (int i = 0; i < mat.getCols(); ++i) {
        SquareMat minor(mat.getRows() - 1,  mat.getCols() - 1);
        for (int r = 1; r < mat.getRows(); ++r) {
            int colIndex = 0;
            for (int c = 0; c < mat.getCols(); ++c) {
                if (c == i) continue;
                minor[r - 1][colIndex] = mat[r][c];
                ++colIndex;
            }
        }
        det += ((i % 2 == 0) ? 1 : -1) * mat[0][i] * getDeterminant(minor);
    }
    return det;
}

// Determinant operator: returns the determinant of the matrix.
double SquareMat::operator!() const {
    if (rows != columns) {
        throw std::invalid_argument("Matrix must be square for determinant calculation");
    }
    if (rows == 1) { return (*this[0][0]); }
    else return getDeterminant(*this);
}

// Add two matrices (element-wise).
SquareMat operator+(const SquareMat& left, const SquareMat& right) {
    if (left.getRows() != right.getRows() || left.getCols() != right.getCols()) {
        throw std::invalid_argument("Matrices must have the same dimensions for addition");
    }
    SquareMat result(left.getRows(), left.getCols());
    for (int i = 0; i < left.getRows(); ++i){
        for (int j = 0; j < left.getCols(); ++j){
            result[i][j] = left[i][j] + right[i][j];
        }
    }
    return result;
}

// Subtract one matrix from another (element-wise).
SquareMat operator-(const SquareMat& left, const SquareMat& right) {
    if (left.getRows() != right.getRows() || left.getCols() != right.getCols()) {
        throw std::invalid_argument("Matrices must have the same dimensions for subtraction");
    }
    SquareMat result(left.getRows(), left.getCols());
    for (int i = 0; i < left.getRows(); ++i){
        for (int j = 0; j < left.getCols(); ++j){
            result[i][j] = left[i][j] - right[i][j];
        }
    }
    return result;
}

// Multiply two matrices (matrix product).

SquareMat operator*(const SquareMat& left, const SquareMat& right) {
    if (left.getRows() != right.getRows() || left.getCols() != right.getCols()) {
        throw std::invalid_argument("Matrices must have the same dimensions for multiplication");
    }
    SquareMat result(left.getRows(), left.getCols());
    for (int i = 0; i < left.getRows(); ++i) {
        for (int j = 0; j < right.getCols(); ++j) {
            result[i][j] = 0;
            for (int k = 0; k < left.getCols(); ++k) {
                result[i][j] += left[i][k] * right[k][j];
            }
        }
    }
    return result;
}

// Multiply each element by a scalar.
SquareMat operator*(const SquareMat& mat, double scalar) {
    SquareMat result(mat.getRows(), mat.getCols());
    for (int i = 0; i < mat.getRows(); ++i){
        for (int j = 0; j < mat.getCols(); ++j){
            result[i][j] = mat[i][j] * scalar;
        }
    }
    return result;
}

// Multiply each element by a scalar (scalar on left).
SquareMat operator*(double scalar, const SquareMat& mat) {
    return mat * scalar;
}

// Element-wise multiplication of two matrices.
SquareMat operator%(const SquareMat& left, const SquareMat& right) {
    if (left.getRows() != right.getRows() || left.getCols() != right.getCols()) {
        throw std::invalid_argument("Matrices must have the same dimensions for element-wise multiplication");
    }
    SquareMat result(left.getRows(), left.getCols());
    for (int i = 0; i < left.getRows(); ++i){
        for (int j = 0; j < left.getCols(); ++j){
            result[i][j] = left[i][j] * right[i][j];
        }
    }
    return result;

}

// Element-wise modulo operation (fmod) with a scalar.
SquareMat operator%(const SquareMat& mat, int scalar) {
    if (scalar == 0) {
        throw std::invalid_argument("Modulo by zero");
    }
    SquareMat result(mat.getRows(), mat.getCols());
    for (int i = 0; i < mat.getRows(); ++i)
        for (int j = 0; j < mat.getCols(); ++j){
            result[i][j] = std::fmod(mat[i][j], scalar);
    }
    return result;
}

// Divide each element by a scalar.
SquareMat operator/(const SquareMat& mat, double scalar) {
    if (scalar == 0.0) {
        throw std::invalid_argument("Division by zero");
    }
    SquareMat result(mat.getRows(), mat.getCols());
    for (int i = 0; i < mat.getRows(); ++i){
        for (int j = 0; j < mat.getCols(); ++j){
            result[i][j] = mat[i][j] / scalar;
        }
    }
    return result;
}

// Transpose of the matrix: returns transposed matrix.

SquareMat operator~(const SquareMat& mat) {
    SquareMat result(mat.getRows(), mat.getCols());
    for (int i = 0; i < mat.getRows(); ++i){
        for (int j = 0; j < mat.getCols(); ++j){
            result[i][j] = mat[j][i];
        }
    }
    return result;
}
// Output the matrix to an output stream, formatted as rows of elements.
std::ostream& operator<<(std::ostream& stream, const SquareMat& mat) {
    for (int i = 0; i < mat.getRows(); ++i) {
        for (int j = 0; j < mat.getCols(); ++j) {
            stream << "[ " << mat[i][j] << " ]";
        }
        stream << std::endl;
    }
    return stream;
}

}