#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void test_mean1(double *xymean, double* xy, int n)
{
    int i;
    double xsum = 0;
    double ysum = 0;
    for (i = 0; i < 2*n; i += 2) {
        xsum += xy[i+0];
        ysum += xy[i+1];
    }
    xymean[0] = xsum/n;
    xymean[1] = ysum/n;
}

void test_mean2(double *xymean, double* xy, int n)
{
    int i;
    double xsum = 0;
    double ysum = 0;
    for (i = 0; i < 2*n; i += 2) 
        xsum += xy[i+0];
    for (i = 0; i < 2*n; i += 2)
        ysum += xy[i+1];
    xymean[0] = xsum/n;
    xymean[1] = ysum/n;
}

void test_mean3(double *xymean, double* xy, int n)
{
    int i;
    double xsum = 0;
    double ysum = 0;
    for (i = 0; i < n; ++i)
        xsum += xy[i];
    for (i = n; i < 2*n; ++i)
        ysum += xy[i];
    xymean[0] = xsum/n;
    xymean[1] = ysum/n;
}

int main()
{
    int i;
    int N = 1000000;
/*    int trials = 100; */
    int trials = 50; 
    int tstart, tstop;
    double xymean[2];
    double* xy = (double*) malloc(2 * N * sizeof(double));

    for (i = 0; i < 2*N; ++i)
        xy[i] = random();

    tstart = clock();
    for (i = 0; i < trials; ++i)
        test_mean1(xymean, xy, N);
    tstop = clock();
    printf("Version 1: %e\n", (double) (tstop-tstart)/trials/CLOCKS_PER_SEC);

    tstart = clock();
    for (i = 0; i < trials; ++i)
        test_mean2(xymean, xy, N);
    tstop = clock();
    printf("Version 2: %e\n", (double) (tstop-tstart)/trials/CLOCKS_PER_SEC);

    tstart = clock();
    for (i = 0; i < trials; ++i)
        test_mean3(xymean, xy, N);
    tstop = clock();
    printf("Version 3: %e\n", (double) (tstop-tstart)/trials/CLOCKS_PER_SEC);

    free(xy);
}
