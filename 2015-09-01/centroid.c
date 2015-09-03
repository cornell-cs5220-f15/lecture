#include "centroid.h"

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

