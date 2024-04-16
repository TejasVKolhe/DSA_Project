#include<stdio.h>
#include<stdbool.h>
#define N 6
#define MAX_DISCARDED_EDGES 2 
#define MAX_VERTICES_IN_CYCLE 4 


void FindSpan(int Adj[][N + 1], int span[][N + 1], int trav[N + 1]);
void createAdjMatrix(int Adj[][N + 1],int arr[][2]);
void printAdjMatrix(int Adj[][N + 1]);
int count_discarded(int Adj[][N+1], int span[][N+1], int discarded[][2]);
void pruning(int span[][N+1], int discarded[][2], int loops[MAX_DISCARDED_EDGES][MAX_VERTICES_IN_CYCLE], int number_of_discarded);
void DFS(int u, int v, int parent, bool visited[], int span[][N + 1], int cycle[], int *cycle_index);
void findlink(int cycle[][N + 1], int *v1, int *v2);

static int M;

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

