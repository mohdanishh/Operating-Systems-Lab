#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/time.h>
#define SIZE 10

int shmid_1,shmid_2,shmid_3,shmid_4,shmid_5;
int *t1;
int *t2;
int *reader;
int *writer; 
int *mutex;
int wait(mutex)
{
	while(mutex<=0); //no-operation
	(mutex)--;
	return (mutex);
}
int signal(mutex)
{
	(mutex)++;
	return (mutex);
}
int main()
{
	shmid_1 = shmget(IPC_PRIVATE, 1*sizeof(int), IPC_CREAT | 0666); //flag_id
	shmid_2 = shmget(IPC_PRIVATE, 1*sizeof(int), IPC_CREAT | 0666); //turn_id
	shmid_3 = shmget(IPC_PRIVATE, SIZE*sizeof(int), IPC_CREAT | 0666); //reader_id
	shmid_4 = shmget(IPC_PRIVATE, 1*sizeof(int), IPC_CREAT | 0666); //mutex_q
	if (shmid_1 < 0 || shmid_2 < 0 || shmid_3 <0 || shmid_5 < 0) 
	{
    		printf("shmget error\n");
     		exit(1);
	}
	mutex = (int *)shmat(shmid_4, NULL, 0);
	*mutex = 1;	
	int pflag = 0, state = 1;	
	while(1)
	{	
		if(pflag == 0)	//PRODUCER CODE
		{
			pflag ++;
			if(fork()==0)
			{
				t1 = (int *)shmat(shmid_1, NULL, 0);
				*t1 = 0;
				t2 = (int *)shmat(shmid_2, NULL, 0);
				(*t2) = 0;
				reader = (int *)shmat(shmid_3, NULL, 0);
				writer = (int *)shmat(shmid_5, NULL, 0);
				while(*mutex)
				{
					wait(*mutex);
					while((*t1)<SIZE)
					{
						(*t1)++;
						//printf("\n%d\t%d\n",reader[*t1],(*t1));
						if(reader[*t1] == 0)
						{
							int temp = 1;
							printf("READER has entered\n");
							reader[(*t1)] = temp;
							break;
						}						
					}
					if(*t1 == SIZE)
					{
						printf("Reader is full now\n");
					}
					int index = 0, pindex = 0;
					printf("WAITING READER: ");
					for(index=1;index<=(*t1);index++)
						printf("%d\t",reader[index]);
					printf("\n");
					printf("WAITING WRITER: ");
					for(pindex=1; pindex<=(*t2); pindex++)
						printf("%d\t",writer[*t2]);
					printf("\n");
					signal(*mutex);
					//critical section ends
					wait(*mutex);
					if((*t1)>=2)
					{
						printf("2 Readers are active\n");
						reader[(*t1)--] = 0;
						reader[(*t1)--] = 0;
						printf("WAITING READER: ");
						for(index=1;index<=(*t1);index++)
							printf("%d\t",reader[index]);
						printf("\n");
						sleep(2);
						printf("Reading is Completed\n");
					}
					else if((*t2)>=1 && (*t1)<2)
					{
						printf("Writer is active.\n");
						writer[(*t2)--] = 0;
						printf("WAITING WRITER: ");
						for(pindex=1; pindex<=(*t2); pindex++)
							printf("%d\t",writer[*t2]);
						printf("\n");
						sleep(3);
						printf("Writing is Completed\n");
					}
					else
					{
						printf("Neither Reader nor Writer is Active\n");
					}
					signal(*mutex);
					printf("\n");
					int read_time = rand() % 3 + 1;//reader will wait for any random time between 1-3
					sleep(read_time);
				}
				exit(0);
			}
			if(fork()==0)	//CONSUMER CODE
			{
				t2 = (int *)shmat(shmid_2, NULL, 0);
				(*t2) = 0;
				//reader = (int *)shmat(shmid_3, NULL, 0);
				writer = (int *)shmat(shmid_5, NULL, 0);
				//sleep(5);
				while(*mutex)
				{
					sleep(2);
					wait(*mutex);
					int index = 1, pindex=1;
					while((*t2)<SIZE)
					{
						(*t2)++;
						if(writer[*t2]==0)
						{
							printf("\nWRITER has entered");
							writer[(*t2)]=1;
							break;
						}
					}
					if((*t2) == SIZE)
					{
						printf("Writer is full\n");
					}
					signal(*mutex);
					//Critical Section End	
					int write_time = rand() % 10 + 1;
					sleep(write_time);
				}
				exit(0);
			}
		}
	}
	wait(NULL);
	return 0;
}
