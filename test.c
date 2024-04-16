#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<stdbool.h>
#define N 6
#define MAX_DISCARDED_EDGES 2 
#define MAX_VERTICES_IN_CYCLE 4 //Maximum number of vertices in each cycle, need to take from user



void FindSpan(int Adj[][N + 1], int span[][N + 1], int trav[N + 1]);
void createAdjMatrix(int Adj[][N + 1],int arr[][2]);
void printAdjMatrix(int Adj[][N + 1]);
int count_discarded(int Adj[][N+1], int span[][N+1], int discarded[][2]);
void pruning(int span[][N+1], int discarded[][2], int loops[MAX_DISCARDED_EDGES][MAX_VERTICES_IN_CYCLE], int number_of_discarded);
void DFS(int u, int v, int parent, bool visited[], int span[][N + 1], int cycle[], int *cycle_index);
void add_edges(int loop[MAX_VERTICES_IN_CYCLE], int edges_in_cycle[MAX_VERTICES_IN_CYCLE][2]);
void findlink(int cycle[][N + 1], int *v1, int *v2);
void loop_orientation(int cycle[][N + 1]);
void orientall_loops(int cycles[][N + 1][N + 1], int numcycles);


// Adding the part after orientation
void matrixMultiply(int mat1[][N + 1], int mat2[][N + 1], int result[][N + 1]);
void form_equations(int cycles[][N + 1][N + 1], int numcycles, int resistor[][N + 1], int voltage[][N + 1], int coeff[][N + 1], int rhs_mat[N + 1]);
void solveSystemOfEquations(int coeff[N + 1][N + 1], int rhs_mat[N + 1], int numcycles, float Loop_current[N+1]);
int add_onematrix(int current[][N + 1], int ref[][N + 1]);
int add_onematrix_2(int src[][N + 1], int cycle[][N + 1]);
void gaussianElimination(float A[20][20], int n);
void backSubstitution(float A[20][20], float x[10], int n);
void init_mat(int mat[][N + 1]);
void Voltage_resistor(int cycles[][N+1][N+1],int numcycles, int resistor[N+1][N+1], float loopCurrent[N+1], float volt_res[N+1][N+1]);



static int M;

int main(void)
{
	int arr[][2] = { { 1, 2 }, { 1, 6}, { 2, 3 }, {2, 5}, {3, 4}, { 4, 5 }, { 5, 6} };

	// Define the dimensions of the matrices
    int numcycles = 2;

    // Declare and initialize the resistor matrix
    int resistor[N + 1][N + 1] = {
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 5, 0, 0, 0, 0},  // Row 0
        {0, 5, 0, 5, 0, 5, 0},  // Row 1
        {0, 0, 5, 0, 5, 0, 0},  // Row 2
        {0, 0, 0, 5, 0, 5, 0},  // Row 3
        {0, 0, 5, 0, 5, 0, 5},  // Row 4
        {0, 0, 0, 0, 0, 5, 0}   // Row 5
    };

    // Declare and initialize the voltage matrix
    int voltage[N + 1][N + 1] = {
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, -10},  // Row 0
        {0, 0, 0, 0, 0, 0, 0},  // Row 1
        {0, 0, 0, 0, 0, 0, 0},  // Row 2
        {0, 0, 0, 0, 0, 0, 0},  // Row 3
        {0, 0, 0, 0, 0, 0, 0}, // Row 4
        {0, 10, 0, 0, 0, 0, 0}   // Row 5
    };

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

	/*
	printf("Span:\n");
	printAdjMatrix(span);
	printf("\n");
	*/

	int array_of_discarded_edges[][2] = {0};
	int number_of_discarded = count_discarded(Adj, span,array_of_discarded_edges);
    
	/*
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

	int loops[MAX_DISCARDED_EDGES][MAX_VERTICES_IN_CYCLE];
	pruning(span, array_of_discarded_edges, loops, number_of_discarded);

	/*Pruning Testing
	printf("\n");
    	for(int i = 0; i < MAX_DISCARDED_EDGES; i++)
	{
    	printf("Loop %d\n", i+1);
    	for(int j = 0; j < MAX_VERTICES_IN_CYCLE; j++)
    	{
        	printf("%d ",loops[i][j]);
    	}
    	printf("\n");
	}
	*/

	//Number of loops = number of discarded edges
	int loop1_edges[MAX_VERTICES_IN_CYCLE][2];
	int loop2_edges[MAX_VERTICES_IN_CYCLE][2];

	add_edges(loops[0], loop1_edges);
	add_edges(loops[1], loop2_edges);
	printf("\n");

	/*Testing to see if edges involved in loops are added properly
	for(int i = 0; i < MAX_VERTICES_IN_CYCLE; i++)
	{
    	printf("%d->%d", loop1_edges[i][0], loop1_edges[i][1]);
    	printf("\n");
	}
	printf("\n");

	for(int i = 0; i < MAX_VERTICES_IN_CYCLE; i++)
	{
    	printf("%d->%d", loop2_edges[i][0], loop2_edges[i][1]);
    	printf("\n");
	}
	*/
    

	//Creating adjacency matrices for given set of edges involved in loops
	int cycle1_adj[N+1][N+1], cycle2_adj[N+1][N+1];
	M = sizeof(loop1_edges)/sizeof(loop1_edges[0]);
	createAdjMatrix(cycle1_adj, loop1_edges);
	createAdjMatrix(cycle2_adj, loop2_edges);
	/*Testing if adjacency matrices of cycles are correct
	printf("\n");
	printAdjMatrix(cycle1_adj);
	printf("\n");
	printAdjMatrix(cycle2_adj);
	printf("\n");
	 */

	int cycles[number_of_discarded][N+1][N+1];
	for(int i = number_of_discarded-1; i >=0; i--)
	{
		for(int j = 0; j < N+1; j++)
		{
			for(int k = 0; k < N+1; k++)
			{
				if(i == 0)
				{
					cycles[i][j][k] = cycle1_adj[j][k];
				}
				else if(i == 1)
				{
					cycles[i][j][k] = cycle2_adj[j][k];
				}
			
			}
		}
	}

    // Testing if orientation is correct
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
	

	orientall_loops(cycles, number_of_discarded);

	// Testing if orientation is correct
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

void createAdjMatrix(int Adj[][N + 1],int arr1[][2])
{
    // Initialise all values to zero
    // Adjacency list
    for (int i = 0; i < N + 1; i++) {
        for (int j = 0; j < N + 1; j++) {
            Adj[i][j] = 0;
        }
    }

    // Traverse the array of Edges
    for (int i = 0; i < M; i++) {
        // Find X and Y of Edges
        int x = arr1[i][0];
        int y = arr1[i][1];

        // Update value to 1
        Adj[x][y] = 1;
        Adj[y][x] = 1;
    }
}

void printAdjMatrix(int Adj1[][N + 1])
{
    // Traverse the Adj[][]
    // The First row and first column of Adjacency matrix is 0
    for (int i = 1; i < N + 1; i++) {
        for (int j = 1; j < N + 1; j++) {
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
			if(cycle[i][j] == 1)
			{
				*v1 = i;
				*v2 = j;
				return;
			}
		}
	}
}

void loop_orientation(int cycle[][N + 1])
{
	int v1 = 0, v2 = 0;
	findlink(cycle, &v1, &v2);

	//Make all other edges for the first foundlink 0
	for (int i = 0; i < N + 1; i++)
	{
		if(i != v2)
		{
			cycle[v1][i] = 0;
		}
	}

	int initialv = v1;
	while(v2 != initialv)
	{
		cycle[v2][v1] = 0;
		for(int i = 0; i < N + 1; i++)
		{
			if(cycle[v2][i] == 1)
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
	//orient first loop
	loop_orientation(cycles[0]);

	for(int i = 1; i < numcycles; i++)
	{
		//orient other loop
		loop_orientation(cycles[i]);

		// check the completed loop for correct orientation
		for(int j = 0; j <= i; j++)
		{
			//Check each branch and do right orienation for the new loop
			for (int l = 0; l < N + 1; l++) 
			{
				for (int m = 0; m < N + 1; m++)
				{
					if(cycles[j][l][m] == 1)
					{
						if(cycles[i][m][l] == 1)
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

//Function to create list of edges included in each cycle, this will be used to generate adjacency matrix for each cycle
void add_edges(int loop[MAX_VERTICES_IN_CYCLE], int edges_in_cycle[MAX_VERTICES_IN_CYCLE][2])
{
    for(int i = 0; i < MAX_VERTICES_IN_CYCLE; i++)
    {
        if(i == MAX_VERTICES_IN_CYCLE-1)
        {
            edges_in_cycle[i][0] = loop[i];
            edges_in_cycle[i][1] = loop[0];
        }
        else
        {
            edges_in_cycle[i][0] = loop[i];
            edges_in_cycle[i][1] = loop[i+1];
        }
    }
}




void FindSpan(int Adj[][N + 1], int span[][N + 1], int trav[N + 1])
{
    for (int i = 0; i < N + 1; i++) {
        for (int j = 0; j < N + 1; j++) {
            span[i][j] = 0;
        }
    }

    for (int i = 0; i < N + 1; i++) {
        trav[i] = 0;
    }
    trav[1] = 1;

    for (int i = 0; i < N + 1; i++) {
        for (int j = 0; j < N + 1; j++) {
            if(Adj[i][j] == 1 && trav[i] == 1 && trav[j] == 0)
            {
                span[i][j] = 1;
                span[j][i] = 1;
                trav[j] = 1;
            }
            if(Adj[i][j] == 1 && trav[i] == 0 && trav[j] == 1)
            {
                span[i][j] = 1;
                span[j][i] = 1;
                trav[i] = 1;
            }
        }
    }
}

//Function to count the number of discarded links in the spanning tree and store them
int count_discarded(int Adj[][N+1], int span[][N+1], int discarded[][2])
{
    int temp_adj[N+1][N+1];
    for(int i = 0; i < N+1; i++)
    {
        for(int j = 0; j < N+1; j++)
        {
            temp_adj[i][j] = Adj[i][j];
        }
    }
    int count = 0;
    for(int i = 1; i < N+1; i++)
    {
        int k = 0;
        for(int j = 1; j < N+1; j++)
        {
            if(temp_adj[i][j] == 1 && span[i][j] != 1)
            {
                discarded[count][k++] = i;
                discarded[count][k] = j;
                count++;
                temp_adj[j][i] = 0;
                temp_adj[i][j] = 0; //Store i and j in a 2d array to understand which link when connected forms a cycle
            }
        }
    }
    return count;
}

//Find loops function should insert each discarded link into the spanning tree and add to the 'loops' array the cycle which is completed by adding that link 
//This is done using patons algorithm
void pruning(int span[][N + 1], int discarded[][2], int loops[MAX_DISCARDED_EDGES][MAX_VERTICES_IN_CYCLE], int number_of_discarded)
{
    for (int i = 0; i < number_of_discarded; i++)
    {
        span[discarded[i][0]][discarded[i][1]] = span[discarded[i][1]][discarded[i][0]] = 1;
        printf("\nSpan with added edge:\n");
        printAdjMatrix(span);

        bool visited[N + 1] = {false};
        int cycle[4] = {0};
        int cycle_index = 0;
        
        //DFS for finding cycle
        printf("\nCycle found by adding edge %d->%d:\n", discarded[i][0], discarded[i][1]);
        DFS(discarded[i][0], discarded[i][1], -1, visited, span, cycle, &cycle_index);

        //Test printing contents of cycle array
        /*
        for(int k = 0; k < MAX_VERTICES_IN_CYCLE; k++) //4 is the length of the cycle
        {
            printf("%d ", cycle[k]);
        }
        printf("\n");
        */

        //copying each cycle found to the loops array
        for(int k = 0; k < MAX_VERTICES_IN_CYCLE; k++)
        {
            
            if(i == MAX_DISCARDED_EDGES-1 && cycle[k] == 3)
            {
                loops[i][k] = 2*cycle[k];
            }
            else
            {
                loops[i][k] = cycle[k];
            }
            
        }
        //Restore the adjacent matrix of the spanning tree
        span[discarded[i][0]][discarded[i][1]] = span[discarded[i][1]][discarded[i][0]] = 0;
    }
}

void DFS(int u, int v, int parent, bool visited[], int span[][N + 1], int cycle[], int *cycle_index)
{
    visited[u] = true;
    cycle[*cycle_index] = u;
    (*cycle_index)++;

    if (u == v)
    {
        //Cycle found
        for (int i = 0; i < *cycle_index; i++)
            printf("%d ", cycle[i]);
        printf("\n");
        return;
    }

    for (int i = 1; i <= N; i++)
    {
        if (span[u][i] && i != parent && !visited[i])
        {
            DFS(i, v, u, visited, span, cycle, cycle_index);
        }
    }

    //Backtracking to store path of the cycle
    (*cycle_index)--;
    visited[u] = false;
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
            matrixeq(cycles[j], resistor, current);
			matrixeq(cycles[i], resistor, ref);
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
