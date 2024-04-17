#include <stdio.h>

#define N 3 // Change this value to adjust matrix size

// Function to multiply two matrices
void matrixMultiply(int mat1[][N], int mat2[][N], int result[][N]) {
    int i, j, k;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            result[i][j] = 0;
            for (k = 0; k < N; k++) {
                result[i][j] += mat1[i][k] * mat2[k][j];
            }
        }
    }
}

// Function to display a matrix
void displayMatrix(int matrix[][N]) {
    int i, j;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            printf("%d\t", matrix[i][j]);
        }
        printf("\n");
    }
}

int main() {
    int mat1[N][N] = {{1, 2, 3},
                      {4, 5, 6},
                      {7, 8, 9}};

    int mat2[N][N] = {{9, 8, 7},
                      {6, 5, 4},
                      {3, 2, 1}};

    int result[N][N];

    // Multiply the matrices
    matrixMultiply(mat1, mat2, result);

    // Display the result
    printf("Resultant Matrix:\n");
    displayMatrix(result);

    return 0;
}
