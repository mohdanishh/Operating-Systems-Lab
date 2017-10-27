#include <stdio.h>
#include <sys/resource.h>
#include<unistd.h>
int main()
{
        int i=1;
        pid_t child;
        child=fork();
        for(i=1;;i++)
        {
              child=fork();
              printf("child=%d/n",i);
              if(child<0)
              break;
        }
        printf("Total forked:%d",i);
        return 0;
}
