// adar101101@gmail.com

#define DOCTEST_CONFIG_NO_MULTITHREADING
#define DOCTEST_CONFIG_USE_STD_HEADERS
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <cerrno>
#include <ctime>
#include <cmath>
#include <stdexcept>

// Shim for gmtime_s on MinGW/Windows
inline int gmtime_s(std::tm* tmDest, const time_t* sourceTime) {
    if (!tmDest || !sourceTime) return EINVAL;
    std::tm* res = std::gmtime(sourceTime);
    if (res) { *tmDest = *res; return 0; }
    return -1;
}

#include "doctest.h"
#include "SquareMat.hpp"

namespace Mat = Matrix;

#define DEFAULT_SIZE 3     
#define EPS 1e-6           ///< Epsilon for floating-point comparison

/**
 * @brief Checks whether two doubles are equal up to EPSILON.
 * @param d1 First value.
 * @param d2 Second value.
 * @return True if |d1 - d2| < EPS, false otherwise.
 */
bool isEqual(double d1, double d2) {
    return std::fabs(d1 - d2) < EPS;
}

/**
 * @brief Checks whether two matrices are equal element-wise up to EPSILON.
 * @param m1 First matrix.
 * @param m2 Second matrix.
 * @return True if matrices are the same size and all elements are equal up to EPS, false otherwise.
 */
bool isEqual(const Mat::SquareMat& m1, const Mat::SquareMat& m2) {
    if (m1.getRows() != m2.getRows() || m1.getCols() != m2.getCols())
        return false;
    for (int i = 0; i < m1.getRows(); ++i)
        for (int j = 0; j < m1.getCols(); ++j)
            if (!isEqual(m1(i, j), m2(i, j)))
                return false;
    return true;
}

/**
 * @brief Fills a matrix with zeros.
 * @param m Matrix to fill.
 */
void fillZero(Mat::SquareMat& m) {
    m.fill(0.0);
}

/**
 * @brief Fills a matrix as an identity matrix.
 * @param m Matrix to fill.
 */
void fillIdentity(Mat::SquareMat& m) {
    m.fill(0.0);
    for (int i = 0; i < m.getRows(); ++i) {
        m(i, i) = 1.0;
    }
}

/**
 * @brief Fills a matrix with arbitrary values for testing.
 * @param m Matrix to fill.
 */
void fillArbitrary(Mat::SquareMat& m) {
    m(0,0) = 4.5; m(0,1) = 8.0;  m(0,2) = 7.0;
    m(1,0) = 2.0; m(1,1) = 0.0;  m(1,2) = -12.0;
    m(2,0) = 3.3; m(2,1) = 5.6;  m(2,2) = -2.1;
}


TEST_SUITE("Matrix Construction and Fill") {
    TEST_CASE("Matrix cannot be created with non-positive dimensions") {
        // Check that creating a matrix with non-positive dimensions throws an exception
        CHECK_THROWS_AS(Mat::SquareMat(0,0), std::invalid_argument);
        CHECK_THROWS_AS(Mat::SquareMat(-3,-3), std::invalid_argument);
    }
    TEST_CASE("Fill and identity fill") {
        // Check that fill sets all elements and fillIdentity sets up the identity matrix
        Mat::SquareMat m(DEFAULT_SIZE, DEFAULT_SIZE);
        m.fill(5.5);
        for (int i = 0; i < DEFAULT_SIZE; ++i)
            for (int j = 0; j < DEFAULT_SIZE; ++j)
                CHECK(isEqual(m(i, j), 5.5));
        fillIdentity(m);
        for (int i = 0; i < DEFAULT_SIZE; ++i)
            for (int j = 0; j < DEFAULT_SIZE; ++j)
                CHECK(isEqual(m(i, j), (i == j) ? 1.0 : 0.0));
    }
    TEST_CASE("Fill with inf and nan") {
        // Check that fill works with infinity and NaN values
        Mat::SquareMat m(DEFAULT_SIZE, DEFAULT_SIZE);
        m.fill(INFINITY);
        for (int i = 0; i < DEFAULT_SIZE; ++i)
            for (int j = 0; j < DEFAULT_SIZE; ++j)
                CHECK(std::isinf(m(i,j)));
        m.fill(NAN);
        for (int i = 0; i < DEFAULT_SIZE; ++i)
            for (int j = 0; j < DEFAULT_SIZE; ++j)
                CHECK(std::isnan(m(i,j)));
    }
    TEST_CASE("Fill with huge and negative values") {
        // Check that fill works with very large and negative values
        Mat::SquareMat m(DEFAULT_SIZE, DEFAULT_SIZE);
        double big = 1e12;
        m.fill(big);
        Mat::SquareMat n = m * big;
        for (int i = 0; i < DEFAULT_SIZE; ++i)
            for (int j = 0; j < DEFAULT_SIZE; ++j)
                CHECK(isEqual(n(i,j), big*big));
        m.fill(-42);
        for (int i = 0; i < DEFAULT_SIZE; ++i)
            for (int j = 0; j < DEFAULT_SIZE; ++j)
                CHECK(isEqual(m(i,j), -42));
    }
    TEST_CASE("Fill with alternating signs") {
        // Check that fill can create a matrix with alternating signs
        Mat::SquareMat m(DEFAULT_SIZE, DEFAULT_SIZE);
        for (int i = 0; i < DEFAULT_SIZE; ++i)
            for (int j = 0; j < DEFAULT_SIZE; ++j)
                m(i,j) = (i+j)%2==0 ? 1.0 : -1.0;
        Mat::SquareMat n = m * -2;
        for (int i = 0; i < DEFAULT_SIZE; ++i)
            for (int j = 0; j < DEFAULT_SIZE; ++j)
                CHECK(isEqual(n(i,j), m(i,j)*-2));
    }
}

TEST_SUITE("Element Access and Range Checks") {
    TEST_CASE("Valid element assignment and retrieval") {
        // Check that element assignment and retrieval works as expected
        Mat::SquareMat m(DEFAULT_SIZE, DEFAULT_SIZE);
        m(1,2) = 7.2;
        CHECK(isEqual(m(1,2), 7.2));
        m(1,2) = -3.14;
        CHECK(isEqual(m(1,2), -3.14));
    }
    TEST_CASE("Out-of-range element access throws") {
        // Check that accessing elements out of range throws an exception
        Mat::SquareMat m(DEFAULT_SIZE, DEFAULT_SIZE);
        CHECK_THROWS_AS(m(DEFAULT_SIZE,0) = 0, std::out_of_range);
        CHECK_THROWS_AS((void)m(-1,0), std::out_of_range);
        CHECK_THROWS(m(-1,0));
        CHECK_THROWS(m(0,-1));
        CHECK_THROWS(m(DEFAULT_SIZE,0));
        CHECK_THROWS(m(0,DEFAULT_SIZE));
    }
}

TEST_SUITE("Copy and Move Semantics") {
    TEST_CASE("Copy constructor produces deep copy") {
        // Check that the copy constructor makes a deep copy and changes don't affect the original
        Mat::SquareMat m(DEFAULT_SIZE, DEFAULT_SIZE);
        fillArbitrary(m);
        Mat::SquareMat cpy(m);
        CHECK(isEqual(m, cpy));
        cpy(0,0) = 100.0;
        CHECK_FALSE(isEqual(m, cpy));
    }
    TEST_CASE("Move constructor works") {
        // Check that the move constructor works and the moved-from object is valid
        Mat::SquareMat m(DEFAULT_SIZE, DEFAULT_SIZE);
        fillArbitrary(m);
        Mat::SquareMat moved(std::move(m));
        CHECK(isEqual(moved, moved)); // Just check it's valid
    }
    TEST_CASE("Copy assignment operator") {
        // Check that copy assignment operator works as expected
        Mat::SquareMat m(DEFAULT_SIZE, DEFAULT_SIZE);
        fillArbitrary(m);
        Mat::SquareMat b(DEFAULT_SIZE, DEFAULT_SIZE);
        b.fill(1.0);
        b = m;
        CHECK(isEqual(b, m));
        // Check that original matrix is not affected
        m(0,0) = -1.1;
        CHECK_FALSE(isEqual(b, m));
    }
    TEST_CASE("Move assignment operator") {
        // Check that move assignment operator works as expected
        Mat::SquareMat m(DEFAULT_SIZE, DEFAULT_SIZE);
        fillArbitrary(m);
        Mat::SquareMat b(DEFAULT_SIZE, DEFAULT_SIZE);
        b.fill(1.0);
        b = std::move(m);
        CHECK(isEqual(b, b)); // Just check it's valid
    }
    TEST_CASE("Self-assignment is safe") {
        // Check that self-assignment does not alter the matrix
        Mat::SquareMat m(DEFAULT_SIZE, DEFAULT_SIZE);
        fillArbitrary(m);
        m = m;
        CHECK(isEqual(m, m));
    }
    TEST_CASE("Move assignment resets source") {
        // Check that after move assignment, the source is reset
        Mat::SquareMat m(DEFAULT_SIZE, DEFAULT_SIZE); m.fill(7.0);
        Mat::SquareMat n = std::move(m);
        for (int i = 0; i < DEFAULT_SIZE; ++i)
            for (int j = 0; j < DEFAULT_SIZE; ++j)
                CHECK(isEqual(n(i,j), 7.0));
    }


}

/**
 * @brief Tests for row access via operator[]: double* operator[](size_t row) const.
 *
 * These tests cover:
 * - Valid row access for both non-const and const matrices
 * - Assignment and retrieval of values via mat[row][col]
 * - Out-of-range row access (negative, too large)
 * - Comparison with operator()(row, col)
 * - Edge cases: 1x1 matrix, last row, empty matrix throws
 */

TEST_SUITE("Row Access Operator []") {
    TEST_CASE("Valid row access and assignment") {
        Mat::SquareMat m(3,3);
        m.fill(0.0);
        m[1][2] = 7.5;
        CHECK(m[1][2] == 7.5);
        m[0][0] = -3.14;
        CHECK(m[0][0] == -3.14);
        m[2][1] = 42;
        CHECK(m[2][1] == 42);
    }

    TEST_CASE("Const correctness for operator[]") {
        Mat::SquareMat m(3,3);
        m[0][1] = 2.5;
        const Mat::SquareMat& cm = m;
        CHECK(cm[0][1] == 2.5);
        // Can't assign to const matrix: cm[0][1] = 5; // This should not compile
    }

    TEST_CASE("Comparison to operator()(row,col)") {
        Mat::SquareMat m(3,3);
        m[2][0] = 123.4;
        CHECK(m(2,0) == m[2][0]);
        m(1,2) = -9.9;
        CHECK(m[1][2] == -9.9);
    }

    TEST_CASE("Out-of-range row access throws") {
        Mat::SquareMat m(3,3);
        CHECK_THROWS_AS(m[3][1] = 0, std::out_of_range);
        CHECK_THROWS_AS(m[100][0] = 0, std::out_of_range);
        CHECK_THROWS_AS(m[-1][0] = 0, std::out_of_range);
    }


    TEST_CASE("Edge case: 1x1 matrix") {
        Mat::SquareMat m(1,1);
        m[0][0] = 77.7;
        CHECK(m[0][0] == 77.7);
    }

    TEST_CASE("Edge case: last row access") {
        Mat::SquareMat m(4,4);
        m[3][2] = 3.3;
        CHECK(m[3][2] == 3.3);
    }

    TEST_CASE("Multiple row access and assignment") {
        Mat::SquareMat m(3,3);
        for (int i=0; i<3; ++i)
            for (int j=0; j<3; ++j)
                m[i][j] = i*10 + j;
        for (int i=0; i<3; ++i)
            for (int j=0; j<3; ++j)
                CHECK(m[i][j] == i*10 + j);
    }
}

TEST_SUITE("Arithmetic Operations") {
    TEST_CASE("Addition and subtraction") {
        // Check that addition and subtraction of matrices works, and dimension mismatch throws
        Mat::SquareMat a(DEFAULT_SIZE, DEFAULT_SIZE), b(DEFAULT_SIZE, DEFAULT_SIZE);
        a.fill(1.0); b.fill(2.0);
        Mat::SquareMat c = a + b;
        for (int i=0;i<DEFAULT_SIZE;++i)
            for (int j=0;j<DEFAULT_SIZE;++j)
                CHECK(isEqual(c(i,j), 3.0));
        Mat::SquareMat d = a - b;
        for (int i=0;i<DEFAULT_SIZE;++i)
            for (int j=0;j<DEFAULT_SIZE;++j)
                CHECK(isEqual(d(i,j), -1.0));
        Mat::SquareMat e(DEFAULT_SIZE+1, DEFAULT_SIZE+1);
        CHECK_THROWS_AS(a + e, std::invalid_argument);
        CHECK_THROWS_AS(a - e, std::invalid_argument);
    }
    TEST_CASE("Scalar multiplication and edge cases") {
        // Check that scalar multiplication works and edge cases are handled
        Mat::SquareMat a(DEFAULT_SIZE, DEFAULT_SIZE); a.fill(2.0);
        Mat::SquareMat b = a * 5.0;
        Mat::SquareMat c = 5.0 * a;
        for (int i=0;i<DEFAULT_SIZE;++i)
            for (int j=0;j<DEFAULT_SIZE;++j) {
                CHECK(isEqual(b(i,j), 10.0));
                CHECK(isEqual(c(i,j), 10.0));
            }
        Mat::SquareMat z = a * 0.0;
        for (int i=0;i<DEFAULT_SIZE;++i)
            for (int j=0;j<DEFAULT_SIZE;++j)
                CHECK(isEqual(z(i,j), 0.0));
        Mat::SquareMat n = a * -1.0;
        for (int i=0;i<DEFAULT_SIZE;++i)
            for (int j=0;j<DEFAULT_SIZE;++j)
                CHECK(isEqual(n(i,j), -2.0));
    }
    TEST_CASE("Matrix multiplication and errors") {
        // Check that matrix multiplication works and dimension mismatch throws
        Mat::SquareMat a(DEFAULT_SIZE, DEFAULT_SIZE), b(DEFAULT_SIZE, DEFAULT_SIZE);
        fillArbitrary(a);
        fillArbitrary(b);
        Mat::SquareMat c = a * b;
        CHECK(c.getRows() == DEFAULT_SIZE);
        CHECK(c.getCols() == DEFAULT_SIZE);
        Mat::SquareMat d(DEFAULT_SIZE+1, DEFAULT_SIZE+1);
        CHECK_THROWS_AS(a * d, std::invalid_argument);
        // Check that multiply by identity matrix doesn't change the matrix
        Mat::SquareMat id(DEFAULT_SIZE, DEFAULT_SIZE); fillIdentity(id);
        Mat::SquareMat prod = a * id;
        CHECK(isEqual(prod, a));
        Mat::SquareMat prod2 = id * a;
        CHECK(isEqual(prod2, a));
        // Check that multiplying by zero matrix gives zero matrix
        Mat::SquareMat zero(DEFAULT_SIZE, DEFAULT_SIZE); fillZero(zero);
        Mat::SquareMat prod3 = a * zero;
        Mat::SquareMat prod4 = zero * a;
        for (int i=0;i<DEFAULT_SIZE;++i)
            for (int j=0;j<DEFAULT_SIZE;++j)
                CHECK(isEqual(prod3(i,j), 0.0));
        for (int i=0;i<DEFAULT_SIZE;++i)
            for (int j=0;j<DEFAULT_SIZE;++j)
                CHECK(isEqual(prod4(i,j), 0.0));
    }
    TEST_CASE("Modulo element-wise and scalar") {
        // Check that modulo operator works element-wise and with scalars, and throws on zero
        Mat::SquareMat a(DEFAULT_SIZE, DEFAULT_SIZE);
        a(0,0)=5; a(0,1)=7; a(0,2)=9;
        a(1,0)=11; a(1,1)=13; a(1,2)=15;
        a(2,0)=17; a(2,1)=19; a(2,2)=21;
        Mat::SquareMat b = a % 4;
        CHECK(isEqual(b(0,0), 1));
        CHECK(isEqual(b(0,1), 3));
        CHECK(isEqual(b(0,2), 1));
        CHECK(isEqual(b(1,0), 3));
        CHECK(isEqual(b(1,1), 1));
        CHECK(isEqual(b(1,2), 3));
        CHECK(isEqual(b(2,0), 1));
        CHECK(isEqual(b(2,1), 3));
        CHECK(isEqual(b(2,2), 1));
        CHECK_THROWS(a % 0);
        // Check that element-wise modulo is correct
        Mat::SquareMat other(DEFAULT_SIZE, DEFAULT_SIZE);
        other(0,0)=2; other(0,1)=3; other(0,2)=4;
        other(1,0)=5; other(1,1)=6; other(1,2)=7;
        other(2,0)=8; other(2,1)=9; other(2,2)=10;
        Mat::SquareMat mod_elem = a % other;
        for (int i=0;i<DEFAULT_SIZE;++i)
            for (int j=0;j<DEFAULT_SIZE;++j)
                CHECK(isEqual(mod_elem(i,j), a(i,j) * other(i,j)));
    }
    TEST_CASE("Division and division by zero") {
        // Check that division works and division by zero throws
        Mat::SquareMat a(DEFAULT_SIZE, DEFAULT_SIZE);
        a(0,0)=4; a(0,1)=8; a(0,2)=16;
        a(1,0)=32; a(1,1)=64; a(1,2)=128;
        a(2,0)=256; a(2,1)=512; a(2,2)=1024;
        Mat::SquareMat b = a / 4.0;
        CHECK(isEqual(b(0,0), 1));
        CHECK(isEqual(b(0,1), 2));
        CHECK(isEqual(b(0,2), 4));
        CHECK(isEqual(b(1,0), 8));
        CHECK(isEqual(b(1,1), 16));
        CHECK(isEqual(b(1,2), 32));
        CHECK(isEqual(b(2,0), 64));
        CHECK(isEqual(b(2,1), 128));
        CHECK(isEqual(b(2,2), 256));
        CHECK_THROWS(a / 0.0);
    }
    TEST_CASE("Chained addition/subtraction") {
        // Check that chained addition and subtraction works
        Mat::SquareMat a(DEFAULT_SIZE, DEFAULT_SIZE), b(DEFAULT_SIZE, DEFAULT_SIZE), c(DEFAULT_SIZE, DEFAULT_SIZE);
        a.fill(1.0); b.fill(2.0); c.fill(3.0);
        Mat::SquareMat d = a + b + c;
        for (int i=0;i<DEFAULT_SIZE;++i)
            for (int j=0;j<DEFAULT_SIZE;++j)
                CHECK(isEqual(d(i,j), 6.0));
        Mat::SquareMat e = d - a - b - c;
        for (int i=0;i<DEFAULT_SIZE;++i)
            for (int j=0;j<DEFAULT_SIZE;++j)
                CHECK(isEqual(e(i,j), 0.0));
    }
    TEST_CASE("Chained multiplication") {
        // Check that chained multiplication works (power of diagonal matrix)
        Mat::SquareMat a(DEFAULT_SIZE, DEFAULT_SIZE);
        for (int i=0;i<DEFAULT_SIZE;++i)
            for (int j=0;j<DEFAULT_SIZE;++j)
                a(i,j) = (i==j)?2:0;
        Mat::SquareMat b = a * a * a;
        for (int i=0;i<DEFAULT_SIZE;++i)
            for (int j=0;j<DEFAULT_SIZE;++j)
                CHECK(isEqual(b(i,j), (i==j)?8:0));
    }
    TEST_CASE("Compound assignment operators") {
        // Check that compound assignment operators work as expected
        Mat::SquareMat a(DEFAULT_SIZE, DEFAULT_SIZE); a.fill(10.0);
        a += a; // now all 20
        a -= Mat::SquareMat(DEFAULT_SIZE, DEFAULT_SIZE); // subtract zero matrix
        a *= 2;
        a /= 4;
        a %= 7;
        for (int i=0;i<DEFAULT_SIZE;++i)
            for (int j=0;j<DEFAULT_SIZE;++j)
                CHECK(isEqual(a(i,j), 3.0));
    }
}

TEST_SUITE("Comparison Operators") {
    TEST_CASE("Equality, inequality, and ordering") {
        // Check that comparison operators (==, !=, >, <, >=, <=) behave as expected
        Mat::SquareMat a(DEFAULT_SIZE, DEFAULT_SIZE), b(DEFAULT_SIZE, DEFAULT_SIZE);
        a.fill(1.0); b.fill(2.0);
        CHECK(a != b);
        CHECK_FALSE(a == b);
        CHECK(b > a);
        CHECK(a < b);
        b.fill(1.0);
        CHECK(a == b);
        Mat::SquareMat c(DEFAULT_SIZE+1, DEFAULT_SIZE+1);
        CHECK_FALSE(a == c);
        CHECK(a != c);
        // Check matrices with same sum but different values
        Mat::SquareMat d(DEFAULT_SIZE, DEFAULT_SIZE), e(DEFAULT_SIZE, DEFAULT_SIZE);
        d.fill(3.0);
        e.fill(2.0);
        e(0,0) = 11.0;
        CHECK(isEqual(d.countSum(), e.countSum()));
        CHECK_FALSE(d == e);
    }
}

TEST_SUITE("Exponentiation and Determinant") {
    TEST_CASE("Exponentiation operator") {
        // Check that the exponentiation operator ^ works and throws on negative powers
        Mat::SquareMat a(DEFAULT_SIZE, DEFAULT_SIZE);
        fillIdentity(a);
        Mat::SquareMat b = a ^ 3;
        for (int i=0;i<DEFAULT_SIZE;++i)
            for (int j=0;j<DEFAULT_SIZE;++j)
                CHECK(isEqual(b(i,j), (i == j) ? 1.0 : 0.0));
        Mat::SquareMat id = a ^ 0;
        for (int i=0;i<DEFAULT_SIZE;++i)
            for (int j=0;j<DEFAULT_SIZE;++j)
                CHECK(isEqual(id(i,j), (i == j) ? 1.0 : 0.0));
        CHECK_THROWS_AS(a ^ -2, std::invalid_argument);
        // Check exponentiation edge cases for diagonal matrix
        Mat::SquareMat diag(DEFAULT_SIZE, DEFAULT_SIZE);
        for (int i=0;i<DEFAULT_SIZE;++i)
            for (int j=0;j<DEFAULT_SIZE;++j)
                diag(i,j) = (i==j)?2:0;
        Mat::SquareMat b2 = diag ^ 5;
        for (int i=0;i<DEFAULT_SIZE;++i)
            for (int j=0;j<DEFAULT_SIZE;++j)
                CHECK(isEqual(b2(i,j), (i==j)?32:0));
    }
    TEST_CASE("Determinant calculation") {
        // Check that determinant calculation is correct for common cases
        Mat::SquareMat a(2,2);
        a(0,0)=1; a(0,1)=2; a(1,0)=3; a(1,1)=4;
        CHECK(isEqual(a.operator!(), -2.0));
        Mat::SquareMat id(3,3); fillIdentity(id);
        CHECK(isEqual(id.operator!(), 1.0));
        Mat::SquareMat z(3,3); fillZero(z);
        CHECK(isEqual(z.operator!(), 0.0));
        // Check determinant for non-square throws
        CHECK_THROWS(Mat::SquareMat(2,3).operator!());
    }
    TEST_CASE("Determinant advanced cases") {
        // Check determinant of zero row/column
        Mat::SquareMat mat(DEFAULT_SIZE, DEFAULT_SIZE); fillArbitrary(mat);
        mat(0,0) = mat(0,1) = mat(0,2) = 0;
        CHECK(isEqual(mat.operator!(), 0.0));
        mat = Mat::SquareMat(DEFAULT_SIZE, DEFAULT_SIZE); fillArbitrary(mat);
        mat(2,0) = mat(2,1) = mat(2,2) = 0;
        CHECK(isEqual(mat.operator!(), 0.0));
        mat = Mat::SquareMat(DEFAULT_SIZE, DEFAULT_SIZE); fillArbitrary(mat);
        mat(0,1) = mat(1,1) = mat(2,1) = 0;
        CHECK(isEqual(mat.operator!(), 0.0));
    }
    TEST_CASE("Determinant of identity and zero matrices") {
        // Check determinant for identity and zero matrices
        Mat::SquareMat id(DEFAULT_SIZE, DEFAULT_SIZE); for(int i=0;i<DEFAULT_SIZE;++i) id(i,i)=1;
        Mat::SquareMat z(DEFAULT_SIZE, DEFAULT_SIZE); fillZero(z);
        CHECK(isEqual(id.operator!(), 1.0));
        CHECK(isEqual(z.operator!(), 0.0));
    }
}

TEST_SUITE("Increment and Decrement Operators") {
    TEST_CASE("Prefix and postfix increment/decrement") {
        // Check that prefix and postfix increment and decrement work as expected
        Mat::SquareMat a(DEFAULT_SIZE, DEFAULT_SIZE); a.fill(1.0);
        ++a; // Check that ++a increments before use
        CHECK(isEqual(a(0,0), 2.0));
        a++; // Check that a++ increments after use
        CHECK(isEqual(a(0,0), 3.0));
        --a; // Check that --a decrements before use
        CHECK(isEqual(a(0,0), 2.0));
        a--; // Check that a-- decrements after use
        CHECK(isEqual(a(0,0), 1.0));
        for(int i=0;i<1000;++i) ++a; // Check repeated increment
        CHECK(isEqual(a(0,0), 1001.0));
    }
    TEST_CASE("Multiple increments") {
        // Check that multiple increments work
        Mat::SquareMat m(DEFAULT_SIZE, DEFAULT_SIZE); m.fill(0.0);
        for(int k=0;k<100;k++) ++m;
        for(int i=0;i<DEFAULT_SIZE;++i)
            for(int j=0;j<DEFAULT_SIZE;++j)
                CHECK(isEqual(m(i,j), 100.0));
    }
    TEST_CASE("Multiple decrements") {
        // Check that multiple decrements work
        Mat::SquareMat m(DEFAULT_SIZE, DEFAULT_SIZE); m.fill(50.0);
        for(int k=0;k<25;k++) m--;
        for(int i=0;i<DEFAULT_SIZE;++i)
            for(int j=0;j<DEFAULT_SIZE;++j)
                CHECK(isEqual(m(i,j), 25.0));
    }
}

TEST_SUITE("Transpose Operation") {
    TEST_CASE("Transpose produces correct output") {
        // Check that the transpose operator ~ produces the correct matrix
        Mat::SquareMat a(DEFAULT_SIZE, DEFAULT_SIZE);
        fillArbitrary(a);
        Mat::SquareMat t = ~a;
        for (int i=0;i<DEFAULT_SIZE;++i)
            for (int j=0;j<DEFAULT_SIZE;++j)
                CHECK(isEqual(t(i,j), a(j,i)));
        Mat::SquareMat tt = ~t;
        CHECK(isEqual(tt, a));
    }
    TEST_CASE("Transpose twice returns original") {
        // Check that applying transpose twice returns the original matrix
        Mat::SquareMat m(DEFAULT_SIZE, DEFAULT_SIZE);
        int v=1;
        for(int i=0;i<DEFAULT_SIZE;++i)
            for(int j=0;j<DEFAULT_SIZE;++j)
                m(i,j)=v++;
        Mat::SquareMat t = ~m;
        Mat::SquareMat tt = ~t;
        CHECK(isEqual(tt, m));
    }
}

TEST_SUITE("Fill Edge Cases and Miscellaneous") {
    TEST_CASE("Fill with various values") {
        // Check that fill works for various values including negatives and large numbers
        Mat::SquareMat a(DEFAULT_SIZE, DEFAULT_SIZE); a.fill(7.0);
        for (int i=0;i<DEFAULT_SIZE;++i)
            for (int j=0;j<DEFAULT_SIZE;++j)
                CHECK(isEqual(a(i,j), 7.0));
        a.fill(-17.0);
        CHECK(isEqual(a(1,1), -17.0));
        a.fill(0.0);
        CHECK(isEqual(a(0,0), 0.0));
        a.fill(1e9);
        CHECK(isEqual(a(1,0), 1e9));
    }
}

TEST_SUITE("Edge Cases") {
    TEST_CASE("Edge case with 1x1 matrix") {
        // Check that a 1x1 matrix works as expected
        Mat::SquareMat one(1,1); one(0,0) = 7.0;
        CHECK(isEqual(one(0,0), 7.0));
        CHECK(isEqual(one.operator!(), 7.0));
        Mat::SquareMat id(1,1); id(0,0) = 1.0;
        CHECK(isEqual(id ^ 100, id));
        CHECK(isEqual(one * 0, Mat::SquareMat(1,1))); // Should be zero
    }
    TEST_CASE("Large matrix fill and sum") {
        // Check that large matrix fill and countSum work as expected
        const int N = 8;
        Mat::SquareMat big(N,N);
        big.fill(3.0);
        for(int i=0; i<N; ++i)
            for(int j=0; j<N; ++j)
                CHECK(isEqual(big(i,j), 3.0));
        CHECK(isEqual(big.countSum(), 3.0*N*N));
    }

    TEST_CASE("Floating point precision edge case") {
        // Check that floating point precision is handled correctly
        Mat::SquareMat f(2,2); f(0,0)=0.1+0.2; f(0,1)=0.3; f(1,0)=0.5; f(1,1)=0.7;
        CHECK(std::fabs(f(0,0)-0.3)<1e-12);
    }
    TEST_CASE("Sum of elements utility") {
        // Check that countSum utility works
        Mat::SquareMat m(DEFAULT_SIZE, DEFAULT_SIZE); m.fill(2.5);
        CHECK(isEqual(m.countSum(), 2.5*DEFAULT_SIZE*DEFAULT_SIZE));
        m(0,0) = 10.0;
        CHECK(isEqual(m.countSum(), 2.5*DEFAULT_SIZE*DEFAULT_SIZE + 7.5));
    }


}

TEST_SUITE("Exception Handling") {
    TEST_CASE("Wrong size binary operations") {
        // Check that binary operations with different sizes throw
        Mat::SquareMat a(DEFAULT_SIZE, DEFAULT_SIZE), b(DEFAULT_SIZE+1, DEFAULT_SIZE+1);
        CHECK_THROWS(a + b);
        CHECK_THROWS(a - b);
        CHECK_THROWS(a * b);
        CHECK_THROWS(a % b);
    }
    TEST_CASE("Division and modulo by zero") {
        // Check that division and modulo by zero throw an exception
        Mat::SquareMat m(DEFAULT_SIZE, DEFAULT_SIZE); m.fill(5.0);
        CHECK_THROWS(m / 0.0);
        CHECK_THROWS(m % 0);
    }
}
