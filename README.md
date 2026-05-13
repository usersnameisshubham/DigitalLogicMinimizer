# Digital Logic Minimizer & Gate-Level Realizer

## Overview

This project is a C++ implementation of:

- Quine–McCluskey Algorithm
- Petrick’s Method
- NAND/NOR Gate-Level Realization

The program minimizes Boolean expressions and converts them into optimized logic gate implementations.

It supports both:

- SOP (Sum of Products)
- POS (Product of Sums)

forms along with don't-care conditions.

---

## Features

- Quine–McCluskey Boolean minimization
- Petrick’s Method implementation
- Essential Prime Implicant selection
- Support for don't-care conditions
- SOP and POS simplification
- Truth table generation
- NAND-only realization
- NOR-only realization
- Gate count estimation
- Interactive CLI interface

---

## Algorithms Used

### 1. Quine–McCluskey Algorithm

Used for systematic Boolean expression minimization by:

- Grouping minterms
- Combining terms differing by one bit
- Finding prime implicants
- Selecting essential prime implicants

---

### 2. Petrick’s Method

Used when multiple prime implicant combinations exist.

It helps find:
- Minimal cover solutions
- Optimized Boolean expressions

---

### 3. Gate-Level Realization

The minimized expressions are converted into:

- NAND-only circuits
- NOR-only circuits

which are universal gate implementations used in digital hardware design.

---

## Technologies Used

- C++
- STL
- Boolean Algebra
- Digital Logic Design
- Quine–McCluskey Method
- Petrick’s Method

---

## Project Structure

```bash
.
├── logic_minimizer.cpp
├── README.md
```

---

## Compilation

### Linux / macOS

```bash
g++ logic_minimizer.cpp -O2 -o logic
./logic
```

---

## Menu Options

```text
1. Quine-McCluskey (Heuristic Method)
2. Quine-McCluskey (Petrick's Method)
3. Gate-level realization (NAND/NOR only)
```

---

## Input Supported

### SOP Example

```text
F(A,B,C,D) = Σ(1,3,7,11,15)
```

### POS Example

```text
F(A,B,C,D) = Π(0,2,5,8)
```

### Don't-Care Terms

```text
d(4,6,9)
```

---

## Output Features

The program generates:

- Minimized Boolean expression
- Prime implicants
- Truth table verification
- NAND/NOR gate implementation
- Total gate count

---

## Example Features

### Boolean Minimization

Input:

```text
Minterms: 1 3 7 15
```

Output:

```text
A'B + CD
```

---

### Gate-Level Realization

Example:

```text
NAND(A,B) -> T1
NAND(T1,C) -> F
```

---

## Why This Project Is Useful

This project helps understand:

- Boolean function optimization
- Logic synthesis
- Hardware implementation
- Universal gates
- Digital circuit design

It is highly useful for:
- Digital Electronics courses
- Computer Organization
- VLSI fundamentals
- Logic Design labs

---

## Advantages

- Fully algorithmic minimization
- Handles large Boolean expressions
- Supports don't-care optimization
- Hardware-level realization support
- Educational and practical implementation

---

## Future Improvements

- GUI visualization
- Karnaugh Map visualization
- Verilog/VHDL code generation
- Circuit diagram generation
- Multi-output optimization
- Delay and power estimation

---

## Author

Developed as a Digital Logic Design and Boolean Optimization project to study logic minimization algorithms and gate-level synthesis.
