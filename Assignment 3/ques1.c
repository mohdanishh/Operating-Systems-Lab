#define _GNU_SOURCE 
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *runner(void *param);

int main(int argc, char *argv[]){
  pthread_t tid;
  pthread_attr_t attr;

  pthread_attr_init(&attr);
  pthread_create(&tid, &attr, runner, argv[1]);
  int check = pthread_join(tid, NULL);
  if(check == 0)
  	printf("Thread reaped by main thread!");
  else
  	printf("Thread NOT reaped by main thread!");

}

void *runner(void *param){
  pthread_attr_t attrF;
  size_t stack_size, guard_size;
  int detach_state, inherit_sched, policy, contention_scope;
  void *stack_addr;

  printf("Attributes of created thread:\n");

  pthread_getattr_np(pthread_self(), &attrF);
  pthread_attr_getguardsize(&attrF, &guard_size);
  pthread_attr_getstack(&attrF, &stack_addr, &stack_size);
  pthread_attr_getdetachstate(&attrF, &detach_state);
  pthread_attr_getinheritsched(&attrF, &inherit_sched);
  pthread_attr_getschedpolicy(&attrF, &policy);
  pthread_attr_getscope(&attrF, &contention_scope);
  printf("Stack size = %lu bytes\n",stack_size);
  printf("Guard size = %lu bytes\n",guard_size);
  printf("Detach State = %d bytes\n",detach_state);
  printf("Inherit Sched = %d bytes\n",inherit_sched);
  printf("Stack addr = %p bytes\n",stack_addr);
  printf("Policy = %d bytes\n",policy);
  printf("Scope =%d bytes\n",contention_scope);
  pthread_exit(0);
}