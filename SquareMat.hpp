// adar101101@gmail.com

#pragma once
#include <iostream>
#include <stdexcept>

/**
 * @file SquareMat.hpp
 * @brief Declaration of the SquareMat class for square matrix operations, with comprehensive documentation.
 */

namespace Matrix {

/**
 * @class SquareMat
 * @brief Represents a square matrix of doubles with extensive operator overloading for arithmetic and utility operations.
 *
 * This class supports deep copy, move semantics, arithmetic operations (including element-wise and scalar),
 * increment/decrement, comparisons, matrix exponentiation, determinant calculation, and more.
 * All operations enforce square matrix dimensions unless explicitly stated.
 */
class SquareMat {
private:
    int rows;         
    int columns;         
    double** data;  


public:
    size_t size;   
      

    // 
    // Constructors & Destructor
    // 

    /**
     * @brief Constructs a square matrix of given size.
     * @param rows Number of rows (must equal columns).
     * @param columns Number of columns (must equal rows).
     */
    SquareMat(int rows, int columns);

    /**
     * @brief Copy constructor. Performs a deep copy of another matrix.
     * @param other Matrix to copy.
     */
    SquareMat(const SquareMat& other);

    /**
     * @brief Move constructor. Transfers ownership of resources from another matrix.
     * @param other Matrix to move from.
     */
    SquareMat(SquareMat&& other) noexcept;

    /**
     * @brief Copy assignment operator. Deep copies another matrix into this one.
     * @param other Matrix to copy.
     * @return Reference to this matrix.
     */
    SquareMat& operator=(const SquareMat& other);

    /**
     * @brief Move assignment operator. Transfers resources from another matrix into this one.
     * @param other Matrix to move from.
     * @return Reference to this matrix.
     */
    SquareMat& operator=(SquareMat&& other) noexcept;

    /**
     * @brief Destructor. Frees all allocated memory.
     */
    ~SquareMat();

    
    /**
     * @brief Return matrix row, given row index. can be used by adding another [] to the return value for get cell data.
     * @param row Index of wanted row
     * @return Pointer to the wanted row
     */
 

    double* operator[](size_t row) const {
    if (row >= (size_t)rows) throw std::out_of_range("Row index out of range");
    return this->data[row];
}
    

    // 
    // Element Access 
    // 

    /**
     * @brief Accesses/modifies the element at (row, col).
     * @param row Rows number.
     * @param col Columns number.
     * @return Reference to the element.
     */
    double& operator()(int row, int col);

    /**
     * @brief Accesses the element at (row, col), for const contexts.
     * @param row Rows number.
     * @param col Columns number.
     * @return Const reference to the element.
     */
    const double& operator()(int row, int col) const;

    // 
    // Arithmetic Assignment Operators (in-place)
    // 

    /**
     * @brief In-place matrix addition.
     * @param other Matrix to add.
     * @return Reference to this matrix.
     */
    SquareMat& operator+=(const SquareMat& other);

    /**
     * @brief In-place matrix subtraction.
     * @param other Matrix to subtract.
     * @return Reference to this matrix.
     */
    SquareMat& operator-=(const SquareMat& other);

    /**
     * @brief In-place matrix multiplication.
     * @param other Matrix to multiply by.
     * @return Reference to this matrix.
     */
    SquareMat& operator*=(const SquareMat& other);

    /**
     * @brief In-place scalar multiplication.
     * @param scalar Scalar value to multiply by.
     * @return Reference to this matrix.
     */
    SquareMat& operator*=(double scalar);

    /**
     * @brief In-place scalar division.
     * @param scalar Scalar value to divide by.
     * @return Reference to this matrix.
     * @throws std::invalid_argument if scalar == 0.
     */
    SquareMat& operator/=(double scalar);

    /**
     * @brief In-place scalar modulo operation (applies fmod to each element).
     * @param scalar Scalar divisor.
     * @return Reference to this matrix.
     * @throws std::invalid_argument if scalar == 0.
     */
    SquareMat& operator%=(const int scalar);

    /**
     * @brief In-place element-wise modulo operation.
     * @param other Matrix to modulo with.
     * @return Reference to this matrix.
     */
    SquareMat& operator%=(const SquareMat& other);

    // 
    // Increment / Decrement
    // 

    /**
     * @brief Prefix increment: increases each element by 1.
     * @return Reference to this matrix.
     */
    SquareMat& operator++();

    /**
     * @brief Postfix increment: increases each element by 1, returns copy before increment.
     * @return Copy of this matrix before increment.
     */
    SquareMat operator++(int);

    /**
     * @brief Prefix decrement: decreases each element by 1.
     * @return Reference to this matrix.
     */
    SquareMat& operator--();

    /**
     * @brief Postfix decrement: decreases each element by 1, returns copy before decrement.
     * @return Copy of this matrix before decrement.
     */
    SquareMat operator--(int);

    // 
    // Comparison Operators
    // 

    /**
     * @brief Checks if two matrices are equal (all elements equal and same size).
     * @param other Matrix to compare.
     * @return True if equal.
     */
    bool operator==(const SquareMat& other) const;

    /**
     * @brief Checks if two matrices are not equal.
     * @param other Matrix to compare.
     * @return True if not equal.
     */
    bool operator!=(const SquareMat& other) const;

    /**
     * @brief Compares sum of elements. True if this matrix's sum > other's sum.
     * @param other Matrix to compare.
     * @return True if sum is greater.
     */
    bool operator>(const SquareMat& other) const;

    /**
     * @brief Compares sum of elements. True if this matrix's sum >= other's sum.
     * @param other Matrix to compare.
     * @return True if sum is greater or equal.
     */
    bool operator>=(const SquareMat& other) const;

    /**
     * @brief Compares sum of elements. True if this matrix's sum < other's sum.
     * @param other Matrix to compare.
     * @return True if sum is less.
     */
    bool operator<(const SquareMat& other) const;

    /**
     * @brief Compares sum of elements. True if this matrix's sum <= other's sum.
     * @param other Matrix to compare.
     * @return True if sum is less or equal.
     */
    bool operator<=(const SquareMat& other) const;

    // 
    // Utilities
    // 

    /**
     * @brief Returns the number of rows.
     * @return Number of rows.
     */
    int getRows() const;

    /**
     * @brief Returns the number of columns.
     * @return Number of columns.
     */
    int getCols() const;

    /**
     * @brief Sets all elements to the specified value.
     * @param value Value to assign to all elements.
     */
    void fill(double value);

    /**
     * @brief Returns the sum of all elements in the matrix.
     * @return Sum of elements.
     */
    double countSum() const;

    // 
    // Exponentiation and Determinant
    // 

    /**
     * @brief Raises the matrix to an integer non-negative power.
     * @param power Exponent (must be > 0).
     * @return Matrix raised to the given power.
     * @throws std::invalid_argument if power < 0.
     */
    SquareMat operator^(int power) const;

    /**
     * @brief Computes the determinant of the matrix.
     * @return Determinant value.
     */
    double operator!() const;

    // 
    // Friend Non-member Operators
    // 

    /**
     * @brief Adds two matrices (element-wise).
     * @param left Left operand.
     * @param right Right operand.
     * @return New matrix containing the sum.
     */
    friend SquareMat operator+(const SquareMat& left, const SquareMat& right);

    /**
     * @brief Subtracts one matrix from another (element-wise).
     * @param left Left operand.
     * @param right Right operand.
     * @return New matrix containing the difference.
     */
    friend SquareMat operator-(const SquareMat& left, const SquareMat& right);

    /**
     * @brief Multiplies two matrices (matrix product).
     * @param left Left operand.
     * @param right Right operand.
     * @return New matrix containing the product.
     */
    friend SquareMat operator*(const SquareMat& left, const SquareMat& right);

    /**
     * @brief Multiplies each element by a scalar.
     * @param mat Matrix operand.
     * @param scalar Scalar operand.
     * @return New matrix with elements scaled.
     */
    friend SquareMat operator*(const SquareMat& mat, double scalar);

    /**
     * @brief Multiplies each element by a scalar (scalar on left).
     * @param scalar Scalar operand.
     * @param mat Matrix operand.
     * @return New matrix with elements scaled.
     */
    friend SquareMat operator*(double scalar, const SquareMat& mat);

    /**
     * @brief Divides each element by a scalar.
     * @param mat Matrix operand.
     * @param scalar Scalar divisor.
     * @return New matrix with elements divided.
     */
    friend SquareMat operator/(const SquareMat& mat, double scalar);

    /**
     * @brief Element-wise multiplication (Hadamard product).
     * @param left Left operand.
     * @param right Right operand.
     * @return New matrix with element-wise products.
     */
    friend SquareMat operator%(const SquareMat& left, const SquareMat& right);

    /**
     * @brief Element-wise modulo operation (fmod) with a scalar.
     * @param mat Matrix operand.
     * @param scalar Scalar operand.
     * @return New matrix with elements modulo scalar.
     */
    friend SquareMat operator%(const SquareMat& mat, int scalar);

    /**
     * @brief Returns the transpose of the matrix.
     * @param mat Matrix to transpose.
     * @return Transposed matrix.
     */
    friend SquareMat operator~(const SquareMat& mat);

    /**
     * @brief Outputs the matrix to an output stream, formatted as rows of elements.
     * @param stream Output stream.
     * @param mat Matrix to output.
     * @return Reference to the output stream.
     */
    friend std::ostream& operator<<(std::ostream& stream, const SquareMat& mat);
};

}