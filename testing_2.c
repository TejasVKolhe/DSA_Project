#include <stdio.h>

#define N 8
// Function prototypes

void matrixMultiply(int mat1[][N + 1], int mat2[][N + 1], int result[][N + 1]);
void form_equations(int cycles[][N + 1][N + 1], int numcycles, int resistor[][N + 1], int voltage[][N + 1], int coeff[][N + 1], int rhs_mat[N + 1]);
void solveSystemOfEquations(int coeff[N + 1][N + 1], int rhs_mat[N + 1], int numcycles, float Loop_current[N+1]);
int add_onematrix(int current[][N + 1], int ref[][N + 1]);
int add_onematrix_2(int src[][N + 1], int cycle[][N + 1]);
void gaussianElimination(float A[20][20], int n);
void backSubstitution(float A[20][20], float x[10], int n);
void init_mat(int mat[][N + 1]);
void Voltage_resistor(int cycles[][N+1][N+1],int numcycles, int resistor[N+1][N+1], float loopCurrent[N+1], float volt_res[N+1][N+1]);

int main() {
    // Define the dimensions of the matrices
    int numcycles = 3;

    // Declare and initialize the resistor matrix
    int resistor[N + 1][N + 1] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 5, 0, 0, 0, 0, 0, 0},
        {0, 5, 0, 5, 0, 0, 0, 5, 0},
        {0, 0, 5, 0, 5, 0, 0, 0, 0},
        {0, 0, 0, 5, 0, 0, 0, 5, 0},
        {0, 0, 0, 0, 0, 0, 5, 0, 0},
        {0, 0, 0, 0, 0, 5, 0, 0, 0},
        {0, 0, 0, 0, 5, 0, 0, 0, 5},
        {0, 0, 0, 0, 0, 0, 0, 5, 0},

    };

    // Declare and initialize the voltage matrix
    int voltage[N + 1][N + 1] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 10},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, -10, 0, 0, 0, 0, 0, 0, 0},
    };

    // Declare and initialize the cycles matrix
    int cycles[3][N + 1][N + 1] = {
    {
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 1},
        {0, 1, 0, 0, 0, 0, 0, 0, 0},
        
    },
    {
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0, -1, 0},
        {0, 0, 0, 0, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
    },
    {
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 0, -1, 0},
        {0, 0, 0, 0, 0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
    }
};

    // Declare matrices coeff and rhs_mat in main
    int coeff[N + 1][N + 1];
    int rhs_mat[N + 1];

    init_mat(coeff);
    for(int i = 0; i < N + 1; i++)
    {
        rhs_mat[i] = 0;
    }

    // Call form_equations to generate the system of equations
    form_equations(cycles, numcycles, resistor, voltage, coeff, rhs_mat);

    float Loop_current[N+1];

    // Call solveSystemOfEquations to solve the system of equations
    solveSystemOfEquations(coeff, rhs_mat, numcycles, Loop_current);
    printf("Solution:\n");
    for (int i = 1; i <= numcycles; i++) {
        printf("x[%d] = %.2f\n", i, Loop_current[i]);
    }

    float vr[N+1][N+1];
    for(int i = 0; i<N+1; i++){
        for(int j =0 ; j<N+1 ; j++){
            vr[i][j] = 0;
        }
    }
    Voltage_resistor(cycles, numcycles, resistor, Loop_current, vr);

    for(int i = 0; i<N+1; i++){
        for(int j =0 ; j<N+1 ; j++){
            printf(" %.2f ", vr[i][j]);
        }
        printf("\n");
    }

    return 0;
}

void Voltage_resistor(int cycles[][N+1][N+1],int numcycles, int resistor[N+1][N+1], float loopCurrent[N+1], float volt_res[N+1][N+1]){
    for(int i = 0; i<numcycles; i++){
        for(int j = 0; j<N+1; j++){
            for(int k = 0; k<N+1; k++){
                if(cycles[i][j][k] == 1){
                    volt_res[j][k] = loopCurrent[i+1] * resistor[j][k];
                }
                else if( numcycles%2==1 && cycles[i][j][k] == -1){
                    volt_res[j][k] -= loopCurrent[i+1] * resistor[j][k];
                }
                else if( numcycles%2==0 && cycles[i][j][k] == -1){
                    volt_res[j][k] += loopCurrent[i+1] * resistor[j][k];
                }
            }
        }
    }
} 

// Function to multiply two matrices
void matrixMultiply(int mat1[][N + 1], int mat2[][N + 1], int result[][N + 1]) {
    int i, j, k;
    for (i = 0; i < N + 1; i++) {
        for (j = 0; j < N + 1; j++) {
            result[i][j] = 0;
            for (k = 0; k < N + 1; k++) {
                result[i][j] += mat1[i][k] * mat2[k][j];
            }
        }
    }
}


int add_onematrix(int current[][N + 1], int ref[][N + 1])
{
	int sum = 0;
	for (int i = 0; i < N + 1; i++) 
	{
        for (int j = 0; j < N + 1; j++) 
		{
			if(current[i][j] != 0 && ref[i][j] > 0)
			{
				sum = sum + current[i][j];
			}
            else
            if(current[i][j] != 0 && ref[i][j] < 0)
            {
                sum = sum - current[i][j];
            }
            // else
            // if(current[i][j] < 0 && ref[i][j] < 0)
            // {
            //     sum = sum - current[i][j];
            // }
            // else 
            // if(current[i][j] < 0 && ref[i][j] > 0)
            // {
            //     sum = sum + current[i][j];
            // }

		}
	}
	return sum;
}

int add_onematrix_2(int src[][N + 1], int cycle[][N + 1])
{
	int sum = 0;
	for (int i = 0; i < N + 1; i++) 
	{
        for (int j = 0; j < N + 1; j++) 
		{
			if(cycle[i][j] > 0 && src[i][j] != 0)
			{
				sum = sum + src[i][j]; 
			}
            // else
            // if(cycle[i][j] > 0 && src[i][j] < 0)
            // {
            //     sum = sum + src[i][j];
            // }
            else
            if(cycle[i][j] < 0 && src[i][j] != 0)
            {
                sum = sum - src[i][j];
            }
            // else
            // if(cycle[i][j] < 0 && src[i][j] < 0)
            // {
            //     sum = sum - src[i][j];
            // }
		}
	}
	return sum;
}

void print(int Adj[][N + 1])
{

	// Traverse the Adj[][]
	for (int i = 0; i < N + 1; i++) {
		for (int j = 0; j < N + 1; j++) {

			// Print the value at Adj[i][j]
			printf("%d ", Adj[i][j]);
		}
		printf("\n");
	}
}

void init_mat(int mat[][N + 1])
{
    for (int i = 0; i < N + 1; i++) {
		for (int j = 0; j < N + 1; j++) {
			mat[i][j] = 0;
		}
	}
}

void tran_matrix(int mat[][N + 1])
{
    for (int i = 0; i < N + 1; i++) {
		for (int j = 0; j < N + 1; j++) {
			mat[i][j] = mat[j][i];
		}
	}
}

void matrixeq(int cycle[][N + 1], int resi[][N + 1], int result[][N + 1])
{
    for (int i = 0; i < N + 1; i++) {
		for (int j = 0; j < N + 1; j++) 
        {
            if(cycle[i][j] != 0)
            {
                result[i][j] = resi[i][j] * cycle[i][j];
            }
            else
            {
                result[i][j] = 0;
            }
        }
    }
}

void form_equations(int cycles[][N + 1][N + 1], int numcycles, int resistor[][N + 1], int voltage[][N + 1], int coeff[][N + 1], int rhs_mat[N + 1])
{
	int current[N + 1][N + 1], ref[N + 1][N + 1], mat[N + 1][N + 1];

    init_mat(current);
    init_mat(ref);
    init_mat(mat);

	//coeff = coefficient matrix
	//rhs_matrix = the matrix on rhs of the equation that makes it non homo

	for(int i = 0; i < numcycles; i++)
	{
		for (int j = 0; j < numcycles; j++) 
		{
            matrixeq(cycles[i], resistor, current);
			matrixeq(cycles[j], resistor, ref);
            /*printf("\nCurrent: \n");
            print(current);
            printf("\nref: \n");
            print(ref);*/
			int temp = add_onematrix(current, ref);
			coeff[i + 1][j + 1] = temp;
		}
	}

	for(int l = 0; l < numcycles; l++)
	{
		matrixeq(cycles[l], voltage, mat);
		int temp = add_onematrix_2(mat, cycles[l]);
		rhs_mat[l + 1] = temp;
	}

    printf("coeff: \n");
    print(coeff);
    printf("rhs_mat: \n");
    for(int i = 0; i < N + 1; i++)
    {
        printf("%i ", rhs_mat[i]);
    }
    printf("\n");
}


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
