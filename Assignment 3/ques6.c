#include<stdio.h>
#include<pthread.h>
#include<time.h>
#include<unistd.h>

void *thread (void *vargp)
{
}
int main()
{
	int i;
	clock_t begin1,begin2,end1,end2;
	pthread_t tid;
	begin1 = clock();
	for(i=0;i<1000;i++)
		pthread_create(&tid,NULL,thread,NULL);
	end1 = clock();
	begin2 = clock();
	for(i=0;i<1000;i++)
	{
		if(fork()==0)
			return 0;
	}
	end2 = clock();
	printf("Thread Time  : %fsec\n",(double)(end1-begin1)/(CLOCKS_PER_SEC*1000));
	printf("Process Time : %fsec\n",(double)(end2-begin2)/(CLOCKS_PER_SEC*1000));
	return 0;
}