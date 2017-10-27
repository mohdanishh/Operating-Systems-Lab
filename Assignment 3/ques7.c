/*
Prove that in user-level threads, if one thread blocks for
an I/O, all other threads created within the process is also
blocked and the kernel switches to another process.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>

void* thread1(void *argv){
	//Compute Factorial
	printf("Insert Number: \n");
	int num; scanf("%d",&num);
	int fact=1;
	for (; num>1; num--){
		fact = fact*num;
	}	
		
	printf("Factorial is:	%d\n",fact);
	
	pthread_exit(0);
}

void* thread2(void *now){
	//return Current Time
	time_t cur_time=time(0);
	now = &cur_time;
  	pthread_exit(0);
}

void* thread3(void *a){
	//Increment a
	++a;
	pthread_exit(0);
}

int main(void)
{
//	pid_t x;
//	x=fork();
	if(fork()==0){
		//child process
		printf("Inside Process 1\n");
		pthread_t tid1,tid2;
		time_t now;
		pthread_create(&tid1,NULL,thread1,NULL);// I/O operation 
		pthread_create(&tid2,NULL,thread2,&now);//non I/O op of same proccess
		pthread_join(tid1,NULL);
		pthread_join(tid2,NULL);
		printf("Current time : %s",ctime(&now));
		exit(0);
	}
	else{
		//parent process
		if(fork()==0)
		{
			pthread_t tid3;
			printf("Inside Process 2\n");
			int a = 5;
			pthread_create(&tid3,NULL,thread3,&a);//non I/O op of different process
			pthread_join(tid3,NULL);
			printf("Incremented value of a is: %d\n",a);
		}
		else
			wait();
		sleep(2);
	}

	return 0;
}
