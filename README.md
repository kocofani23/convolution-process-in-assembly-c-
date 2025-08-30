2D Convolution on Arrays (Assembly Implementation)
📌 Project Description

This project implements two-dimensional convolution on given matrices using kernels in x86 Assembly, integrated with a C++ driver program. The goal is to simulate how convolution works on matrices by manually calculating the sum of element-wise multiplications between matrix sub-regions and kernels.

The convolution is carried out with:

Padding = 0 (no extra border added)

Stride = 1 (kernel moves one step at a time)

This ensures that the kernel slides over the matrix in the standard convolution way.

📂 Given Materials

2 matrices of different sizes (matrix1.txt, matrix2.txt).

2 kernels of different sizes (kernel1.txt, kernel2.txt).

A C++ source file (lab4final.cpp) with predefined variables and arrays for matrices, kernels, and outputs.

A Visual Studio project file (lab4final.vcxproj) to compile and run the program.

⚙️ Requirements

Visual Studio (x86 / 32-bit mode enabled)

Basic understanding of C++ and Assembly integration

Familiarity with row-major storage of 2D arrays in memory

🏗️ Implementation Details

Matrix and Kernel Representation

The matrix and kernel are stored as 1D arrays in memory.

Access formula:
matrix[i][j]  ->  matrix[i * m + j]
kernel[x][y]  ->  kernel[x * k + y]
where m is the matrix column size and k is the kernel size.

Assembly Block

Only the Assembly section in the provided C++ file should be completed.

You can access arrays like this:
MOV EBX, matrix       ; base address of matrix
MOV EAX, [EBX + 8]    ; read 3rd element (2 * 4 bytes offset)
Use 32-bit registers (EAX, EBX, ECX, EDX, etc.).

Convolution Algorithm

Position the kernel at the top-left of the matrix.

Multiply overlapping elements and sum them.

Store the result in the output array.

Slide the kernel one column at a time, then move to the next row
