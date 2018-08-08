#include <stdio.h>
#include <string.h>
#include <mpi.h>

const int MAX_STRING = 100;

int main()
{
    char greeting[MAX_STRING];
    int comm_sz;
    int my_rank;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    if (my_rank == comm_sz - 1)
    {
        sprintf(greeting, "Hola desde el proceso %d\n", my_rank);

        MPI_Recv(greeting, MAX_STRING, MPI_CHAR, my_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Send(greeting, strlen(greeting) + 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);

        printf("%s\n", greeting);
    }
    else if (my_rank == 0)
    {
        sprintf(greeting, "Hola desde el proceso %d\n", my_rank);

        MPI_Send(greeting, strlen(greeting) + 1, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(greeting, MAX_STRING, MPI_CHAR, comm_sz - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("%s\n", greeting);
    }
    else
    {
        sprintf(greeting, "Hola desde el proceso %d\n", my_rank);

        MPI_Send(greeting, strlen(greeting) + 1, MPI_CHAR, my_rank + 1, 0, MPI_COMM_WORLD);
        MPI_Recv(greeting, MAX_STRING, MPI_CHAR, my_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("%s\n", greeting);
    }
    MPI_Finalize();
}