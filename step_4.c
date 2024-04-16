#include<stdio.h>
#define N 6

void solveSystemOfEquations(int coeff[N + 1][N + 1], int rhs_mat[N + 1], int numcycles, float Loop_current[N+1]);
void gaussianElimination(float A[20][20], int n);
void backSubstitution(float A[20][20], float x[10], int n);
void Voltage_resistor(int cycles[][N+1][N+1],int numcycles, int resistor[N+1][N+1], float loopCurrent[N+1], float volt_res[N+1][N+1]);
// Function to perform Gaussian Elimination
void gaussianElimination(float A[20][20], int n){
    int i, j, k;
    float c;
    for (j = 1; j <= n; j++) {
        for (i = 1; i <= n; i++) {
            if (i > j) {
                c = A[i][j] / A[j][j];
                for (k = 1; k <= n+1; k++) {
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

void solveSystemOfEquations(int coeff[N + 1][N + 1], int rhs_mat[N + 1], int numcycles, 
    float x[N + 1]) {
    // Convert equations to matrix form
    float A[20][20];
    for (int i = 1; i <= numcycles; i++) {
        for (int j = 1; j <= numcycles; j++) {
            A[i][j] = coeff[i][j];
        }
        A[i][numcycles + 1] = rhs_mat[i];
    }

    // Apply Gaussian Elimination
    gaussianElimination(A, numcycles);

    // Back Substitution
    backSubstitution(A, x, numcycles);

    // Output the solution
    // printf("Solution:\n");
    // for (int i = 1; i <= numcycles; i++) {
    //     printf("x[%d] = %.2f\n", i, x[i]);
    // }
}

void Voltage_resistor(int cycles[][N+1][N+1],int numcycles, int resistor[N+1][N+1], float loopCurrent[N+1], float volt_res[N+1][N+1]){
    for(int i = 0; i<numcycles; i++){
        for(int j = 0; j<N+1; j++){
            for(int k = 0; k<N+1; k++){
                if(cycles[i][j][k] == 1){
                    volt_res[j][k] = loopCurrent[i+1] * resistor[j][k];
                }
                else if(cycles[i][j][k] == -1){
                    volt_res[j][k] += loopCurrent[i+1] * resistor[j][k];
                }
            }
        }
    }
} 
