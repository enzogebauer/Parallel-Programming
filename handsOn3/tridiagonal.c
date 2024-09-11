//%%writefile tridiagonal.c
#include <stdio.h>
#define ORDER 4
#include <mpi.h>

void printMatrix(int m[][ORDER])
{
  int i, j;
  for (i = 0; i < ORDER; i++)
  {
    printf("| ");
    for (j = 0; j < ORDER; j++)
    {
      printf("%3d ", m[i][j]);
    }
    printf("|\n");
  }
  printf("\n");
}

int main(int argc, char **argv)
{
  int k[3] = {100, 200, 300};
  int matrix[ORDER][ORDER] = {0}, received_matrix[ORDER][ORDER], i, j;
  int numberOfProcessors, id, to, from, tag = 1000;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numberOfProcessors);
  MPI_Comm_rank(MPI_COMM_WORLD, &id);
  MPI_Status status;

  if (id == 0)
  {
    // Process 0 sends the initial matrix to the other processes
    for (to = 1; to < numberOfProcessors; to++)
    {
      MPI_Send(&matrix, ORDER * ORDER, MPI_INT, to, tag, MPI_COMM_WORLD);
    }

    // Process 0 receives the submatrices from the other processes
    for (from = 1; from < numberOfProcessors; from++)
    {
      MPI_Recv(&received_matrix, ORDER * ORDER, MPI_INT, from, tag, MPI_COMM_WORLD, &status);

      // Combine the received submatrix into the final matrix
      for (i = 0; i < ORDER; i++)
      {
        for (j = 0; j < ORDER; j++)
        {
          if (received_matrix[i][j] != 0) // Avoid overwriting existing values
          {
            matrix[i][j] = received_matrix[i][j];
          }
        }
      }
    }

    // Print the final matrix in process 0
    printf("Final matrix assembled by process 0:\n");
    printMatrix(matrix);
  }
  else
  {
    // Processes 1, 2, and 3 receive the initial matrix and perform their tasks
    MPI_Recv(&matrix, ORDER * ORDER, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);

    switch (id)
    {
    case 1:
      for (i = 0; i < ORDER; i++)
        for (j = 0; j < ORDER; j++)
          if (i == j)
            matrix[i][j] = i + j + 1 + k[0]; // Main diagonal
      break;
    case 2:
      for (i = 0; i < ORDER; i++)
        for (j = 0; j < ORDER; j++)
          if (i == (j + 1))
          {
            matrix[i][j] = i + j + 1 + k[1];    // Subdiagonal
            matrix[j][i] = matrix[i][j] + k[2]; // Superdiagonal
          }
      break;
    case 3:
      for (i = 0; i < ORDER; i++)
      {
        for (j = 0; j < ORDER; j++)
        {
          if (i < j - 1 || i > j + 1)
          {
            matrix[i][j] = 0; // Zero outside the super and subdiagonal
          }
        }
      }
      break;
    }
    printf("Process %d - Matrix after the task:\n", id);
    printMatrix(matrix);

    // Processes 1, 2, and 3 send the submatrices back to process 0
    MPI_Send(&matrix, ORDER * ORDER, MPI_INT, 0, tag, MPI_COMM_WORLD);
  }

  MPI_Finalize();
  return 0;
}