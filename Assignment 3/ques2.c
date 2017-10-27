/*
From main create the thread t1.
Within t1 create t2.
Within t2 create t3.
Demonstrate and explain the following:
i) if t1 exits what will be the status of t2 and t3 if t1 calls
pthread_join on t3.
ii) if the main thread exits.
*/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void* thread3(void *argv){
	
	printf("Thread 3 ID:	%lu\n",pthread_self());
	pthread_exit(0);
}

void* thread2(void *tid3){
	

	printf("Thread 2 ID:	%lu\n",pthread_self());
	pthread_create(tid3,NULL,thread3,NULL);
	
	pthread_exit(0);
}

void* thread1(void *argv){
	
	pthread_t tid2;
	long unsigned int tid3;
	printf("Thread 1 ID:	%lu\n",pthread_self());
	pthread_create(&tid2,NULL,thread2,&tid3);
	
	sleep(2);
	
	int check = pthread_join(tid3, NULL);
	printf("\nThread 1 joined with:	%lu\n",tid3);
	
  	
  	if(check == 0)
 	 	printf("Thread3 reaped by Thread1!\n");
  	else
  		printf("Thread3 NOT reaped by Thread1!\n");

	pthread_exit(0);
}

int main(void)
{
	pthread_t tid1;
	pthread_create(&tid1,NULL,thread1,NULL);
	int check = pthread_join(tid1, NULL);
  	
  	if(check == 0)
 	 	printf("Thread1 reaped by main thread!\n");
  	else
  		printf("Thread1 NOT reaped by main thread!\n");

	return 0;
}