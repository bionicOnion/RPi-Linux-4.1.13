#include <stdint.h>


#define SHR_MEM_SIZE 2000000


const char* SHR_MEM_NAME = "sharedBuffer";


typedef int shared_data_t;

typedef struct
{
  volatile uint8_t  write_guard;
  volatile uint8_t  read_guard;
  volatile uint8_t  delete_guard;
  volatile shared_data_t data[SHR_MEM_SIZE];
} SharedMemoryRegion;
