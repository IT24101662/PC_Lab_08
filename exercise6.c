#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

int main(int argc, char *argv[])
{
    int rank, size;
    long long total_points = 10000000;
    long long local_points;
    long long local_inside = 0;
    long long total_inside = 0;
    MPI_Status status;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    local_points = total_points / size;

    srand(time(NULL) + rank);

    for (long long i = 0; i < local_points; i++)
    {
        double x = (double)rand() / RAND_MAX;
        double y = (double)rand() / RAND_MAX;

        if (x * x + y * y <= 1.0)
            local_inside++;
    }

    if (rank == 0)
    {
        total_inside = local_inside;

        for (int i = 1; i < size; i++)
        {
            long long received_inside;

            MPI_Recv(&received_inside, 1, MPI_LONG_LONG,
                     MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);

            total_inside += received_inside;
        }

        double pi = 4.0 * total_inside / total_points;

        printf("Estimated Pi = %.10f\n", pi);
    }
    else
    {
        MPI_Send(&local_inside, 1, MPI_LONG_LONG,
                 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
}
