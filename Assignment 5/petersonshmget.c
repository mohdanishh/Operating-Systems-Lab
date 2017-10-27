#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#define BUFFER_SIZE 10
#define MAX_ITEM 101


void reset_buffer(int *buf,int *c);
void display_buffer(int *buf);

bool *SHM1;
int *SHM2;
int *SHM3;
int *SHM4;
key_t k1 = 2429;
key_t k2 = 3329;
key_t k3 = 4005;
key_t k4 = 7550;
key_t k5 = 1650;
int in=0,out=0,j=0,l=0;
int p=0;
void *producer()
{
if(SHM1 == (bool*)-1 || SHM2 == (int*)-1 ||SHM3 == (int*)-1|| SHM4 == (int*)-1)
{
perror("ERORR !!!!! \n");
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
        *turn = 1;
        while(flag[1] && *turn ==1);
        if(*counter == BUFFER_SIZE)
        {
        printf("BUFFER IS FULL\n");
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
        printf("ITEM PRODUCED %d \n",item);
        in =(in+1)%BUFFER_SIZE;
        (*counter)++;
        printf("NO. OF ITEMS IN BUFFER %d \n",*counter);
        }
        display_buffer(buffer);
        flag[0]=false;
    }
        exit(0);
}
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
int rand_gen()
{
int rt =1+ rand()% MAX_ITEM;
return rt;
}
void display_buffer(int *buf)
{
int index=0;
printf("CURRENT BUFFER : ");
while(index<BUFFER_SIZE)
{
printf("%d\t" ,buf[index++]);
}
printf("\n");
}
void *consumer()
{

if(SHM1 == (bool*)-1 || SHM2 == (int*)-1  ||SHM3 == (int*)-1|| SHM4 == (int*)-1)
{
perror("ERORR !!!!! \n");
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
        *turn = 0;
        while(flag[0]  && *turn ==0);
        if(*counter == 0)
        {
        printf("BUFFER IS EMPTY\n");
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
        printf("ITEM CONSUMED %d \n",item);
        out =(out+1)%BUFFER_SIZE;
        (*counter)--;
        printf("NO. OF ITEMS IN BUFFER %d \n",*counter);
        }
        display_buffer(buffer);
        flag[1]=false;
     }
        exit(0);
}
int main()
{
    pid_t pid1;
    pid_t pid2;
    int SHM_ID_1,SHM_ID_2,SHM_ID_3,SHM_ID_4,SHM_ID_5,SHM_ID_6;
     SHM_ID_1 = shmget(k1,2*(sizeof(bool)),IPC_CREAT|0666);
     SHM_ID_2 = shmget(k2,1*(sizeof(int)),IPC_CREAT|0666);
     SHM_ID_3 = shmget(k3,BUFFER_SIZE*(sizeof(int)),IPC_CREAT|0666);
     SHM_ID_4 = shmget(k4,1*(sizeof(int)),IPC_CREAT|0666);
     SHM_ID_5 = shmget(k5,1024,IPC_CREAT|0666);
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
    perror("PRODUCER ERORR !!!!! \n");
    exit(EXIT_FAILURE);
    }
    if(pid1 == 0)
    {
    pdc_ptr = shmat(SHM_ID_5,NULL,0);
    pdc_ptr = producer(SHM_ID_1,SHM_ID_2,SHM_ID_3,SHM_ID_4);
    exit(0);
    }
    pid2 = fork();
    if(pid2<0)
    {
    perror("PRODUCER ERORR !!!!! \n");
    exit(EXIT_FAILURE);
    }
    if(pid2 == 0)
    {
    pdc_ptr = shmat(SHM_ID_5,NULL,0);
    pdc_ptr = consumer(SHM_ID_1,SHM_ID_2,SHM_ID_3,SHM_ID_4);
    exit(0);
    }
    wait();
    return 0;
}