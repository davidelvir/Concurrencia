#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

double sum;
long n = 1000000;
int thread_count;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *Thread_sum(void *rank);

int main(int argc, char const *argv[])
{

    pthread_t *thread_handles;
    thread_count = strtol(argv[1], NULL, 10);

    thread_handles = malloc(thread_count * sizeof(pthread_t));

    pthread_mutex_init(&mutex, NULL);
    for (long thread = 0; thread < thread_count; thread++)
    {
        pthread_create(&thread_handles[thread], NULL, Thread_sum, (void *)thread);
    }

    for (long thread = 0; thread < thread_count; thread++)
    {
        pthread_join(thread_handles[thread], NULL);
    }

    printf("La aproximación a PI es: %f\n", 4*sum);

    return 0;
}

void *Thread_sum(void *rank)
{
    long my_rank = (long)rank;
    double factor;
    long my_n = n / thread_count;
    long my_first_i = my_n * my_rank;
    long my_last_i = my_first_i + my_n;
    double my_sum = 0.0;

    if (my_first_i % 2 == 0)
    {
        factor = 1.0;
    }
    else
    {
        factor = -1.0;
    }

    for (int i = my_first_i; i < my_last_i; i++, factor = -factor)
    {
        my_sum += factor / (2*i+1);
    }
    pthread_mutex_lock(&mutex); //Mutex
    sum += my_sum;
    pthread_mutex_unlock(&mutex); // Mutex
    return NULL;
}
