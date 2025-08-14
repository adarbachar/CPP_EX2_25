**This project was carried out as part of a bachelor’s degree in Computer and Software Engineering at Ariel University.** The goal is to implement a robust `SquareMat` class in C++, supporting a full suite of matrix operations, complete with value-based arithmetic, in-place modifiers, and advanced utilities like exponentiation and determinant calculation.

## Overview

The assignment focuses on a **square** matrix class (`SquareMat`) and the following features:

- **Construction**: size checks, copy/move semantics, and proper memory management  
- **Element access**: bounds-checked `operator()(row,col)`  
- **Arithmetic operators**: `+`, `-`, `*` (scalar, matrix, element-wise), `/` (scalar), `%` (element-wise and scalar)  
- **Compound assignment**: `+=, -=, *=, /=, %=`  
- **Increment/decrement**: prefix and postfix `++`, `--`  
- **Comparison**: `==, !=, >, >=, <, <=` based on sum of elements  
- **Utilities**:  
  - `fill(value)` to set all entries  
  - `operator~` for transpose  
  - `operator^` for exponentiation by nonnegative integer  
  - `operator!` (and helper) for determinant via cofactor expansion  

Each operation throws `std::invalid_argument` or `std::out_of_range` on misuse.

## Project Structure

```
SquareMatrix/
├─ src/
│  ├─ SquareMat.hpp
│  ├─ SquareMat.cpp
│  ├─ main.cpp
│  ├─ SquareMatTest.cpp
│  ├─ Makefile
│  └─ doctest.h
├─ README.md
└─ submission.txt
```

## File Descriptions

### `SquareMat.hpp`

Declares the `Matrix::SquareMat` class:

- Constructors & destructor (including copy/move)  
- `operator=` overloads  
- In-place modifiers: `+=, -=, *=, /=, %=`  
- Increment/decrement: `operator++/--` (prefix/postfix)  
- Element access: `operator()(int row, int col)` (throws on OOB)  
- Comparison operators: `==, !=, >, >=, <, <=`  
- Utilities: `getRows()`, `getCols()`, `fill(double)`  
- Declarations of non-member overloads: `+, -, *, /, %, ~, ^, !`

### `SquareMat.cpp`

Implements every method declared in the header:

- Allocation of a 2D `double` array and cleanup  
- Copy and move logic for efficient ownership transfer  
- Bounds checking on element access  
- Full definitions of all arithmetic, compound, comparison, and utility operators, including determinant recursion and fast exponentiation

### `main.cpp`

A simple demo program:

- Constructs two 3×3 matrices  
- Demonstrates all operators: addition, subtraction, scalar/matrix multiplication, element-wise operations, transpose, exponentiation, determinant, comparisons, and `fill`  
- Prints results both before and after modifications

### `SquareMatTest.cpp`

Unit tests using [doctest](https://github.com/onqtam/doctest) that cover:

- Constructor validation and default fill  
- Element access and range-check exceptions  
- All arithmetic and compound operators, including mismatch errors  
- Scalar multiplication/division edge cases (zero, negative)  
- Matrix multiplication, element-wise operations, modulo, and error cases  
- Increment/decrement chains and overflow behavior  
- Transpose, exponentiation, and determinant for 1×1, 2×2, and larger matrices  
- Copy/move constructors and assignment, including self-assignment  
- Comparison operators based on sum of entries

### `doctest.h`

Single-header testing framework, configured for single-threaded tests.

## Build and Run

**Prerequisites:**

- **Compiler:** `g++` (C++17 or newer)  
- **Build tool:** `make`  

**Typical Workflow:**

```bash
$ make Main      # Compile and execute the demo (./Main)
$ make test      # Build and run all unit tests
$ make valgrind  # Run tests under Valgrind (zero leaks expected)
$ make clean     # Remove build artifacts
```

## Testing & Memory Management

- **Unit Tests:** Thorough coverage of all matrix operations, located in `SquareMatTest.cpp`.  
- **Valgrind:** Should report zero leaks, verifying that dynamic allocations and deallocations in `SquareMat` are correctly balanced.  
