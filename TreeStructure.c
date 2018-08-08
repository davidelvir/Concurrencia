#include <stdio.h>
#include <string.h>
#include <mpi.h>

const int MAX_STRING = 100;

double Trap(double left_endpt, double right_endpt, int trap_count, double base_len);
double f(double x);

int main()
{
    int my_rank, comm_sz, n = 1024, local_n;
    double a = 1.0, b = 15.0, h, local_a, local_b;
    double local_int, total_int, res;
    char greeting[MAX_STRING];

    int source;

    MPI_Init(NULL, NULL);

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    h = (b - a) / n;

    local_n = n / comm_sz;

    local_a = a + my_rank * local_n * h;
    local_b = local_a + local_n * h;

    //el proceso calcula sus áreas
    local_int = Trap(local_a, local_b, local_n, h);


    /*si el número de trapezoides(n) no es divisible entre el número de procesos(comm_sz)
    le digo al último proceso que me sume lo que falta
    */
    if (my_rank == comm_sz - 1 && (n % comm_sz != 0))
    {
        //calculo el área desde local_b hasta b y el número de trapecios a calcular es n % comm_sz
        local_int += Trap(local_b, b, (n % comm_sz), h);
    }

    //si el proceso es impar solo manda su área a un proceso par
    if (my_rank % 2 != 0)
    {
        sprintf(greeting, "El proceso %d ha calculado %f\n", my_rank, local_int);
        MPI_Send(greeting, strlen(greeting) + 1, MPI_CHAR, 0, 1, MPI_COMM_WORLD);

        MPI_Send(&local_int, 1, MPI_DOUBLE, my_rank - 1, 0, MPI_COMM_WORLD);
    }
    else
    {
        //si es par y NO es 0 entonces recibe de un proceso impar, suma a su área y lo manda al proceso 0
        if (my_rank != 0)
        {
            sprintf(greeting, "El proceso %d ha calculado %f\n", my_rank, local_int);
            MPI_Send(greeting, strlen(greeting) + 1, MPI_CHAR, 0, 1, MPI_COMM_WORLD);

            //total_int acumula el area del proceso par y el impar
            total_int = local_int;

            //si el último proceso es par, no recibe de ningun proceso impar
            if (my_rank != comm_sz - 1)
            {
                MPI_Recv(&local_int, 1, MPI_DOUBLE, my_rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                total_int += local_int;

                MPI_Send(&total_int, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);

                sprintf(greeting, "La suma parcial del proceso %d es %f\n", my_rank, total_int);
                MPI_Send(greeting, strlen(greeting) + 1, MPI_CHAR, 0, 2, MPI_COMM_WORLD);
            }
            else
            {
                MPI_Send(&total_int, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
            }
        }
        else
        {
            total_int = local_int;
            printf("El proceso 0 ha calculado %f\n", local_int);

            for (source = 1; source < comm_sz; source++)
            {
                if (source % 2 == 0 || source == 1)
                {
                    MPI_Recv(&local_int, 1, MPI_DOUBLE, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    total_int += local_int;
                }
                MPI_Recv(greeting, MAX_STRING, MPI_CHAR, source, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                printf("%s\n", greeting);

                if (source % 2 != 0 && source != comm_sz - 1 && source - 1 != 0)
                {
                    MPI_Recv(greeting, MAX_STRING, MPI_CHAR, source - 1, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    printf("%s\n", greeting);
                }
            }
        }
    }

    if (my_rank == 0)
    {
        printf("Version Final\n");

        printf("With n = %d trapezoids, our estimate\n", n);
        printf("of the integral from %f to %f = %.15e\n", a, b, total_int);
    }
    MPI_Finalize();
    return 0;
}

double Trap(double left_endpt, double right_endpt, int trap_count, double base_len)
{

    double estimate, x;
    int i;

    estimate = (f(left_endpt) + f(right_endpt)) / 2.0;
    for (i = 1; i <= trap_count - 1; i++)
    {
        x = left_endpt + i * base_len;
        estimate += f(x);
    }

    estimate = estimate * base_len;

    return estimate;
}
double f(double x)
{
    return x * x;
}