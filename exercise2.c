#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int rank, size;
    long long n = 10000000;
    long long start, end;
    long long local_sum = 0;
    long long total_sum = 0;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    start = (n / size) * rank + 1;

    if (rank == size - 1)
        end = n;
    else
        end = (n / size) * (rank + 1);

    for (long long i = start; i <= end; i++)
        local_sum += i;

    MPI_Reduce(&local_sum, &total_sum, 1, MPI_LONG_LONG,
               MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0)
        printf("Total sum = %lld\n", total_sum);

    MPI_Finalize();

    return 0;
}
