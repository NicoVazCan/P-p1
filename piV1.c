#include <stdio.h>
#include <math.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int i, done = 0, n, nproc, numprocs, rank;
    double PI25DT = 3.141592653589793238462643;
    double pi, h, sum, sumi, x;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);


    while(!done)
    {   if(rank == 0)
        {   printf("Enter the number of intervals: (0 quits) \n");
            scanf("%d",&n);

            for(i = 1; i < numprocs; i++)
            {   MPI_Send(&n, 1, MPI_INT, i, 0, MPI_COMM_WORLD); }
        } else
        {   MPI_Recv(&n, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); }
    
        if (n == 0) break;
  
        h   = 1.0 / (double) n;
        sum = 0.0;
        for(i = 1+rank; i <= n; i += numprocs) 
        {   x = h * ((double)i - 0.5);
            sum += 4.0 / (1.0 + x*x);
        }

        if(rank == 0)
        {   for(i = 1; i < numprocs; i++)
            {   MPI_Recv(&sumi, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                sum += sumi;
            }

            pi = h * sum;
            printf("pi is approximately %.16f, Error is %.16f\n", pi, fabs(pi - PI25DT));
        } else
        {   MPI_Send(&sum, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD); }
    }


    MPI_Finalize();
    return 0;
}
