#include <math.h>
#include <stdio.h>
#include <unistd.h>


void genChildren(int generationLevel);


int main(int argc, char** argv)
{
  if (argc < 2)
  {
    printf("Too few arguments provided.\n");
    return 1;
  }
  genChildren( atoi(argv[1]));
}


void genChildren(int generationLevel)
{
    printf("%d\n", getpid());

  // Base case
  if (generationLevel == 0)
    return;

  if (fork() == 0)
    genChildren(generationLevel - 1);
  else if (fork() == 0)
    genChildren(generationLevel - 1);

  sleep(120);
}
