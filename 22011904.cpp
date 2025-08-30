#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>

// Function to print a matrix stored in a 1D array
void print_matrix(unsigned* matrix, unsigned rows, unsigned cols, FILE* file);
// Function to read matrix from a file
void read_matrix(const char* filename, unsigned** matrix, unsigned* rows, unsigned* cols);
// Function to read kernel from a file
void read_kernel(const char* filename, unsigned** kernel, unsigned* k);
// Function to write output matrix to a file
void write_output(const char* filename, unsigned* output, unsigned rows, unsigned cols);
// Initialize output as zeros.
void initialize_output(unsigned*, unsigned, unsigned);

int main() {

    unsigned n, m, k;  // n = rows of matrix, m = cols of matrix, k = kernel size
    // Dynamically allocate memory for matrix, kernel, and output
    unsigned* matrix = NULL;  // Input matrix
    unsigned* kernel = NULL;  // Kernel size 3x3
    unsigned* output = NULL;  // Max size of output matrix

    char matrix_filename[30];
    char kernel_filename[30];

    // Read the file names
    printf("Enter matrix filename: ");
    scanf("%s", matrix_filename);
    printf("Enter kernel filename: ");
    scanf("%s", kernel_filename);


    // Read matrix and kernel from files
    read_matrix(matrix_filename, &matrix, &n, &m);  // Read matrix from file
    read_kernel(kernel_filename, &kernel, &k);      // Read kernel from file

    // For simplicity we say: padding = 0, stride = 1
    // With this setting we can calculate the output size
    unsigned output_rows = n - k + 1;
    unsigned output_cols = m - k + 1;
    output = (unsigned*)malloc(output_rows * output_cols * sizeof(unsigned));
    initialize_output(output, output_rows, output_cols);

    // Print the input matrix and kernel
    printf("Input Matrix: ");
    print_matrix(matrix, n, m, stdout);

    printf("\nKernel: ");
    print_matrix(kernel, k, k, stdout);

    /******************* KODUN BU KISMINDAN SONRASINDA DEĞİŞİKLİK YAPABİLİRSİNİZ - ÖNCEKİ KISIMLARI DEĞİŞTİRMEYİN *******************/

    // Assembly kod bloğu içinde kullanacağınız değişkenleri burada tanımlayabilirsiniz. ---------------------->
    // Aşağıdaki değişkenleri kullanmak zorunda değilsiniz. İsterseniz değişiklik yapabilirsiniz.
    unsigned sum = 0;                           // Konvolüsyon toplamını saklayabilirsiniz.
    unsigned first_loop_size = output_rows * 4;
    unsigned second_loop_size = output_cols * 4;
    unsigned third_loop_size = k * 4;
    unsigned fourth_loop_size = k * 4;
    unsigned first_loop_index, second_loop_index;
    // Assembly dilinde 2d konvolüsyon işlemini aşağıdaki blokta yazınız ----->
    __asm {

        XOR EAX, EAX // zero out the first loop index

     START_FIRST_LOOP:
        PUSH EAX // push it to the stack because we will be using eax later
        MOV first_loop_index, EAX
        XOR EBX, EBX // zero out the second loop index

     START_SECOND_LOOP:
        PUSH EBX // push it to the stack because we will be using ebx later
        MOV second_loop_index, EBX
        MOV sum, 0 // zero out the sum
        XOR ECX, ECX // zero out the third loop index

     START_THIRD_LOOP:
        XOR EDX, EDX // zero out the fourth loop index

        START_FOURTH_LOOP:
        //calculate the index of the input matrix
        PUSH EDX // push it before multiplication
        MOV EAX, first_loop_index
        ADD EAX, ECX // calculate the input row index 
        MUL m // calculate the input_index
        POP EDX
        MOV EBX, second_loop_index
        ADD EBX, EDX // calculate the input col index
        ADD EAX, EBX // add the input col to the input index
        MOV ESI, EAX // move it to esi for memory access
        
        // calculate the index for the kernel matrix
        PUSH EDX
        MOV EAX, ECX // move ecx to eax for kernel index calculation
        MUL k // multiply it by the kernel row/col size
        POP EDX // pop the value of edx from the stack
        ADD EAX, EDX // add it to eax for kernel index calculation
        MOV EDI, EAX // move it to edi for memory access

        MOV EAX, matrix // move the address of the matrix to eax
        ADD ESI, EAX // add it to esi for address calculation

        MOV EBX, kernel // move the address of the kernel to ebx
        ADD EDI, EBX // add it to edi for address calculation

        MOV EAX, [ESI] // move to eax the value of the i-th element of matrix
        MOV EBX, [EDI] // move to ebx the value of the ki-th element of the kernel

        PUSH EDX // save edx once again
        MUL EBX // multiply the value of the input matrix by that of the kernel matrix
        POP EDX // pop edx from the stack

        ADD sum, EAX // add the production value to the sum variable
        ADD EDX, 4 // if not start from the fourth loop back again
        CMP EDX, fourth_loop_size // compare edx with the fourth_loop_size if the fourth loop should break or not
        JL START_FOURTH_LOOP // if yes start from the third loop back again
     END_FOURTH_LOOP:
        ADD ECX, 4 // if not start from the third loop back again
        CMP ECX, third_loop_size // compare ecx with the third_loop_save if the third loop should break or not
        JL START_THIRD_LOOP // if yes start from the second loop
     END_THIRD_LOOP:
        MOV EAX, first_loop_index
        PUSH EDX // push edx to the stack for multiplication purposes
        MUL output_cols // multiply by output_cols to get the value of output_index
        POP EDX // pop edx as we have finished with the multiplication
        ADD EAX, second_loop_index // add ebx to eax to calculate the output_index
        MOV ESI, output // add the output address to eax
        ADD ESI, EAX // move it to esi for address calculation
        MOV EAX, sum // move the value of the sum variable to eax
        MOV [ESI], EAX // save the sum variable to the index of the output matrix
        POP EBX // pop eax from the stack
        ADD EBX, 4 // if not start from the second loop back again
        CMP EBX, second_loop_size // compare ebx with the second_loop_size if the second loop should break or not
        JL START_SECOND_LOOP // if yes start from the first loop back again
     END_SECOND_LOOP:
        POP EAX
        ADD EAX, 4 // if not start from the first loop back again
        CMP EAX, first_loop_size // compare eax with the first_loop_size if the first loop should break or not
        JL START_FIRST_LOOP // if yes finish the convolution problem
     END_FIRST_LOOP:
    }

    /******************* KODUN BU KISMINDAN ÖNCESİNDE DEĞİŞİKLİK YAPABİLİRSİNİZ - SONRAKİ KISIMLARI DEĞİŞTİRMEYİN *******************/


    // Write result to output file
    write_output("./output.txt", output, output_rows, output_cols);

    // Print result
    printf("\nOutput matrix after convolution: ");
    print_matrix(output, output_rows, output_cols, stdout);

    // Free allocated memory
    free(matrix);
    free(kernel);
    free(output);

    return 0;
}

void print_matrix(unsigned* matrix, unsigned rows, unsigned cols, FILE* file) {
    if (file == stdout) {
        printf("(%ux%u)\n", rows, cols);
    }
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fprintf(file, "%u ", matrix[i * cols + j]);
        }
        fprintf(file, "\n");
    }
}

void read_matrix(const char* filename, unsigned** matrix, unsigned* rows, unsigned* cols) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file %s\n", filename);
        exit(1);
    }

    // Read dimensions
    fscanf(file, "%u %u", rows, cols);
    *matrix = (unsigned*)malloc(((*rows) * (*cols)) * sizeof(unsigned));

    // Read matrix elements
    for (int i = 0; i < (*rows); i++) {
        for (int j = 0; j < (*cols); j++) {
            fscanf(file, "%u", &(*matrix)[i * (*cols) + j]);
        }
    }

    fclose(file);
}

void read_kernel(const char* filename, unsigned** kernel, unsigned* k) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file %s\n", filename);
        exit(1);
    }

    // Read kernel size
    fscanf(file, "%u", k);
    *kernel = (unsigned*)malloc((*k) * (*k) * sizeof(unsigned));

    // Read kernel elements
    for (int i = 0; i < (*k); i++) {
        for (int j = 0; j < (*k); j++) {
            fscanf(file, "%u", &(*kernel)[i * (*k) + j]);
        }
    }

    fclose(file);
}

void write_output(const char* filename, unsigned* output, unsigned rows, unsigned cols) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Error opening file %s\n", filename);
        exit(1);
    }

    // Write dimensions of the output matrix
    fprintf(file, "%u %u\n", rows, cols);

    // Write output matrix elements
    print_matrix(output, rows, cols, file);

    fclose(file);
}

void initialize_output(unsigned* output, unsigned output_rows, unsigned output_cols) {
    int i;
    for (i = 0; i < output_cols * output_rows; i++)
        output[i] = 0;
    
}

