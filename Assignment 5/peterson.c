#include <stdio.h>
#include <pthread.h>
int flag[2];
int turn;
const int MAX = 1e9;
int ans = 0;
void* processp1(void *s)
{
    int i = 0;
    int self = (int *)s;
    printf("Thread Entered: %d\n", self);
    flag[self]=1;
    turn=0;
    while(turn==0 && flag[1-self]==1);
    for (i=0; i<MAX; i++)
        ans++;
    flag[self]=0;
}
void* processp2(void *s)
{
    int i = 0;
    int self = (int *)s;
    printf("Thread Entered: %d\n", self);
    flag[self]=0;
    turn=1;
    while(turn==1 && flag[1-self]==1);
    for (i=0; i<MAX; i++)
        ans++;
    flag[self]=0;
}
int main()
{
    pthread_t p1, p2;
    flag[0]=0;
    flag[1]=0;
    turn=0;
    pthread_create(&p1, NULL, processp1, (void*)0);
    pthread_create(&p2, NULL, processp2, (void*)1);
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    printf("final Value: %d | Expected Value: %d\n",ans, MAX*2);
    return 0;
}   
