#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/mman.h>
#include <sys/stat.h>

#include "shared_mem.h"


int main(int argc, char** argv)
{
  int i;
  int shrMemFD;
  SharedMemoryRegion* shrMem;
  shared_data_t localData[SHR_MEM_SIZE];
  
  // Create the shared memory region
  shrMemFD = shm_open(SHR_MEM_NAME, O_RDWR, S_IRWXU);
  shrMem = (SharedMemoryRegion*) mmap(NULL, sizeof(SharedMemoryRegion),
    PROT_READ | PROT_WRITE, MAP_SHARED, shrMemFD, 0);
  if (shrMem <= 0)
  {
    printf("mmap call failed\n");
    return -2;
  }

  // Indicate that we're ready to receive data
  shrMem->write_guard = 1;
  while(shrMem->read_guard == 0)
  {}
  
  // Copy shared data
  memcpy((void*) localData, (void*) shrMem->data,
    SHR_MEM_SIZE * sizeof(shared_data_t));
  shrMem->delete_guard = 1;
  close(shrMemFD);
  
  // Print received data
  // for (i = 0; i < SHR_MEM_SIZE; ++i)
  //   printf("localData[%d]: %d\n", i, localData[i]);
}
