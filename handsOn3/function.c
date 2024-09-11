% % writefile function.c
#include <stdio.h>
#include <mpi.h>

    int
    main(int argc, char **argv)
{
  double coef[4], result[4] = {0}, total = 0, x = 10, received_result;
  int numberOfProcessors, id, index, i, to, from, tag = 1000, received_index;

  // Initialize coef values
  for (i = 1; i <= 4; i++)
  {
    coef[i - 1] = i;
  }

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numberOfProcessors);
  MPI_Comm_rank(MPI_COMM_WORLD, &id);
  MPI_Status status;

  switch (id)
  {
  case 0: // Master
    // Send x to all workers
    for (to = 1; to < numberOfProcessors; to++)
    {
      MPI_Send(&x, 1, MPI_DOUBLE, to, tag, MPI_COMM_WORLD);
    }
    break;

  default: // Workers
    // Receive x from master
    MPI_Recv(&x, 1, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, &status);
    index = 3 - id;              // Calculate indexed value based on id
    result[index] = coef[index]; // Initialize result with corresponding coef value

    // Multiply result by x id times
    for (i = 1; i <= id; i++)
    {
      result[index] *= x;
    }

    // If id == 1, add coef[3]
    if (id == 1)
    {
      result[index] += coef[3];
    }

    break;
  }

  // Send result and index back to the Master
  MPI_Send(&result[index], 1, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD);
  MPI_Send(&index, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);

  if (id == 0)
  {
    // Master receives results from Workers
    for (from = 1; from < numberOfProcessors; from++)
    {
      MPI_Recv(&received_result, 1, MPI_DOUBLE, from, tag, MPI_COMM_WORLD, &status);
      MPI_Recv(&received_index, 1, MPI_INT, from, tag, MPI_COMM_WORLD, &status);
      result[received_index] = received_result;
      printf("(%d) = %lf\n", received_index, result[received_index]);
      total += received_result;
    }

    // Print total sum of results
    if (total > 0)
      printf("Total: %.5lf\n", total);
  }

  MPI_Finalize();
  return 0;
}
