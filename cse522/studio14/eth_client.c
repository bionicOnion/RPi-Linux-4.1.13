#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

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

  char msgBuf[100];
  struct sockaddr_in remote;
  int rv = 0;
  
  // Create a local socket
  SOCK sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0)
  {
    printf("Failed to create a socket: %s\n", strerror(errno));
    return SOCK_CREATE_FAIL;
  }

  memset( &remote, 0, sizeof(remote));
  remote.sin_family = AF_INET;
  remote.sin_port = htons( 17000 );

  if( inet_aton( "24.107.185.147", &(remote.sin_addr) ) == 0 )
  {
    printf( "ERROR: Failed to convert provided address\n" );
    rv = -1;
  }
  
  // Connect to the server
  if (connect(sock, (struct sockaddr*) &remote, sizeof(remote)) < 0)
  {
    printf("Failed to connect: %s\n", strerror(errno));
    return BIND_FAIL;
  }

  printf( "Connect Successful\n" );

  while( 1 )
  {
    memset( msgBuf, 0, 100 );
    int recvAmt = read( sock, msgBuf, 1 );
    if( recvAmt == 0 )
    {
      break;
    }
    else if( recvAmt < 0 )
    {
      printf( "ERROR: %s\n", strerror(errno) );
      rv = errno;
    }
    else
    {
      printf( "%c", msgBuf[0] );
    }
  }
  
  close(sock);
  
  return SUCCESS;
}
