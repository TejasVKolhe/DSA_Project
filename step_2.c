#include<stdio.h>
#include<stdbool.h> 
#include"step_1.c"

void loop_orientation(int cycle[][N + 1]);
void orientall_loops(int cycles[][N + 1][N + 1], int numcycles);
void init_cycles(int cycles[][N + 1][N + 1], int number_of_discarded);

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

