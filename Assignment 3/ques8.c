#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>

void *runner();

FILE *fi;
int f = 1;
int main(){

  pthread_t tid1, tid2;
  fi = fopen("/home/student/dk_os17/Ass3/8.txt", "r");

  pthread_create(&tid1, NULL, runner, NULL);
  pthread_create(&tid2, NULL, runner, NULL);
  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);
  fclose(fi);
}

void *runner()
{
  char ch;
  while((ch = getc(fi)) != EOF){
    if(ch == 'z'){
      pthread_exit(0);
      return 0;
    }
    if(ch == '\n')
      printf("\t%ld\n",pthread_self());
    printf("%c", ch);
  }

  pthread_exit(0);
}