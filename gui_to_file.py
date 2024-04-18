import numpy as np

def initmatrix(n):
    matrix = np.zeros((n, n))
    Rmatrix = np.zeros((n, n))
    Vmatrix = np.zeros((n, n))
    return matrix, Rmatrix, Vmatrix

def insertdata(data, button_edge_set):
    matrix = data[0]
    Rmatrix = data[1]
    Vmatrix = data[2]

    for edges in button_edge_set:
            m = edges[2]
            n = edges[3]
            if m or n:
                # print(m,n)
                matrix[m][n] = 1
                matrix[n][m] = 1

                if edges[0] == 'Resistor':
                    Rmatrix[m][n] = edges[1]
                    Rmatrix[n][m] = edges[1]
                
                if edges[0] == 'Battery':
                    Vmatrix[m][n] = edges[1]
                    Vmatrix[n][m] = edges[1] * (-1)
        

    return [matrix, Rmatrix, Vmatrix]


def remove_zero_rows_and_cols(data):
    matrix = data[0]
    Rmatrix = data[1]
    Vmatrix = data[2]

    # Get the number of rows and columns in the matrix
    num_rows, num_cols = matrix.shape

    # Initialize lists to store indices of rows and columns to keep
    rows_to_keep = []
    cols_to_keep = []

    # Check each row to see if it contains any non-zero elements
    for row_idx in range(num_rows):
        if not np.all(matrix[row_idx] == 0):
            rows_to_keep.append(row_idx)

    # Check each column to see if it contains any non-zero elements
    for col_idx in range(num_cols):
        if not np.all(matrix[:, col_idx] == 0):
            cols_to_keep.append(col_idx)

    # Use boolean indexing to filter the matrix based on rows and columns to keep
    result_matrix = matrix[rows_to_keep][:, cols_to_keep]
    result_Rmatrix = Rmatrix[rows_to_keep][:, cols_to_keep]
    result_Vmatrix = Vmatrix[rows_to_keep][:, cols_to_keep]

    return [result_matrix, result_Rmatrix, result_Vmatrix]

def write_into_files(data):
    matrix = data[0]
    Rmatrix = data[1]
    Vmatrix = data[2]

    matrix_to_file(matrix, "adjacency_matrix.txt")
    matrix_to_file(Rmatrix, "resistor_matrix.txt")
    matrix_to_file(Vmatrix, "voltage_matrix.txt")



def matrix_to_file(matrix, filename):
    # Get the dimension of the matrix
    dimension = matrix.shape[0]

    # Open the file in write mode, creating it if it doesn't exist
    with open(str(filename), "w") as file:
        # Write the dimension to the first line of the file
        file.write(str(dimension) + "\n")

        # Write each row of the matrix to the file
        for row in matrix:
            row_str = "\t".join(map(str, row))  # Convert row elements to strings
            file.write(row_str + "\n")


def add_zero_row_and_column(matrix):
    num_rows, num_cols = matrix.shape
    # Create a new matrix with one additional row and one additional column
    new_matrix = np.zeros((num_rows + 1, num_cols + 1))
    # Copy the original matrix data into the new matrix, leaving the first row and column as zeros
    new_matrix[1:, 1:] = matrix
    return new_matrix