#include<stdio.h>
#include<stdbool.h>
#define N 6
#define MAX_DISCARDED_EDGES 2 
#define MAX_VERTICES_IN_CYCLE 4 

#include"step_1.c"
#include"step_2.c"
#include"step_3.c"
#include"step_4.c"


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
