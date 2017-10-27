#include<semaphore.h>
#include<stdio.h>
#include<pthread.h>

void *reader(void *);
void *writer(void *);

int readcount=0,writecount=0,sh_var=5,bsize[5];
sem_t x,y,z;//y controls updating of writecount, x controls readcount
pthread_t r[3],w[2];
int rsem=1,wsem=1;
int rwait=0;
int wwait=0;
void readwait(int *x)
{
	(*x)--;
	if((*x)<0)
	{	rwait++;//block the other readers
	}
}
void writewait(int *x)
{
	(*x)--;
	if((*x)<0)
	{	wwait++;
	}

}


void writesignal(int *x)
{	(*x)++;
	if((*x)>=0)
		wwait--;
}

void readsignal(int *x)
{	(*x)++;
	if((*x)>=0)
		rwait--;
}

	
	
	
	
void *reader(void *i)
{
//printf("\n-------------------------");
//printf("\n\n reader-%d is reading",i);
int zw = (int)i;
sem_wait(&z);//readers queue on z after one reader queues on rsem
readwait(&rsem);//a reader waits
sem_wait(&x);//x controls readcount
readcount++;
if(readcount<=2)
    sem_wait(&wsem);//if there's one reader then writer wait
if(wwait==1 && rwait==2)
	readwait(&rsem);

sem_post(&x);//end x here as updation of readcount is done
readsignal(&rsem);//the queue head is taken out
sem_post(&z);
printf("\nReading performed by reader %d\n",zw);
sem_wait(&x);
readcount--;
if(readcount==0)
    sem_post(&wsem);
sem_post(&x);
//printf("%ld count\n",x.__align);
}

void *writer(void *i)
{
int zw= (int)i;
//printf("\n\n writer-%d is writing",i);
sem_wait(&y);
writecount++;
if(readcount<=2)
	writewait(&wsem);
if(writecount==1)
	readwait(&rsem);//if there's a writer then wait other readers and writers
sem_post(&y);
writewait(&wsem);
if(rwait>=2)
	writewait(&wsem);
printf("Writing performed by writer %d\n",zw);
//sh_var=sh_var+5;
writesignal(&wsem);
sem_wait(&y);
writecount--;
if(writecount==0)
	readsignal(&rsem);
sem_post(&y);
}

int main()
{
sem_init(&x,0,1);
//sem_init(&wsem,0,1);
sem_init(&y,0,1);
sem_init(&z,0,1);
//sem_init(&rsem,0,1);

pthread_create(&r[0],NULL,(void *)reader,(void *)0);
pthread_create(&w[0],NULL,(void *)writer,(void *)0);
pthread_create(&r[1],NULL,(void *)reader,(void *)1);
pthread_create(&r[2],NULL,(void *)reader,(void *)2);
pthread_create(&r[3],NULL,(void *)reader,(void *)3);
pthread_create(&w[1],NULL,(void *)writer,(void *)3);
pthread_create(&r[4],NULL,(void *)reader,(void *)4);

pthread_join(r[0],NULL);
pthread_join(w[0],NULL);
pthread_join(r[1],NULL);
pthread_join(r[2],NULL);
pthread_join(r[3],NULL);
pthread_join(w[1],NULL);
pthread_join(r[4],NULL);

return(0);
}

