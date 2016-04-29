#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "constants.h"


inline int writeToSocket(SOCK socket, char* msg)
{
  // Add one byte for the null terminator
  return write(socket, msg, strlen(msg)+1);
}


int main(int argc, char** argv)
{
  // Process command line arguments
  if (argc < MIN_ARG_COUNT)
  {
    printf("Not enough arguments provided.\n");
    return INSUFFICIENT_ARGS;
  }

  char msgBuf[100];
  
  // Create a local socket
  SOCK sock = socket(AF_LOCAL, SOCK_STREAM, 0);
  if (sock < 0)
  {
    printf("Failed to create a socket: %s\n", strerror(errno));
    return SOCK_CREATE_FAIL;
  }
  
  // Connect to the server
  struct sockaddr_un sockAddr;
  sockAddr.sun_family = AF_UNIX;
  strncpy(sockAddr.sun_path, argv[SOCK_FILE_ARG_IDX], UNIX_PATH_MAX);
  if (connect(sock, (struct sockaddr*) &sockAddr, sizeof(sockAddr)) < 0)
  {
    printf("Failed to connect: %s\n", strerror(errno));
    return BIND_FAIL;
  }
  
  // Send some messages over the connection
  sprintf( msgBuf, "hello from %d!", getpid() );
  writeToSocket(sock, msgBuf );
  writeToSocket(sock, "quit" );

  
  close(sock);
  
  return SUCCESS;
}
