#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
int main ()
{
  pid_t c_pid;
  c_pid = fork ();
  //c_pid = fork ();
  if (c_pid>0)
  {
      printf("Zombie Created!!");
      sleep (60);
  }
  else 
  {
    exit (0);
  }
  return 0;
}