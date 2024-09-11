#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv)
{
  double coef[4], total, x = 12, received_result;
  char c;

  char operationsRec;
  int numberOfProcessors, id, to, from, tag = 1000;
  int result, value;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numberOfProcessors);
  MPI_Comm_rank(MPI_COMM_WORLD, &id);
  MPI_Status status;

  switch (id)
  {
  case 0: // master
    int i = 0;
    for (i = 1; i <= 4; i++)
    {
      coef[i - 1] = i;
    }

    for (to = 1; to < numberOfProcessors; to++)
    {
      MPI_Send(&x, 1, MPI_INT, to, tag, MPI_COMM_WORLD);
    }
    break;
  default: // workers
    MPI_Recv(&x, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
    index = 3 - id;
    result[index] = coef[index];
    for (i = 1; i <= id; i++)
    {
      result[index] *= x;
    }
    if (id == 1)
    {
      result[index] += coef[3];
    }
  }
  MPI_Send(&value, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
  MPI_Send(&operationsRec, 1, MPI_CHAR, 0, tag, MPI_COMM_WORLD);

  if (id == 0)
  {
    for (from = 1; from < numberOfProcessors; from++)
    {
      MPI_Recv(&received_result, 1, MPI_DOUBLE, from, tag, MPI_COMM_WORLD, &status);
      MPI_Recv(&received_index, 1, MPI_INT, from, tag, MPI_COMM_WORLD, &status);
      result[received_index] = received_result;
      printf("(%d) = %lf\n", received_index, result[received_index]);
      total += received_result;
    }

    if (total > 0)
      printf("Total: %.5lf\n", total);
  }

  MPI_Finalize();
  return 0;
}