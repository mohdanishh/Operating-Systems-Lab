#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>

void *runner1();
void *runner2();

pthread_t tid1,tid2;

int main(){

  pthread_create(&tid1, NULL, runner1, NULL);
  pthread_create(&tid2, NULL, runner2, NULL);
  pthread_join(tid1, NULL);

}
void *runner1(){

  pthread_join(tid2, NULL);
  int r;
  r = pthread_cancel(tid2);
  if(r == 0)
    printf("Thread 2 Killed by itself!\n");
  else
    printf("Thread 2 Not Killed by Thread 1 %d!\n",r);

  pthread_exit(0);
}
void *runner2()
{
  int dt = 2;
  dt = pthread_detach(pthread_self());
  if(dt == 0)
    printf("Thread 2 Detached!\n");
  else
    printf("Thread 2 Not Detached!\n");
    pthread_exit(0);
}