// adar101101@gmail.com

#include <iostream>
#include "SquareMat.hpp"
namespace st = std;
namespace Mat = Matrix;

int main() {
    // Construct two 3x3 matrices
    Mat::SquareMat A(3,3);
    Mat::SquareMat B(3,3);

    // Fill A with 1s, B with a pattern
    A.fill(1.0);
    B.fill(0.0);
    B(0,0) = 1; B(1,1) = 2; B(2,2) = 3;
    B(0,1) = 4; B(1,2) = 5; B(2,0) = 6;

    st::cout << "\nMatrix A:\n" << A;
    st::cout << "\nMatrix B:\n" << B;

    // Addition
    st::cout << "\nA + B:\n" << (A+B);

    // Subtraction
    st::cout << "\nA - B:\n" << (A-B);

    // Scalar multiplication
    st::cout << "\nA * 10:\n" << (A*10);
    st::cout << "\n10 * B:\n" << (10*B);

    A(0,0) = 3; A(1,1) = 1; A(2,2) = 2;
    A(0,1) = 5; A(1,2) = 4; A(2,0) = 0;

    st::cout << "\nMatrix A:\n" << A;
    st::cout << "\nMatrix B:\n" << B;

    // Matrix multiplication
    st::cout << "\nA * B:\n" << (A*B);

    // Element-wise multiplication and modulo
    st::cout << "\nA % B (element-wise):\n" << (A%B);
    st::cout << "\nB % 2 (modulo each element):\n" << (B%2);

    // Scalar division
    st::cout << "\nB / 2:\n" << (B/2.0);

    // Element access and assignment
    st::cout << "\nA(1,1) before: \n" << A(1,1) << st::endl;
    A(1,1) = 42;
    st::cout << "\nA(1,1) after: \n" << A(1,1) << st::endl;
    st::cout << "\nUpdated A: \n" << A;

    // Postfix and prefix increment/decrement
    ++A;
    st::cout << "\nAfter ++A:\n" << A;
    A++;
    st::cout << "\nAfter A++:\n" << A;
    --A;
    st::cout << "\nAfter --A:\n" << A;
    A--;
    st::cout << "\nAfter A--:\n" << A;

    // Transpose
    st::cout << "\nTranspose of B:\n" << (~B);

    // Exponentiation (power)
    st::cout << "\nB ^ 2:\n" << (B^2);

    // Determinant
    double detB = !B;
    st::cout << "\nDeterminant of B: \n" << detB << st::endl;

    // Comparison
    st::cout << "\nA == B: " << (A==B) << st::endl;
    st::cout << "\nA != B: " << (A!=B) << st::endl;
    st::cout << "\nA > B: " << (A>B) << st::endl;
    st::cout << "\nA < B: " << (A<B) << st::endl;
    st::cout << "\nA >= B: " << (A>=B) << st::endl;
    st::cout << "\nA <= B: " << (A<=B) << st::endl;

    // Logical NOT (determinant for your implementation)
    st::cout << "\n!A (calls determinant): \n" << (!A) << st::endl;

    // Fill
    A.fill(7.0);
    st::cout << "\nA after fill(7.0):\n" << A;

    return 0;
}