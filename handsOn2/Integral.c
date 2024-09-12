#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

/* f(x) function from which the integral will be calculated. */
double f(double x) { return 100 * x + sin(2 * x * M_PI); }

double integral_seq(double a, double b, int n) 
{
    double h, s = 0, result;
    int i;
    h = (b - a) / n;
    for (i = 0; i < n; i++) {
    s += f(a + h * (i + 0.5));
    }
    result = h * s;
    return result;
}

/* Calculates the integral of the function between points a and b. */
double integral_parallel(double a, double b, int n)
{
    double h = (b - a) / n;
    double s = 0.0;
    int i;

    #pragma omp parallel for private(i) shared(a, b, h, n) reduction(+:s)
    for (i = 0; i < n; i++) {
        s += f(a + h * (i + 0.5));
    }

    return h * s;
}

int main(int argc, char **argv)
{
    int steps = atoi(argv[1]);

    double sict0, sictf, pict0, pictf, sict, pict, speedup;

    // Sequential integral calculation
    sict0 = omp_get_wtime(); // Start time
    double resultSeq = integral_seq(0, 1, steps);
    sictf = omp_get_wtime(); // End time
    sict = sictf - sict0;

    printf("Sequential time: %f seconds\n", sict);

    // Parallel integral calculation
    pict0 = omp_get_wtime(); // Start time
    double resultParallel = integral_parallel(0, 1, steps);
    pictf = omp_get_wtime(); // End time
    pict = pictf - pict0;

    printf("Parallel time: %f seconds\n", pict);

    // Calculate speedup
    speedup = sict / pict;
    printf("Speedup: %f\n", speedup);

    return 0;
}
