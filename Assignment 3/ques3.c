#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>

void *runner();

int main(){
  pthread_t tid;

  pthread_create(&tid, NULL, runner, NULL);
  pthread_join(tid, NULL);
  int j;
  j = pthread_join(tid, NULL);
  if(j == 0)
    printf("Thread Rejoined!\n");
  else
    printf("Thread Not Rejoined! The returned value is %d\n",j);
}

void *runner(){
  printf("Thread Created (Joined)!\n");
  int d = pthread_detach(pthread_self());
  if(d == 0)
    printf("Thread Detached!\n");
  else
    printf("Thread Not Detached! (%d)\n",d);
  pthread_exit(0);
}
