#include <stdio.h>

// Function prototypes
void gaussianElimination(float A[20][20], int n);
void backSubstitution(float A[20][20], float x[10], int n);

int main() {
    int i, j, n;
    float A[20][20], x[10];

    printf("\nEnter the order of matrix: ");
    scanf("%d", &n);

    printf("\nEnter the elements of augmented matrix row-wise:\n\n");
    for (i = 1; i <= n; i++) {
        for (j = 1; j <= (n + 1); j++) {
            printf("A[%d][%d] : ", i, j);
            scanf("%f", &A[i][j]);
        }
    }

    // Solve equations
    gaussianElimination(A, n);
    backSubstitution(A, x, n);

    // Output solutions
    printf("\nThe solution is: \n");
    for (i = 1; i <= n; i++) {
        printf("\nx%d=%f\t", i, x[i]);
    }

    return 0;
}

// Function to perform Gaussian Elimination
void gaussianElimination(float A[20][20], int n) {
    int i, j, k;
    float c;
    for (j = 1; j <= n; j++) {
        for (i = 1; i <= n; i++) {
            if (i > j) {
                c = A[i][j] / A[j][j];
                for (k = 1; k <= n + 1; k++) {
                    A[i][k] = A[i][k] - c * A[j][k];
                }
            }
        }
    }
}

// Function for back substitution
void backSubstitution(float A[20][20], float x[10], int n) {
    int i, j;
    float sum;
    x[n] = A[n][n + 1] / A[n][n];
    for (i = n - 1; i >= 1; i--) {
        sum = 0;
        for (j = i + 1; j <= n; j++) {
            sum += A[i][j] * x[j];
        }
        x[i] = (A[i][n + 1] - sum) / A[i][i];
    }
}
