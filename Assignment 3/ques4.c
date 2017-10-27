#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>
#include <time.h>

void *fact(void *x);
void *timeNow();
void *prime(void *x);
void *fibonacci(void *x);

int main(int arg, char *argv[]){
  pthread_t tid;
  int ch = atoi(argv[1]);
  switch(ch){
    case 1 : pthread_create(&tid, NULL, fact, (argv[2]));
    break;
    case 2 : pthread_create(&tid, NULL, timeNow, NULL);
    break;
    case 3 : pthread_create(&tid, NULL, prime, (argv[2]));
    break;
    case 4 : pthread_create(&tid, NULL, fibonacci, (argv[2]));
    break;
    default : printf("Invalid Choice!\n");
  }
  pthread_join(tid, NULL);

  exit(0);
}

void *fact(void *x){
  printf("Executing Thread 1\n");
  int i,f = 1, xx = atoi(x);
  for(i = 1; i <= xx; i++)
    f *= i;
  printf("Factorial of %d :- %d\n", xx, f);
  pthread_exit(0);
}

void *timeNow(){
  printf("Executing Thread 2\n");
  time_t now = time(0);
  printf("Current time : %s",ctime(&now));
  pthread_exit(0);
}

void *prime(void *x){
  printf("Executing Thread 3\n");
  int i,j,p=0, xx = atoi(x);
  printf("The prime Nos upto %d :- \n",xx);
  for(i = 2; i <= xx; i++){
    p = 0;
    for(j = 1; j <= i; j++){
      if(i % j == 0)
      p = p + 1;
    }
    if(p == 2)
    printf("%d ",i);
  }
  printf("\n");
  pthread_exit(0);
}

void *fibonacci(void *x){
  printf("Executing Thread 4\n");
  int a=0,b=1,c=1, xx = atoi(x);
  printf("The Fibonacci series upo %d :-\n",xx);
  while(c < xx){
    c = a + b;
    if(c<=xx)
    printf("%d ",c);
    a = b;
    b = c;
  }
  printf("\n");
  pthread_exit(0);
}