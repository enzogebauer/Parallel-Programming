#include <stdio.h>
#include <math.h>
#include <limits.h>
#include <omp.h>

typedef unsigned long long big_integer;
#define BIGGEST_INTEGER ULLONG_MAX
int num_thread;

int is_prime_seq(big_integer n)
{
  int result;
  big_integer sq_root;

  result = (n % 2 != 0 || n == 2);

  if (result)
  {
    sq_root = sqrt(n);

    for (big_integer i = 3; result && i <= sq_root; i += 2)
      result = n % i != 0;
  }

  return result;
}
int is_prime_parallel(big_integer n)
{
  int result;
  big_integer sq_root;

  result = (n % 2 != 0 || n == 2);

  if (result)
  {
    sq_root = sqrt(n);
//#pragma omp parallel for reduction(&& : result)
#pragma omp parallel for
    for (big_integer i = 3; i <= sq_root; i += 2)
    {
      result = n % i != 0;
      if (i == 3)
      {
        num_thread = omp_get_num_threads();
      }
    }
  }

  return result;
}

int main(int argc, char **argv)
{
  big_integer n;
  double bps0, bpsf, bpst, bpp0, bppf, bppt, speedUp;
  bps0 = omp_get_wtime();
  for (n = BIGGEST_INTEGER; !is_prime_seq(n); n -= 2)
  {
  }
  bpsf = omp_get_wtime();
  printf("%llu\n", n);


  bpp0 = omp_get_wtime();
  for (n = BIGGEST_INTEGER; !is_prime_parallel(n); n -= 2)
  {
  }
  bppf = omp_get_wtime();
  printf("%llu\n", n);

  bpst = bpsf - bps0;
  bppt = bppf - bpp0;

  speedUp = bpst / bppt;
  printf("Sequencial Time: %lf   Parallel Time: %lf   SpeedUp: %lf\n", bpst, bppt, speedUp);

  return 0;
}
