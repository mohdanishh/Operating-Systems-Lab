#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>

void *runner1();
void *runner2();

FILE *fi;
pthread_t master, child;

int main(){

  fi = fopen("/home/student/dk_os17/Ass3/10.txt", "r");
//  pthread_t master;
  pthread_create(&master, NULL, runner1, NULL);
  pthread_join(master, NULL);
  //printf("AM here!\n");
  fclose(fi);
}

void *runner1(){

  int i,j;
  for (i = 1; i <= 100; i++) {
    //sleep(1);
    pthread_create(&child, NULL, runner2, NULL);
  //  printf("<%d>",i);
  }
pthread_exit(0);
}

void *runner2(){

  char ch;
  //printf("AM here again!\n");
  if((ch = getc(fi)) == EOF)
    exit(0);
    /*if(ch == '\n')
      printf("\t%ld\n",child);*/
    if(ch != '\n')
    printf("%c\t%lu\t%lu\n", ch, master, child);
    pthread_exit(0);
}
