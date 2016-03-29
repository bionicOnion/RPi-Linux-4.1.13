#include <stdio.h>
#include <unistd.h>



#define MAX_VAL 1000


int main(int argc, char** argv)
{
  if (argc < 2)
  {
    printf("Need to provide name of FIFO.\n");
    return -1;
  }

  // Fork off a child process
  int pid = fork();
  if (pid == 0)
  {
    // Define child behavior
    FILE* fifo = fopen(argv[1], "w");
    int i;
    for (i = 0; i < MAX_VAL; i += 2)
      fprintf(fifo, "%d\n", i);
    fclose(fifo);
  }
  else
  {
    // Define parent behavior
    FILE* fifo = fopen(argv[1], "w");
    int i;
    for (i = 1; i < MAX_VAL; i += 2)
      fprintf(fifo, "%d\n", i);
    fclose(fifo);
  }
  
  return 0;
}
