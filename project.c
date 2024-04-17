#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define N 8

// STEP 1
void FindSpan(int Adj[][N + 1], int span[][N + 1], int trav[N + 1]);
void createAdjMatrix(int Adj[][N + 1], int arr[][2]);
void printAdjMatrix(int Adj[][N + 1]);
int count_discarded(int Adj[][N + 1], int span[][N + 1], int discarded[][2]);
void pruning(int span[][N + 1], int discarded[][2], int cycles[][N + 1][N + 1], int number_of_discarded);
bool DFS(int u, int v, int parent, bool visited[], int span[][N + 1], int cycle[], int *cycle_index);
void findlink(int cycle[][N + 1], int *v1, int *v2);

// STEP 2
void loop_orientation(int cycle[][N + 1]);
void orientall_loops(int cycles[][N + 1][N + 1], int numcycles);
void init_cycles(int cycles[][N + 1][N + 1], int number_of_discarded);


// STEP 3
void matrixMultiply(int mat1[][N + 1], int mat2[][N + 1], int result[][N + 1]);
void form_equations(int cycles[][N + 1][N + 1], int numcycles, int resistor[][N + 1], int voltage[][N + 1], int coeff[][N + 1], int rhs_mat[N + 1]);
int add_onematrix(int current[][N + 1], int ref[][N + 1]);
int add_onematrix_2(int src[][N + 1], int cycle[][N + 1]);
void init_mat(int mat[][N + 1]);
void print_float(int Adj[][N + 1]);
void print(int Adj[][N + 1]);


void Voltage_resistor(int cycles[][N + 1][N + 1], int numcycles, int resistor[N + 1][N + 1], float loopCurrent[N + 1], float volt_res[N + 1][N + 1]);
void gaussianElimination(float A[20][20], int n);
void backSubstitution(float A[20][20], float x[10], int n);
void current_mat(int cycles[][N + 1][N + 1], int numcycles, float loopCurrent[N + 1], float curr_mat[N + 1][N + 1]);

void solveSystemOfEquations(int coeff[N + 1][N + 1], int rhs_mat[N + 1], int numcycles, float Loop_current[N + 1]);

static int M;

int main()
{
    int arr[][2] = {{1, 2}, {1, 8}, {2, 3}, {2, 7}, {3, 4}, {4, 5}, {4, 7}, {5, 6}, {6, 7}, {7, 8}};

    // Declare and initialize the resistor matrix
    int resistor[N + 1][N + 1] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 5, 0, 0, 0, 0, 0, 0},
        {0, 5, 0, 5, 0, 0, 0, 5, 0},
        {0, 0, 5, 0, 5, 0, 0, 0, 0},
        {0, 0, 0, 5, 0, 0, 0, 5, 0},
        {0, 0, 0, 0, 0, 0, 5, 0, 0},
        {0, 0, 0, 0, 0, 5, 0, 0, 0},
        {0, 0, 5, 0, 5, 0, 0, 0, 5},
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
    /*Declare and initialize the cycles matrix
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
    */

    M = sizeof(arr) / sizeof(arr[0]);

    int Adj[N + 1][N + 1], span[N + 1][N + 1];
    int trav[N + 1];

    createAdjMatrix(Adj, arr);

    /*Print Adjacency Matrix
    printf("Adjacency Matrix:\n");
    printAdjMatrix(Adj);
    printf("\n");
    */

    FindSpan(Adj, span, trav);

    /*Spanning tree testing successful
    printf("Span:\n");
    printAdjMatrix(span);
    printf("\n");
    */

    int array_of_discarded_edges[][2] = {{0}};
    int number_of_discarded = count_discarded(Adj, span, array_of_discarded_edges);

    /*Discarded links testing successful
    printf("Number of Discarded links in the spanning tree = %d\n", number_of_discarded);
    printf("\n");
    printf("Adjacency matrix is \n");
    printAdjMatrix(Adj);
    printf("\n\n");
    printf("Printing the discarded edges\n");
    for(int i = 0; i < number_of_discarded; i++)
    {
        printf("%d->%d", array_of_discarded_edges[i][0], array_of_discarded_edges[i][1]);
        printf("\n");
    }
    */

    int cycles[number_of_discarded][N + 1][N + 1]; // Will store all cycles in their adjacency matrix form, initialised to 0, number of individual cycles will be equal to number of discarded links
    init_cycles(cycles, number_of_discarded);      // Initialize cycles

    pruning(span, array_of_discarded_edges, cycles, number_of_discarded);
    /*Pruning Testing Successful
    for(int i = 0; i < number_of_discarded; i++)
    {
        printf("Cycle %d\n", i+1);
        for(int j = 0; j < N+1; j++)
        {
            for(int k = 0; k < N+1; k++)
            {
                printf("%d ", cycles[i][j][k]);
            }
            printf("\n");
        }
    }
    */

    orientall_loops(cycles, number_of_discarded);
    // Testing of loop orientation is successful
    for (int i = 0; i < number_of_discarded; i++)
    {
        printf("Cycle %d\n", i + 1);
        for (int j = 0; j < N + 1; j++)
        {
            for (int k = 0; k < N + 1; k++)
            {
                printf("%d ", cycles[i][j][k]);
            }
            printf("\n");
        }
    }

    int coeff[N + 1][N + 1];
    int rhs_mat[N + 1];

    init_mat(coeff);
    for (int i = 0; i < N + 1; i++)
    {
        rhs_mat[i] = 0;
    }

    // Call form_equations to generate the system of equations
    form_equations(cycles, number_of_discarded, resistor, voltage, coeff, rhs_mat);

    float Loop_current[N + 1];

    // Call solveSystemOfEquations to solve the system of equations
    solveSystemOfEquations(coeff, rhs_mat, number_of_discarded, Loop_current);
    printf("Solution:\n");
    for (int i = 1; i <= number_of_discarded; i++)
    {
        printf("x[%d] = %.2f\n", i, Loop_current[i]);
    }

    float vr[N + 1][N + 1];
    for (int i = 0; i < N + 1; i++)
    {
        for (int j = 0; j < N + 1; j++)
        {
            vr[i][j] = 0;
        }
    }

    float bc[N + 1][N + 1];
    for (int i = 0; i < N + 1; i++)
    {
        for (int j = 0; j < N + 1; j++)
        {
            bc[i][j] = 0;
        }
    }

    current_mat(cycles, number_of_discarded, Loop_current, bc);
    printf("Printing current matrix :\n");
    for (int i = 0; i < N + 1; i++)
    {
        for (int j = 0; j < N + 1; j++)
        {
            if (bc[i][j] == 0 || bc[j][i] == 0)
                printf(" 0.00 ");
            else if (i > j)
            {
                printf(" %.2f ", bc[i][j]);
            }
            else
            {
                printf(" %.2f ", -bc[i][j]);
            }
        }
        printf("\n");
    }
    Voltage_resistor(cycles, number_of_discarded, resistor, Loop_current, vr);
    printf("Printing voltages matrix :\n");

    for (int i = 0; i < N + 1; i++)
    {
        for (int j = 0; j < N + 1; j++)
        {
            if (bc[i][j] == 0 || bc[j][i] == 0)
                printf(" 0.00 ");
            else if (i > j)
            {
                printf(" %.2f ", bc[i][j]);
            }
            else
            {
                printf(" %.2f ", -bc[i][j]);
            }
        }
        printf("\n");
    }

    return 0;
}

// STEP 1 STARTS

void createAdjMatrix(int Adj[][N + 1], int arr1[][2])
{
    // Initialise all values to zero
    // Adjacency list
    for (int i = 0; i < N + 1; i++)
    {
        for (int j = 0; j < N + 1; j++)
        {
            Adj[i][j] = 0;
        }
    }

    // Traverse the array of Edges
    for (int i = 0; i < M; i++)
    {
        // Find X and Y of Edges
        int x = arr1[i][0];
        int y = arr1[i][1];

        // Update value to 1
        Adj[x][y] = 1;
        Adj[y][x] = 1;
    }
}

void FindSpan(int Adj[][N + 1], int span[][N + 1], int trav[N + 1])
{
    for (int i = 0; i < N + 1; i++)
    {
        for (int j = 0; j < N + 1; j++)
        {
            span[i][j] = 0;
        }
    }

    for (int i = 0; i < N + 1; i++)
    {
        trav[i] = 0;
    }
    trav[1] = 1;

    for (int i = 0; i < N + 1; i++)
    {
        for (int j = 0; j < N + 1; j++)
        {
            if (Adj[i][j] == 1 && trav[i] == 1 && trav[j] == 0)
            {
                span[i][j] = 1;
                span[j][i] = 1;
                trav[j] = 1;
            }
            if (Adj[i][j] == 1 && trav[i] == 0 && trav[j] == 1)
            {
                span[i][j] = 1;
                span[j][i] = 1;
                trav[i] = 1;
            }
        }
    }
}

void printAdjMatrix(int Adj1[][N + 1])
{
    // Traverse the Adj[][]
    // The First row and first column of Adjacency matrix is 0
    for (int i = 1; i < N + 1; i++)
    {
        for (int j = 1; j < N + 1; j++)
        {
            // Print the value at Adj[i][j]
            printf("%d ", Adj1[i][j]);
        }
        printf("\n");
    }
}

void findlink(int cycle[][N + 1], int *v1, int *v2)
{
    for (int i = 0; i < N + 1; i++)
    {
        for (int j = 0; j < N + 1; j++)
        {
            if (cycle[i][j] == 1)
            {
                *v1 = i;
                *v2 = j;
                return;
            }
        }
    }
}

int count_discarded(int Adj[][N + 1], int span[][N + 1], int discarded[][2])
{
    int temp_adj[N + 1][N + 1];
    for (int i = 0; i < N + 1; i++)
    {
        for (int j = 0; j < N + 1; j++)
        {
            temp_adj[i][j] = Adj[i][j];
        }
    }
    int count = 0;
    for (int i = 1; i < N + 1; i++)
    {
        int k = 0;
        for (int j = 1; j < N + 1; j++)
        {
            if (temp_adj[i][j] == 1 && span[i][j] != 1)
            {
                discarded[count][k++] = i;
                discarded[count][k] = j;
                count++;
                temp_adj[j][i] = 0;
                temp_adj[i][j] = 0; // Store i and j in a 2d array to understand which link when connected forms a cycle
            }
        }
    }
    return count;
}

// Find loops function should insert each discarded link into the spanning tree and add to the 'loops' array the cycle which is completed by adding that link
// This is done using patons algorithm


void pruning(int span[][N + 1], int discarded[][2], int cycles[][N + 1][N + 1], int number_of_discarded)
{
    for (int i = 0; i < number_of_discarded; i++)
    {
        span[discarded[i][0]][discarded[i][1]] = span[discarded[i][1]][discarded[i][0]] = 1;

        /*Testing of adding edges to spanning tree
        printf("\nSpan with added edge:\n");
        printAdjMatrix(span);
        printf("\n")
        */

        bool visited[N + 1] = {false};
        int cycle[N + 1] = {0};
        int cycle_index = 0;

        // DFS for finding cycle
        // printf("\nCycle found by adding edge %d->%d:\n", discarded[i][0], discarded[i][1]);
        DFS(discarded[i][0], discarded[i][1], -1, visited, span, cycle, &cycle_index);

        // Adding the adjacency matrix of each loop to the cycles array
        for (int j = 0; j < N; j++)
        {
            if (cycle[j + 1] == 0) // The last node will be connected to the first node of the cycle
            {
                cycles[i][cycle[j]][cycle[0]] = 1; // Mark the elements of adjacency matrix of each of the cycle, each consecutive pair is an edge, which is to be marked in adjacency matrix
                cycles[i][cycle[0]][cycle[j]] = 1;
                break; // Break here because all the nodes in the loop are marked in its respective adjacency matrix(Last is paired with first)
            }
            else
            {
                cycles[i][cycle[j]][cycle[j + 1]] = 1;
                cycles[i][cycle[j + 1]][cycle[j]] = 1;
            }
        }

        /*Test printing contents of cycle array
        for(int k = 0; k < N+1; k++)
        {
            printf("%d ", cycle[k]);
        }
        printf("\n");
        */

        // Restore the adjacent matrix of the spanning tree
        span[discarded[i][0]][discarded[i][1]] = span[discarded[i][1]][discarded[i][0]] = 0;
    }
}

bool DFS(int u, int v, int parent, bool visited[], int span[][N + 1], int cycle[], int *cycle_index)
{
    visited[u] = true;
    cycle[*cycle_index] = u;
    (*cycle_index)++;

    if (u == v && (*cycle_index) > 1) // cycle is found, stop DFS
    {
        return true;
    }

    for (int i = 1; i < N + 1; i++)
    {
        if (span[u][i] && i != parent && !visited[i]) // This checks if 2 nodes are adjacent, prevents to go back to parent in DFS, and prevents to go back to node which is already visited
        {
            if (DFS(i, v, u, visited, span, cycle, cycle_index))
            {
                return true; // If a cycle if found in a DFS traversal, then further traversal has to be stopped
            }
        }
    }

    // If no cycl is found then backtrack
    if (!visited[v])
    {
        (*cycle_index)--;
        visited[u] = false;
        cycle[*cycle_index] = 0;
    }
    return false; // No cycle in this path, continue
}




// <------------------------------------------------------------------------>

// STEP 2 STARTS

void loop_orientation(int cycle[][N + 1])
{
    int v1 = 0, v2 = 0;
    findlink(cycle, &v1, &v2);

    // Make all other edges for the first foundlink 0
    for (int i = 0; i < N + 1; i++)
    {
        if (i != v2)
        {
            cycle[v1][i] = 0;
        }
    }

    int initialv = v1;
    while (v2 != initialv)
    {
        cycle[v2][v1] = 0;
        for (int i = 0; i < N + 1; i++)
        {
            if (cycle[v2][i] == 1)
            {
                v1 = v2;
                v2 = i;
                break;
            }
        }
    }
}

void orientall_loops(int cycles[][N + 1][N + 1], int numcycles)
{
    // orient first loop
    loop_orientation(cycles[0]);

    for (int i = 1; i < numcycles; i++)
    {
        // orient other loop
        loop_orientation(cycles[i]);

        // check the completed loop for correct orientation
        for (int j = 0; j <= i; j++)
        {
            // Check each branch and do right orienation for the new loop
            for (int l = 0; l < N + 1; l++)
            {
                for (int m = 0; m < N + 1; m++)
                {
                    if (cycles[j][l][m] == 1)
                    {
                        if (cycles[i][m][l] == 1)
                        {
                            cycles[i][m][l] = 0;
                            cycles[i][l][m] = -1;
                        }
                    }
                }
            }
        }
    }
}


// Function to count the number of discarded links in the spanning tree and store them

void init_cycles(int cycles[][N + 1][N + 1], int number_of_discarded)
{
    for (int i = 0; i < number_of_discarded; i++)
    {
        for (int j = 0; j < N + 1; j++)
        {
            for (int k = 0; k < N + 1; k++)
            {
                cycles[i][j][k] = 0;
            }
        }
    }
}



// <--------------------------------------------------------------->
// STEP 3 STARTS


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



//<---------------------------------------------------------------------->
// STEP 4 STARTS


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


// Function to perform Gaussian Elimination
void gaussianElimination(float A[20][20], int n)
{
    int i, j, k;
    float c;
    for (j = 1; j <= n; j++)
    {
        for (i = 1; i <= n; i++)
        {
            if (i > j)
            {
                c = A[i][j] / A[j][j];
                for (k = 1; k <= n + 1; k++)
                {
                    A[i][k] = A[i][k] - c * A[j][k];
                }
            }
        }
    }
}

// Function for back substitution
void backSubstitution(float A[20][20], float x[10], int n)
{
    int i, j;
    float sum;
    x[n] = A[n][n + 1] / A[n][n];
    for (i = n - 1; i >= 1; i--)
    {
        sum = 0;
        for (j = i + 1; j <= n; j++)
        {
            sum += A[i][j] * x[j];
        }
        x[i] = (A[i][n + 1] - sum) / A[i][i];
    }
}

void solveSystemOfEquations(int coeff[N + 1][N + 1], int rhs_mat[N + 1], int numcycles,
                            float x[N + 1])
{
    // Convert equations to matrix form
    float A[20][20];
    for (int i = 1; i <= numcycles; i++)
    {
        for (int j = 1; j <= numcycles; j++)
        {
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
void current_mat(int cycles[][N + 1][N + 1], int numcycles, float loopCurrent[N + 1], float curr_mat[N + 1][N + 1])
{
    for (int i = 0; i < numcycles; i++)
    {
        for (int j = 0; j < N + 1; j++)
        {
            for (int k = 0; k < N + 1; k++)
            {
                // if(cycles[i][j][k] == 1){
                //     curr_mat[j][k] = loopCurrent[i+1];
                // }
                if (cycles[i][k][j] == -1 || cycles[i][j][k] == -1)
                {
                    curr_mat[j][k] -= loopCurrent[i + 1];
                }
                else if (cycles[i][k][j] == 1 || cycles[i][j][k] == 1)
                {
                    curr_mat[j][k] += loopCurrent[i + 1];
                }
            }
        }
    }
}

void Voltage_resistor(int cycles[][N + 1][N + 1], int numcycles, int resistor[N + 1][N + 1], float loopCurrent[N + 1], float volt_res[N + 1][N + 1])
{
    for (int i = 0; i < numcycles; i++)
    {
        for (int j = 0; j < N + 1; j++)
        {
            for (int k = 0; k < N + 1; k++)
            {
                // if(cycles[i][j][k] == 1){
                //     volt_res[j][k] = loopCurrent[i+1] * resistor[j][k];
                // }
                if (cycles[i][k][j] == -1 || cycles[i][j][k] == -1)
                {
                    volt_res[j][k] -= loopCurrent[i + 1] * resistor[j][k];
                }
                else if (cycles[i][k][j] == 1 || cycles[i][j][k] == 1)
                {
                    volt_res[j][k] += loopCurrent[i + 1] * resistor[j][k];
                }
            }
        }
    }
}
