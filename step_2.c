#include<stdio.h>
#include<stdbool.h>
#define N 6
#define MAX_DISCARDED_EDGES 2 
#define MAX_VERTICES_IN_CYCLE 4 

void loop_orientation(int cycle[][N + 1]);
void orientall_loops(int cycles[][N + 1][N + 1], int numcycles);
void add_edges(int loop[MAX_VERTICES_IN_CYCLE], int edges_in_cycle[MAX_VERTICES_IN_CYCLE][2]);


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
