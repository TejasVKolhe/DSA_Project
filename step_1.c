#include <stdio.h>
#include <stdbool.h>
#define N 8

void FindSpan(int Adj[][N + 1], int span[][N + 1], int trav[N + 1]);
void createAdjMatrix(int Adj[][N + 1], int arr[][2]);
void printAdjMatrix(int Adj[][N + 1]);
int count_discarded(int Adj[][N + 1], int span[][N + 1], int discarded[][2]);
void pruning(int span[][N + 1], int discarded[][2], int cycles[][N + 1][N + 1], int number_of_discarded);
bool DFS(int u, int v, int parent, bool visited[], int span[][N + 1], int cycle[], int *cycle_index);
void findlink(int cycle[][N + 1], int *v1, int *v2);

static int M;

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



