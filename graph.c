// C program for the above approach
#include <stdio.h>

// N vertices and M Edges
int N, M;

// Function to create Adjacency Matrix
void createAdjMatrix(int Adj[][N + 1],
					int arr[][2])
{

	// Initialise all value to this
	// Adjacency list to zero
	for (int i = 0; i < N + 1; i++) {

		for (int j = 0; j < N + 1; j++) {
			Adj[i][j] = 0;
		}
	}

	// Traverse the array of Edges
	for (int i = 0; i < M; i++) {

		// Find X and Y of Edges
		int x = arr[i][0];
		int y = arr[i][1];

		// Update value to 1
		Adj[x][y] = 1;
		Adj[y][x] = 1;
	}
}

// Function to print the created
// Adjacency Matrix
void printAdjMatrix(int Adj[][N + 1])
{

	// Traverse the Adj[][]
	for (int i = 1; i < N + 1; i++) {
		for (int j = 1; j < N + 1; j++) {

			// Print the value at Adj[i][j]
			printf("%d ", Adj[i][j]);
		}
		printf("\n");
	}
}

void FindSpan(int Adj[][N + 1], int span[][N + 1], int trav[N + 1])
{
	for (int i = 0; i < N + 1; i++) {

		for (int j = 0; j < N + 1; j++) {
			span[i][j] = 0;
		}
	}

	for (int i = 0; i < N + 1; i++)
	{
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
	int v1 = 0, v2 = 0, n = 1;
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
							cycles[i][m][l] == 0;
							cycles[i][l][m] == -1;
						}
					}
				} 
			}
	}

	}
}

// Function to multiply two matrices
void matrixMultiply(int N, int mat1[][N], int mat2[][N], int result[][N]) {
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

// Driver Code
int main()
{

	// Number of vertices
	N = 5;

	// Given Edges
	int arr[][2]
		= { { 1, 2 }, { 1, 3}, { 2, 3 }, 
			{ 4, 5 }, { 1, 5 } };

	// Number of Edges
	M = sizeof(arr) / sizeof(arr[0]);

	// For Adjacency Matrix
	int Adj[N + 1][N + 1], span[N + 1][N + 1], disclinks[N + 1][N + 1], cycles[N + 1][N + 1], testcycle[N + 1][N + 1];
	int trav[N + 1];

	// Function call to create
	// Adjacency Matrix
	createAdjMatrix(Adj, arr);

	// Print Adjacency Matrix
	printAdjMatrix(Adj);

	FindSpan(Adj, span, trav);

	printf("Span:\n");
	printAdjMatrix(span);
	/*
	for (int i = 0; i < N + 1; i++) 
	{
		for (int j = 0; j < N + 1; j++) 
		{
			disclinks[i][j] = Adj[i][j] - span[i][j];
			cycles[i][j] = span[i][j];
		}
	}

	int k = 0;
	for (int i = 0; i < N; i++) 
	{
		for (int j = i + 1; j < N + 1; j++) 
		{
			if(disclinks[i][j] == 1)
			{
				k++;
				cycles[i][j] = 1;
				cycles[j][i] = 1;
			}
		}
	}*/
	// k is the number of fundamental cycles


	int cyc[][2]
		= { { 1, 2 }, { 2, 3}, { 3, 4 }, {1, 4}};

	createAdjMatrix(testcycle, cyc);

	printf("TestCycle:\n");
	printAdjMatrix(testcycle);

	loop_orientation(testcycle);
	printf("TestCycle after loop orientation: \n");
	printAdjMatrix(testcycle);
}

