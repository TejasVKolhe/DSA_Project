#include<stdio.h>
#include<stdbool.h>
// #define N 8
// #define N 6

#include"step_4.c"


static int M;


int main()
{
    int arr[][2] = { { 1, 2 }, { 1, 6}, { 2, 3 }, {2, 5}, {3, 4}, { 4, 5 }, { 5, 6} };

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
            if (bc[i][j] == 0 || vr[j][i] == 0)
                printf(" 0.00 ");
            else if (i > j)
            {
                printf(" %.2f ", vr[i][j]);
            }
            else
            {
                printf(" %.2f ", -vr[i][j]);
            }
        }
        printf("\n");
    }

    return 0;
}
