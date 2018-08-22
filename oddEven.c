#include <stdio.h>
#include <string.h>
#include <mpi.h>

int Compute_partner(int phase, int my_rank, int comm_sz);
void Odd_even_sort(int* a[2], int n);

int main()
{
    int my_rank, comm_sz;
    int arreglo[] = {2,3,1,23,45,67,12,5};
    int* local_a[2];

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    MPI_Scatter(&arreglo,2,MPI_INT,&local_a,2,MPI_INT,0,MPI_COMM_WORLD);

    Odd_even_sort(&local_a,2);

    if(my_rank == 0){
            printf("%d\n",local_a[0]);
        
    }
    MPI_Finalize();
    return 0;
}

int Compute_partner(int phase, int my_rank, int comm_sz){
    int partner;
    if(phase % 2 == 0){
        if(my_rank % 2 != 0){
            partner = my_rank-1;
        }else{
            partner = my_rank+1;
        }
    }else{
        if(my_rank % 2 != 0){
            partner = my_rank + 1;
        }else{
            partner = my_rank - 1;
        }
    }
    if(partner == -1 || partner == comm_sz){
        partner = MPI_PROC_NULL;
    }

    return partner;
}
void Odd_even_sort(int* a[2], int n)
{
    int phase, i;
    int* temp;

    for (phase = 0; phase < n; phase++)
    {
        if (phase % 2 == 0)
        {
            for (i = 1; i < n; i += 2)
            {
                if (a[i - 1] > a[i])
                {
                    temp = a[i];
                    a[i] = a[i - 1];
                    a[i - 1] = temp;
                }
            }
        }
        else
        {
            for (i = 1; i < n - 1; i += 2)
            {
                if (a[i] > a[i + 1])
                {
                    temp = a[i];
                    a[i] = a[i + 1];
                    a[i + 1] = temp;
                }
            }
        }
    }
}