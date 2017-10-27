#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
void func(int signum)
{
    wait(NULL);
}
 
int main()
{
    int i;
    int pid = fork();
    if (pid == 0)
            printf("This is Child\n");
    else
    if (pid>0)
    {
        printf("Zombie Created!!");
        sleep (10);
        printf("Zombie Prevented!!");
        signal(SIGCHLD, func);
        printf("This is Parent\n");
    }
}