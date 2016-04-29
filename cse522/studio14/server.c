#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "constants.h"


inline int readFromSocket(SOCK socket, char* msgBuf)
{
  return read(socket, msgBuf, MAX_MSG_LEN);
}


int main(int argc, char** argv)
{

  int count = 0;
  char msgBuf[MAX_MSG_LEN];

  // Process command line arguments
  if (argc < MIN_ARG_COUNT)
  {
    printf("Not enough arguments provided.\n");
    return INSUFFICIENT_ARGS;
  }

  memset( msgBuf, 0, MAX_MSG_LEN);
  
  // Create a local socket
  SOCK serverSock = socket(AF_LOCAL, SOCK_STREAM, 0);
  if (serverSock < 0)
  {
    printf("Failed to create a socket: %s\n", strerror(errno));
    return SOCK_CREATE_FAIL;
  }
  
  // Bind the created socket
  struct sockaddr_un sockAddr;
  sockAddr.sun_family = AF_UNIX;
  strncpy(sockAddr.sun_path, argv[SOCK_FILE_ARG_IDX], UNIX_PATH_MAX);
  if (bind(serverSock, (struct sockaddr*) &sockAddr, sizeof(sockAddr)) < 0)
  {
    printf("Failed to bind the socket: %s\n", strerror(errno));
    return BIND_FAIL;
  }
  
  // Listen on the bound socket
  if (listen(serverSock, BACKLOG_SIZE) < 0)
  {
    printf("Failed to listen on the bound socket: %s\n", strerror(errno));
    return LISTEN_FAIL;
  }

  while( 1 )
  {
  

    printf( "Waiting for connection...\n" );

    // Accept a single connection
    struct sockaddr_un clientAddr;
    clientAddr.sun_family = AF_UNIX;
    SOCK clientSock = accept(serverSock, NULL, NULL);
    if (clientSock < 0)
    {
      printf("Failed to accept a client connection: %s\n", strerror(errno));
      return ACCEPT_FAIL;
    }

    printf( "Client Connected: %d\n", clientSock );

    memset( msgBuf, 0, MAX_MSG_LEN);
    while( strncmp( msgBuf, "quit", MAX_MSG_LEN ) != 0 )
    {
      
      int res = readFromSocket(clientSock, msgBuf);
      if( res < 0 )
      {
        printf( "ERROR: Recieving error %s\n", strerror(errno));
        break;
      }
      else if( res > 0 )
      {
        printf("%s\n", msgBuf);
      }
      
    }

    close( clientSock );

  }
  
  // Close the connections
  close(serverSock);
  if (unlink(argv[SOCK_FILE_ARG_IDX]) < 0)
  {
    printf("Failed when disposing of system resources: %s\n", strerror(errno));
    return UNLINK_FAIL;
  }
  
  return SUCCESS;
}
