#include<stdio.h>
#include<stdbool.h>
#include"step_2.c"

// Adding the part after orientation
void matrixMultiply(int mat1[][N + 1], int mat2[][N + 1], int result[][N + 1]);
void form_equations(int cycles[][N + 1][N + 1], int numcycles, int resistor[][N + 1], int voltage[][N + 1], int coeff[][N + 1], int rhs_mat[N + 1]);
int add_onematrix(int current[][N + 1], int ref[][N + 1]);
int add_onematrix_2(int src[][N + 1], int cycle[][N + 1]);
void init_mat(int mat[][N + 1]);
void print_float(int Adj[][N + 1]);
void print(int Adj[][N + 1]);
void matrixeq(int cycle[][N + 1], int resi[][N + 1], int result[][N + 1]);

void matrixeq(int cycle[][N + 1], int resi[][N + 1], int result[][N + 1])
{
    for (int i = 0; i < N + 1; i++)
    {
        for (int j = 0; j < N + 1; j++)
        {
            if (cycle[i][j] != 0)
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
// Function to multiply two matrices
void matrixMultiply(int mat1[][N + 1], int mat2[][N + 1], int result[][N + 1])
{
    int i, j, k;
    for (i = 0; i < N + 1; i++)
    {
        for (j = 0; j < N + 1; j++)
        {
            result[i][j] = 0;
            for (k = 0; k < N + 1; k++)
            {
                result[i][j] += mat1[i][k] * mat2[k][j];
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

    // coeff = coefficient matrix
    // rhs_matrix = the matrix on rhs of the equation that makes it non homo

    for (int i = 0; i < numcycles; i++)
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

    for (int l = 0; l < numcycles; l++)
    {
        matrixeq(cycles[l], voltage, mat);
        int temp = add_onematrix_2(mat, cycles[l]);
        rhs_mat[l + 1] = temp;
    }

    printf("coeff: \n");
    print(coeff);
    printf("rhs_mat: \n");
    for (int i = 0; i < N + 1; i++)
    {
        printf("%i ", rhs_mat[i]);
    }
    printf("\n");
}


int add_onematrix(int current[][N + 1], int ref[][N + 1])
{
    int sum = 0;
    for (int i = 0; i < N + 1; i++)
    {
        for (int j = 0; j < N + 1; j++)
        {
            if (current[i][j] != 0 && ref[i][j] > 0)
            {
                sum = sum + current[i][j];
            }
            else if (current[i][j] != 0 && ref[i][j] < 0)
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
            if (cycle[i][j] > 0 && src[i][j] != 0)
            {
                sum = sum + src[i][j];
            }
            // else
            // if(cycle[i][j] > 0 && src[i][j] < 0)
            // {
            //     sum = sum + src[i][j];
            // }
            else if (cycle[i][j] < 0 && src[i][j] != 0)
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

void init_mat(int mat[][N + 1])
{
    for (int i = 0; i < N + 1; i++)
    {
        for (int j = 0; j < N + 1; j++)
        {
            mat[i][j] = 0;
        }
    }
}


void print(int Adj[][N + 1])
{

    // Traverse the Adj[][]
    for (int i = 0; i < N + 1; i++)
    {
        for (int j = 0; j < N + 1; j++)
        {

            // Print the value at Adj[i][j]
            printf("%d ", Adj[i][j]);
        }
        printf("\n");
    }
}

void print_float(int Adj[][N + 1])
{

    // Traverse the Adj[][]
    for (int i = 0; i < N + 1; i++)
    {
        for (int j = 0; j < N + 1; j++)
        {

            // Print the value at Adj[i][j]
            printf("%f ", Adj[i][j]);
        }
        printf("\n");
    }
}
