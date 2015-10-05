#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


void interact(double* result, double x, double y)
{
    *result += fabs(x-y);
}


int main(int argc, char** argv)
{
    int nper = 2;
    int rank, size;
    int next, prev;
    int i, j, phase;

    double* result   = (double*) malloc(nper * sizeof(double));
    double* my_data  = (double*) malloc(nper * sizeof(double));
    double* curr_buf = (double*) malloc(nper * sizeof(double));
    double* recv_buf = (double*) malloc(nper * sizeof(double));
    double* tmp;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    next = (rank+1) % size;
    prev = (rank+size-1) % size;

    /* Populate local arrays */
    memset(result, 0, nper * sizeof(double));
    for (i = 0; i < nper; ++i)
        my_data[i] = rank + 0.1 * i;
    
    /* Compute interactions among local data */
    for (i = 0; i < nper; ++i)
        for (j = 0; j < nper; ++j)
            if (i != j)
                interact(result+i, my_data[i], my_data[j]);

    memcpy(curr_buf, my_data, nper * sizeof(double));
    for (phase = 0; phase < size-1; ++phase) {

        /* Send current buffer onward and receive into recv_buff */
        MPI_Sendrecv(curr_buf, nper, MPI_DOUBLE, next, phase,
                     recv_buf, nper, MPI_DOUBLE, prev, phase,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        /* Swap current and received buffer */
        tmp = curr_buf;
        curr_buf = recv_buf;
        recv_buf = tmp;

        /* Diagnostic message */
        if (rank == 0)
            printf("Interact %g-%g vs %g-%g\n",
                   my_data[0], my_data[1],
                   curr_buf[0], curr_buf[nper-1]);
            
        /* Compute interactions */
        for (i = 0; i < nper; ++i)
            for (j = 0; j < nper; ++j)
                interact(result+i, my_data[i], curr_buf[j]);
    }

    free(curr_buf);
    free(recv_buf);
    free(my_data);
    free(result);

    MPI_Finalize();
    return 0;
}
