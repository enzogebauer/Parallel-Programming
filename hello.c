#include <stdio.h>
#include <omp.h>

int main (int argc, char **argv)  {

 int nthreads, tid;

 #pragma omp parallel private(tid)     /* Define o grupo de threads*/
 {
   tid = omp_get_thread_num();         /* Obtem o identificador da thread*/
   nthreads = omp_get_num_threads();   /* Obtem o numero de threads*/
   printf("Ola Mundo desde a thread %d de um total de %d threads\n", tid, nthreads);
 }                                     /* Sincronizacao das threads*/

 return 0;
}
