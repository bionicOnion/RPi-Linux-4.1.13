#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "shared_mem.h"


int main(int argc, char** argv)
{
  int i;
  int shrMemFD;
  SharedMemoryRegion* shrMem;
  shared_data_t localData[SHR_MEM_SIZE];
  
  // Create the shared memory region
  shrMemFD = shm_open(SHR_MEM_NAME, O_RDWR | O_CREAT, S_IRWXU);
  if (ftruncate(shrMemFD, sizeof(SharedMemoryRegion)) != 0)
  {
    printf("Shared memory truncation failed\n");
    return -1;
  }
  shrMem = (SharedMemoryRegion*) mmap(NULL, sizeof(SharedMemoryRegion),
    PROT_READ | PROT_WRITE, MAP_SHARED, shrMemFD, 0);
  if (shrMem <= 0)
  {
    printf("mmap call failed\n");
    return -2;
  }
  
  // Explicitly zero out shared buffer to prevent possible bugs
  memset(shrMem, 0, sizeof(SharedMemoryRegion));
  
  // Generate random data
  srand(time(NULL));
  for (i = 0; i < SHR_MEM_SIZE; ++i)
  {
    localData[i] = rand();
  //   printf("localData[%d]: %d\n", i, localData[i]);
  }
  printf("Data ready\n");
  
  // Copy data to shared memory buffer
  while(shrMem->write_guard == 0)
  {}
  memcpy((void*) shrMem->data, (void*) localData,
    SHR_MEM_SIZE * sizeof(shared_data_t));
  shrMem->read_guard = 1; 
  
  while(shrMem->delete_guard == 0)
  {}
  close(shrMemFD);
  if (shm_unlink(SHR_MEM_NAME) != 0)
  {
    printf("Shared memory unlinking failed\n");
  }
}
