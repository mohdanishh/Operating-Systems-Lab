#include<stdio.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<unistd.h>
#include<stdlib.h>
#include<time.h>
#include<stdbool.h>


#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define clear()   printf("\033[H\033[J")



#define BUFFER_SIZE 10
#define MAX_ITEM 101
#define key 2103
#define WAIT_P 2
#define WAIT_C 5


void *producer();
void *consumer();


bool *SHM1;
int *SHM2;
int *SHM3;
int *SHM4;


key_t k1 = 2123;
key_t k2 = 3524;
key_t k3 = 4155;
key_t k4 = 7855;
key_t k5 = 1456;
key_t k6 = 2589;


int in=0,out=0,j=0,l=0;
int p=0;




//buffer will be reseted
void reset_buffer(int *buf,int *c)
{
int i=0;
for(i=0;i<BUFFER_SIZE;i++)
{
buf[i]=0;
}
in=0;
out=0;
*c=0;
}




int main()
{
	clear();
	pid_t pid1;
	pid_t pid2;



	int SHM_ID_1,SHM_ID_2,SHM_ID_3,SHM_ID_4,SHM_ID_5,SHM_ID_6;
	 SHM_ID_1 = shmget(k1,2*(sizeof(bool)),IPC_CREAT|0666);
	 SHM_ID_2 = shmget(k2,1*(sizeof(int)),IPC_CREAT|0666);
	 SHM_ID_3 = shmget(k3,BUFFER_SIZE*(sizeof(int)),IPC_CREAT|0666);
	 SHM_ID_4 = shmget(k4,1*(sizeof(int)),IPC_CREAT|0666);
	 SHM_ID_5 = shmget(k5,1024,IPC_CREAT|0666);
	// SHM_ID_6 = shmget(k6,1024,IPC_CREAT|0666);


	SHM1 = (bool*)shmat(SHM_ID_1,NULL,0);
	SHM2 = (int*)shmat(SHM_ID_2,NULL,0);
	SHM3 = (int*)shmat(SHM_ID_3,NULL,0);
	SHM4 = (int*)shmat(SHM_ID_4,NULL,0);
	reset_buffer(SHM3,SHM4);

	int status1,status2;

	void *pdc_ptr;
	void *consm_ptr;

pid1 = fork();
if(pid1<0)
{
perror(ANSI_COLOR_RED " \n PRODUCER ERORR !!!!! ");
exit(EXIT_FAILURE);
}
if(pid1 == 0)
{
pdc_ptr = shmat(SHM_ID_5,NULL,0);
pdc_ptr = producer(SHM_ID_1,SHM_ID_2,SHM_ID_3,SHM_ID_4);
exit(0);
}
/*else
{
wait(&status1);
}*/



pid2 = fork();
if(pid2<0)
{
perror(ANSI_COLOR_RED " \n PRODUCER ERORR !!!!! ");
exit(EXIT_FAILURE);
}
if(pid2 == 0)
{
pdc_ptr = shmat(SHM_ID_5,NULL,0);
pdc_ptr = consumer(SHM_ID_1,SHM_ID_2,SHM_ID_3,SHM_ID_4);
exit(0);
}
/*else
{
wait(&status1);
}*/

wait();
wait();

return 0;
}


//displaying buffer
void display_buffer(int *buf)
{
int index=0;
printf("\n ************* CURRENT BUFFER ********* \n");
while(index<BUFFER_SIZE)
{
printf(ANSI_COLOR_RESET"\t %d" ,buf[index++]);
}
printf("\n\n\n\n");
}




// random waiting time for producer
int wait_producer()
{
int wt = rand()% WAIT_P;
//sleep (wt);
return wt;
}





// random waiting time for consumer
 int wait_consumer()
{
int wt = rand()% WAIT_C;
//sleep (wt);
return wt;
}





// produce a random number between 1 and 100
int rand_gen()
{
int rt =1+ rand()% MAX_ITEM;
return rt;
}






// producer function

void *producer()
{



if(SHM1 == (bool*)-1 || SHM2 == (int*)-1 ||SHM3 == (int*)-1|| SHM4 == (int*)-1)
{
perror(ANSI_COLOR_RED " \n ERORR !!!!! ");
exit(EXIT_FAILURE);
}


bool *flag = SHM1;
int *turn = SHM2;
int *buffer = SHM3;
int *counter = SHM4;


while(1)
{
sleep(1);
flag[0]=true;
printf(ANSI_COLOR_GREEN "\n PRODUCER IS READY TO ENTERED THE CRITICAL SECTION TO PRODUCE AN ITEM \n");
*turn = 1;
while(flag[1] && *turn ==1);
//*********CRITICAL SECTION***************
printf(ANSI_COLOR_YELLOW "\nPRODUCER ENTERED INTO THE CRITICAL SECTION ");

// producer produced an item



if(*counter == BUFFER_SIZE)// condition to check buffer is full or not;
{
printf(ANSI_COLOR_RED "\n BUFFER IS FULL \t NO ITEM CAN BE PRODUCED AT THIS MOMENT \n");
j++;
if(j>3)
{
reset_buffer(buffer,counter);
j=0;
}
}
else
{
int item = rand_gen();
buffer[in] = item;
printf(ANSI_COLOR_RESET"\n ITEM PRODUCED ::::: %d \n",item);
in =(in+1)%BUFFER_SIZE;
(*counter)++;
printf(ANSI_COLOR_MAGENTA"\n NO OF ITEMS IN BUFFER %d \n",*counter);
}
display_buffer(buffer);
printf(ANSI_COLOR_CYAN "\n PRODUCER IS RANDOMLY WAITING TO PRODUCE AN ITEM  for %d milisec.. .. .. ...........\n",wait_producer());
flag[0]=false;
sleep(wait_producer());

}
exit(0);
}





// consumer function


void *consumer()
{

if(SHM1 == (bool*)-1 || SHM2 == (int*)-1  ||SHM3 == (int*)-1|| SHM4 == (int*)-1)
{
perror(ANSI_COLOR_RED " \n ERORR !!!!! ");
exit(EXIT_FAILURE);
}



bool *flag = SHM1;
int *turn = SHM2;
int *buffer = SHM3;
int *counter = SHM4;



while(1)
{
sleep(1);
flag[1]=true;
printf(ANSI_COLOR_GREEN "\n CONSUMER IS READY TO ENTERED THE CRITICAL SECTION TO PRODUCE AN ITEM \n");
*turn = 0;
while(flag[0]  && *turn ==0);
//*********CRITICAL SECTION***************
printf(ANSI_COLOR_YELLOW "\n CONSUMER ENTERED INTO THE CRITICAL SECTION ");

// consumer consumed  an item
if(*counter == 0)
{
printf(ANSI_COLOR_RED "\n BUFFER IS EMPTY \t NO ITEM CAN BE CONSUMED AT THIS MOMENT \n");
l++;
if(l>3)
{
reset_buffer(buffer,counter);
l=0;
}
}
else
{
int item = buffer[out];
buffer[out] = 0;
printf(ANSI_COLOR_RESET"\n ITEM CONSUMED ::::: %d \n",item);
out =(out+1)%BUFFER_SIZE;
(*counter)--;
printf(ANSI_COLOR_MAGENTA"\n NO OF ITEMS IN BUFFER %d \n",*counter);
}
display_buffer(buffer);
printf(ANSI_COLOR_CYAN "\n CONSUMER IS RANDOMLY WAITING TO CONSUMING AN ITEM  for %d milisec.. .. .. ...........\n",wait_consumer());

flag[1]=false;
sleep(wait_consumer());
}
exit(0);
}









