#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
int main()
{
    int i;
    int c_pid = fork();
/*    if (c_pid == 0)
           printf("This is Child\n");
    else*/
    if (c_pid>0)
 	{
        printf("Zombie Created!!");
        sleep (5);
        signal(SIGCHLD,SIG_IGN);
        printf("Zombie Prevented!!");
        printf("This is Parent\n");
    }
    return 0;
}