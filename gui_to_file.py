import numpy as np
from math import sqrt

def initmatrix(dimension):
    n = dimension * dimension
    matrix = np.array(([0]*n)*n)
    # print(matrix)
    write_file(matrix)
    return matrix
    # for row in matrix:
    #     print(row)
    

def write_file(matrix):
    file = open("circuit.txt", "w")
    print(len(matrix))
    sqrt_len = int(sqrt(len(matrix)))

    for i in range(sqrt_len):
        for j in range(sqrt_len):
            file.write(str(matrix[i*sqrt_len + j]))
            file.write(' ')
        file.write('\n')
