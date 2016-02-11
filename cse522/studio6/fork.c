#include <stdio.h>
#include <unistd.h>


int main(int argc, char** argv)
{
  printf("Hello from parent\n");
  pid_t pid = fork();
  if (pid == 0)
  {
    printf("Hello from child\n");
  }
  else
  {
    printf("Child PID: %d\n", pid);
  }
}
