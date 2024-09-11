#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
int  *A;
int  *B; 
int  *C;
int size;
int num_threads;

void writeResultsToFile(int size, double sequential_time, double parallel_time, double parallel_collapse_reduction_time, double speedup) {
    FILE *result_file = fopen("resultFile.txt", "a");
    if (result_file == NULL) {
        perror("Erro ao abrir o arquivo de saída");
        return;
    } 

    fprintf(result_file, "%d %lf %lf %lf %f\n", size, sequential_time, parallel_time, parallel_collapse_reduction_time, speedup);
    if(size == 1000) fprintf(result_file, "\n");

    fclose(result_file);
}

void initializeMatrix(int *matrix, int size)
{
  for(int i = 0; i < size; i++)
    for(int j = 0; j < size; j++)
      matrix[i * size + j] = rand() % (10 - 1) * 1;
}

void multiply_sequential(){
  int i, j, k;
  for(i = 0; i < size; i++)
    for(j = 0; j < size; j++)
      for(k = 0; k < size; k++)
        C[i * size + j] += A[i * size + k] * B[k * size + j];
}

void multiply_parallel(){
  int i, j, k;
  #pragma omp parallel for private(i, j, k)
  for(i = 0; i < size; i++){
    for(j = 0; j < size; j++){
      if (i == 0 && j == 0) num_threads = omp_get_num_threads();
      for(k = 0; k < size; k++)
        C[i * size + j] = A[i * size + k] * B[k * size + j];
    }
  } 
}

void multiply_parallel_collapse_reduction(){
  int i, j, k;
  #pragma omp parallel for collapse(2) private(i, j, k)
  for(i = 0; i < size; i++){
    for(j = 0; j < size; j++){
      if (i == 0 && j == 0) num_threads = omp_get_num_threads();
      int ans = 0;
    #pragma omp simd reduction(+ : ans)
      for(k = 0; k < size; k++){
        ans += A[i * size + k] * B[k * size + j];
      }
      C[i * size + j] = ans;
    }
  } 
}

int main (int argc, char **argv)
{
  size = atoi(argv[1]);  
  int i, j, k;
  double sequential_start, sequential_end, parallel_start, parallel_end, parallel_cr_start, parallel_cr_end; 

  A = (int *) malloc (sizeof(int)*size*size);
  B = (int *) malloc (sizeof(int)*size*size);
  C = (int *) malloc (sizeof(int)*size*size);

  initializeMatrix(A, size);
  initializeMatrix(B, size);

  // Sequential
  sequential_start = omp_get_wtime();
  multiply_sequential();
  sequential_end = omp_get_wtime();
  
  // Parallel
  parallel_start = omp_get_wtime();
  multiply_parallel();
  parallel_end = omp_get_wtime();

  // Parallel collapse reduction
  parallel_cr_start = omp_get_wtime();
  multiply_parallel_collapse_reduction();
  parallel_cr_end = omp_get_wtime();
  
  double sequential_time = sequential_end - sequential_start; // Sequencial time
  double parallel_time = parallel_end - parallel_start; // Parallel time
  double parallel_collapse_reduction_time = parallel_cr_end - parallel_cr_start; // Parallel collapse reduction time

  double speedup = sequential_time / parallel_collapse_reduction_time;

  writeResultsToFile(size, sequential_time, parallel_time, parallel_collapse_reduction_time, speedup);
  printf("Speedup: %f\n", speedup);

  return 0;
}

