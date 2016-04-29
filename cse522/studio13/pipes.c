#include <stdio.h>
#include <unistd.h>


#define PIPE_READ 0
#define PIPE_WRITE 1
#define NUM_MESSAGES 10
#define MSG_SIZE 32


int main(int argc, char** argv)
{
  // Create a pipe
  int pipeFD[2];
  if (pipe(pipeFD) != 0)
  {
    printf("Pipe creation failed.\n");
    return -1;
  }

  // Fork off a child process
  int pid = fork();
  if (pid == 0)
  {
    // Define child behavior
    close(pipeFD[PIPE_WRITE]);
    int i;
    char msg[MSG_SIZE];
    for (i = 0; i < NUM_MESSAGES; ++i)
    {
      if (read(pipeFD[PIPE_READ], msg, MSG_SIZE) < MSG_SIZE)
      {
        printf("Failure to read correctly\n");
        close(pipeFD[PIPE_READ]);
        return -2;
      }
      printf("%s\n", msg);
    }
    close(pipeFD[PIPE_READ]);
  }
  else
  {
    // Define parent behavior
    close(pipeFD[PIPE_READ]);
    int i;
    char msg[MSG_SIZE];
    for (i = 0; i < NUM_MESSAGES; ++i)
    {
      snprintf(msg, MSG_SIZE, "Test message %d", i);
      if (write(pipeFD[PIPE_WRITE], msg, MSG_SIZE) < MSG_SIZE)
      {
        printf("Failure to write correctly\n");
        close(pipeFD[PIPE_WRITE]);
        return -3;
      }
    }
    close(pipeFD[PIPE_WRITE]);
  }
  
  return 0;
}
