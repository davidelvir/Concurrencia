#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

long *resultado;
long **matriz;
long *vector;
long n;

void *multiplicar(void *filaX);

int main(int argc, char *argv[])
{

    long thread;
    n = strtol(argv[1], NULL, 10);
    pthread_t *thread_handles;

    matriz = (long **)malloc(n * sizeof(long *));
    vector = malloc(n * sizeof(long));
    resultado = malloc(n * sizeof(long));
    thread_handles = malloc(n * sizeof(pthread_t));

    srand(time(0));

    //llenar el vector con random e inicializar filas
    for (long i = 0; i < n; i++)
    {
        vector[i] = rand() % 100;
        matriz[i] = (long *)malloc(n * sizeof(long));
    }

    //llenar matriz con random
    for (long i = 0; i < n; i++)
    {
        for (long j = 0; j < n; j++)
        {
            matriz[i][j] = rand() % 100;
        }
    }

    printf("La matriz es: \n");
    for (long i = 0; i < n; i++)
    {
        printf("|\t");
        for (long j = 0; j < n; j++)
        {
            printf("%ld\t", matriz[i][j]);
        }
        printf("|");

        printf("\n");
    }

    printf("El vector es: \n");
    for (long i = 0; i < n; i++)
    {
        printf("|\t");
        printf("%ld", vector[i]);
        printf("\t|");
        printf("\n");
    }

    for (long i = 0; i < n; i++)
    {
        pthread_create(&thread_handles[i], NULL, multiplicar, (void *)i);
    }
    for (long i = 0; i < n; i++)
    {
        pthread_join(thread_handles[i], NULL);
    }

    printf("El vector resultante es: \n");

    for (int i = 0; i < n; i++)
    {
        printf("|\t");
        printf("%ld", resultado[i]);
        printf("\t|");
        printf("\n");
    }

    free(thread_handles);

    for (long i = 0; i < n; i++)
    {
        free(matriz[i]);
    }
}

void *multiplicar(void *rankX)
{
    long rank = (long)rankX;

    long sum = 0;
    for (int i = 0; i < n; i++)
    {
        sum += vector[i] * matriz[rank][i];
    }
    resultado[rank] = sum;
}