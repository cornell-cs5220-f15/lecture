#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include "centroid.h"


void time_centroid(const char* name,
                   void (*mean)(double*, double*, int), 
                   double* xy, int N)
{
    int i;
    int trials = 100; 
    double tstart, tstop;
    double xymean[2] = {0, 0};
    double per_trial;
    double gflops;
    tstart = omp_get_wtime();
    for (i = 0; i < trials; ++i)
        mean(xymean, xy, N);
    tstop = omp_get_wtime();
    per_trial = (tstop-tstart)/trials;
    gflops = (2*N)/per_trial/1e9;
    printf("Result: %g %g\n", xymean[0], xymean[1]);
    printf("Version %s: %e (%g GFLop/s)\n", name, per_trial, gflops);
}


int main()
{
    int i;
    int N = 20000000;
    double* xy = (double*) malloc(2 * N * sizeof(double));

    for (i = 0; i < 2*N; ++i)
        xy[i] = random();

    time_centroid("1", test_mean1, xy, N);
    time_centroid("2", test_mean2, xy, N);
    time_centroid("3", test_mean3, xy, N);

    free(xy);
    return 0;
}
