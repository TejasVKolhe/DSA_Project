#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<stdbool.h>
#define N 8



//Testing 3 loop pruning
void FindSpan(int Adj[][N + 1], int span[][N + 1], int trav[N + 1]);
void createAdjMatrix(int Adj[][N + 1],int arr[][2]);
void printAdjMatrix(int Adj[][N + 1]);
int count_discarded(int Adj[][N+1], int span[][N+1], int discarded[][2]);
void pruning(int span[][N+1], int discarded[][2], int cycles[][N+1][N+1], int number_of_discarded);
bool DFS(int u, int v, int parent, bool visited[], int span[][N + 1], int cycle[], int *cycle_index);
void findlink(int cycle[][N + 1], int *v1, int *v2);
void loop_orientation(int cycle[][N + 1]);
void orientall_loops(int cycles[][N + 1][N + 1], int numcycles);
void init_cycles(int cycles[][N+1][N+1], int number_of_discarded);

static int M;

int main() 
{
    int arr[][2] = { { 1, 2 }, { 1, 8}, { 2, 3 }, {2, 7}, {3, 4}, { 4, 5 }, {4, 7}, { 5, 6}, {6, 7}, {7, 8}};

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
	int number_of_discarded = count_discarded(Adj, span,array_of_discarded_edges);
    
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

    int cycles[number_of_discarded][N+1][N+1];       //Will store all cycles in their adjacency matrix form, initialised to 0, number of individual cycles will be equal to number of discarded links
    init_cycles(cycles, number_of_discarded);        //Initialize cycles
    

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

void init_cycles(int cycles[][N+1][N+1], int number_of_discarded)
{
    for(int i = 0; i < number_of_discarded; i++)
    {
        for(int j = 0; j < N+1; j++)
        {
            for(int k = 0; k < N+1; k++)
            {
                cycles[i][j][k] = 0;
            }
        }
    }
}

//Find loops function should insert each discarded link into the spanning tree and add to the 'loops' array the cycle which is completed by adding that link 
//This is done using patons algorithm
void pruning(int span[][N + 1], int discarded[][2], int cycles[][N+1][N+1], int number_of_discarded)
{
    for(int i = 0; i < number_of_discarded; i++)
    {
        span[discarded[i][0]][discarded[i][1]] = span[discarded[i][1]][discarded[i][0]] = 1;

        /*Testing of adding edges to spanning tree
        printf("\nSpan with added edge:\n");
        printAdjMatrix(span);
        printf("\n")
        */

        bool visited[N + 1] = {false};
        int cycle[N+1] = {0};
        int cycle_index = 0;
        
        //DFS for finding cycle
        //printf("\nCycle found by adding edge %d->%d:\n", discarded[i][0], discarded[i][1]);
        DFS(discarded[i][0], discarded[i][1], -1, visited, span, cycle, &cycle_index);

        //Adding the adjacency matrix of each loop to the cycles array
        for(int j = 0; j < N; j++)
        {
            if(cycle[j+1] == 0)                             //The last node will be connected to the first node of the cycle
            {
                cycles[i][cycle[j]][cycle[0]] = 1;          //Mark the elements of adjacency matrix of each of the cycle, each consecutive pair is an edge, which is to be marked in adjacency matrix
                cycles[i][cycle[0]][cycle[j]] = 1;
                break;                                      //Break here because all the nodes in the loop are marked in its respective adjacency matrix(Last is paired with first)
            }
            else
            {
                cycles[i][cycle[j]][cycle[j+1]] = 1;
                cycles[i][cycle[j+1]][cycle[j]]= 1;
            }
           
        }

        /*Test printing contents of cycle array
        for(int k = 0; k < N+1; k++)
        {
            printf("%d ", cycle[k]);
        }
        printf("\n");
        */

        //Restore the adjacent matrix of the spanning tree
        span[discarded[i][0]][discarded[i][1]] = span[discarded[i][1]][discarded[i][0]] = 0;
    }
}

bool DFS(int u, int v, int parent, bool visited[], int span[][N + 1], int cycle[], int *cycle_index)
{
    visited[u] = true;
    cycle[*cycle_index] = u;
    (*cycle_index)++;

    if(u == v && (*cycle_index) > 1) //cycle is found, stop DFS
    {
        return true; 
    }

    for(int i = 1; i < N+1; i++) 
    {
        if(span[u][i] && i != parent && !visited[i])  //This checks if 2 nodes are adjacent, prevents to go back to parent in DFS, and prevents to go back to node which is already visited
        {
            if(DFS(i, v, u, visited, span, cycle, cycle_index)) 
            {
                return true; //If a cycle if found in a DFS traversal, then further traversal has to be stopped
            }
        }
    }

    //If no cycl is found then backtrack
    if(!visited[v]) 
    {
        (*cycle_index)--;
        visited[u] = false;
        cycle[*cycle_index] = 0;
    }
    return false; //No cycle in this path, continue 
}