#include <stdio.h>
#include <math.h>
#include <omp.h>

typedef unsigned long long big_integer;
#define TOP_LIMIT 20000000ULL
int num_thread;
int is_prime(big_integer n)
{
  int p;
  big_integer i, s;

  p = (n % 2 != 0 || n == 2);

  if (p)
  {
    s = sqrt(n);

    for (i = 3; p && i <= s; i += 2)
      if (n % i == 0)
        p = 0;
  }

  return p;
}

int count_primes_sequential(int primes){
    big_integer i;
    
    for (i = 3; i <= TOP_LIMIT; i += 2)
    if (is_prime(i)){
        primes++;
    }
    return primes;
}
int count_primes_parallel(int primes)
{
    big_integer i;
    #pragma omp parallel for reduction(+:primes)
    for (i = 3; i <= TOP_LIMIT; i += 2)
    if (is_prime(i)){
        primes++;
    }
    return primes;
}

int main(int argc, char **argv)
{
  big_integer i, primes = 2;
  double cps0, cpsf, cpst, cpp0, cppf, cppt, speedUp;
  
  cps0 = omp_get_wtime();
  primes = count_primes_sequential(primes);
  cpsf = omp_get_wtime();
  
  cpp0 = omp_get_wtime();
  primes = count_primes_parallel(primes);
  cppf = omp_get_wtime();
  
  cpst = cpsf - cps0;
  cppt = cppf - cpp0;
  speedUp = cpst / cppt;
  printf("Sequencial Time: %lf   Parallel Time: %lf   SpeedUp: %lf\n", cpst, cppt, speedUp);

  return 0;
}
