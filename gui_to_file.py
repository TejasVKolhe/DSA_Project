def initmatrix(dimension):
    n = dimension * dimension
    matrix = [[0] * n]* n
    write_file(matrix)
    # for row in matrix:
    #     print(row)
    

def write_file(matrix):
    file = open("circuit.txt", "w")
    for row in matrix:
        file.write(str(row))
        file.write('\n')
    
