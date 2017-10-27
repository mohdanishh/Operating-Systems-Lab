#include<stdio.h>
#include<pthread.h>

void *thread ( void *vargp)
{
}
int main()
{
    int err = 0, count = 0;
    pthread_t tid;
    while (err == 0)
    {
        err = pthread_create (&tid, NULL, thread, NULL);
        count++;
    }
    printf("Maximum number of thread within a Process is : %d\n", count);
}