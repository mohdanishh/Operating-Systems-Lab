#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
int main()
{
    int pid=fork();
    if (pid>0)
    {
          printf("\nZombie Created!!\n");
          sleep (10);
          if(fork()==0)
            {
                if(fork()==0)
                {
                 printf("Grandchild prevented the Zombie !!\n");
                 printf("Zombie Prevented!!\n");
                }
            }
        }
    else
    {
        exit (0);
    }
    return 0;
    
}