#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


#define BUF_SIZE 64


int main(int argc, char** argv)
{
  // Create a FIFO
  if (mkfifo("customFIFO", S_IRUSR | S_IWUSR) != 0)
  {
    printf("FIFO creation failed\n");
    return -1;
  }

  // Open the FIFO
  FILE* fifoRead = fopen("customFIFO", "r");
  FILE* fifoWrite = fopen("customFIFO", "w");

  // Read from the FIFO
  char msgBuf[BUF_SIZE];
  while (1)
  {
    int readValue;
    fscanf(fifoRead, "%d", &readValue);
    printf("%d * 2 = %d\n", readValue, readValue*2);
  }
}
