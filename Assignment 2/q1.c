#include<stdio.h>
#include<unistd.h>
#include<sys/time.h>
void loop_delay();
double time_diff(struct timeval x , struct timeval y)
{
    double x_ms , y_ms , diff;
     
    x_ms = (double)x.tv_sec*1000000 + (double)x.tv_usec;
    y_ms = (double)y.tv_sec*1000000 + (double)y.tv_usec;
     
    diff = (double)y_ms - (double)x_ms;
     
    return diff;
}
int main()
{
        int i=1;
        pid_t child;
        child=fork();
        struct timeval before , after;
        gettimeofday(&before , NULL);
        if (child == 0)
        {
                printf ("Child successfully Created\n");
                loop_delay();
                gettimeofday(&after , NULL);
                printf("Total time elapsed : %.0lf us\n" , time_diff(before , after) );
        }
        else
        if(child==-1)
        {
                printf("Failed to create the child\n");
        }
        return 0;
}
void loop_delay()
{
        int i;
        for(i=1;i<=10000000;i++);
        printf("Loop succesfully executed ! \n"); 
}      
