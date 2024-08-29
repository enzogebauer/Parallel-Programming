#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#define SIZE_MATRIX 10

int main(int argc, char **argv)
{
  int n = atoi(argv[1]);
  int block_size = atoi(argv[2]);
  int matrix[SIZE_MATRIX][SIZE_MATRIX];
  int k[5] = {10, 20, 30, 40, 50}; 
  // adicionando as variáveis k3 e k4 para poder multiplicar os valores da matriz
  int i, j, row, column;

  for(i = 0; i < n; i++)
  {
    for(j = 0; j < n; j++)
    {
      matrix[i][j] = 5;
      printf("%d\t", matrix[i][j]);
    }
    printf("\n");
  }

  printf("\n\n");

  omp_set_num_threads(5);
  #pragma omp parallel private(row, column)
  {
    int id = omp_get_thread_num();
 printf("Ola Mundo desde a thread %d \n", id);
    if(id == 0)
    {
      for(row = 0; row < n; row++)
        for(column = 0; column < block_size; column++)
          matrix[row][column] *= k[id];
    }

    if(id == 1)
    {
      for(row = 0; row < n; row++)
        for(column = block_size; column < 2 * block_size; column++)
          matrix[row][column] *= k[id];
    }

    if(id == 2)
    {
        for(row = 0; row < n; row++)
        for(column = 2 * block_size; column < 3 * block_size; column++)
          matrix[row][column] *= k[id];
    }

    if(id == 3)
    {
        for(row = 0; row < n; row++)
        for(column = 3 * block_size; column < 4 * block_size; column++)
          matrix[row][column] *= k[id];
    }
    if (id == 4)
    {
        for(row = 0; row < n; row++)
        for(column = 4 * block_size; column < 5 * block_size; column++)
          matrix[row][column] *= k[id];
    }

   }
   //pragma omp

  for(i = 0; i < n; i++)
  {
    for(j = 0; j < n; j++)
      printf("%d\t", matrix[i][j]);
    printf("\n");
  }

  return 0;
}

// What does the code do from the compilation and execution of the previous code?
// Utilizes 5 thread to run the code in a prallel way

// How would it be possible to extend the code so that the five threads perform asynchronous tasks?
// adding the if statement to the parallel code and adding the new values of k to the matrix
