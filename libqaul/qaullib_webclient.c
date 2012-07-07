/*
 * qaul.net is free software
 * licensed under GPL (version 3)
 */

#include "qaullib_private.h"

#ifdef WIN32
#define close(x) closesocket(x)
#undef errno
#define errno WSAGetLastError()
#undef strerror
#define strerror(x) StrError(x)
#define perror(x) WinSockPError(x)
#endif

#ifndef MSG_NOSIGNAL
#define MSG_NOSIGNAL 0
#endif

// ------------------------------------------------------------
int Qaullib_WgetConnect(struct qaul_wget_connection *myConn)
{
	int inet;
	myConn->connected = 0;

#ifdef WIN32
  int On = 1;
  unsigned long Len;
#else
  int flags;
#endif

  if (!myConn->socket)
  {
	if ((myConn->socket = socket(myConn->ip.sin_family, SOCK_STREAM, 0)) == -1)
	{
      perror("socket");
#ifdef WIN32
      int err = WSAGetLastError();
#else
      int err = -1;
#endif
      printf("[qaullib] socket error %i\n", err);
      return -1;
    }
  }

  printf("Attempting connect...");

  // connect to PORT on HOST
  if (connect(myConn->socket, (struct sockaddr *)&myConn->ip, sizeof(struct sockaddr)) < 0) {
    fprintf(stderr, "Error connecting %d - %s\n", errno, strerror(errno));
    return 0;
  } else {
	  char ipbuf[MAX(INET6_ADDRSTRLEN, INET_ADDRSTRLEN)];
	  inet_ntop(myConn->ip.sin_family, &myConn->ip.sin_addr, &ipbuf, MAX(INET6_ADDRSTRLEN, INET_ADDRSTRLEN));
	  printf("Connected!! socket: %i ip: %s\n", myConn->socket, ipbuf);

    // Setting socket non-blocking
#ifdef WIN32
    if (WSAIoctl(myConn->socket, FIONBIO, &On, sizeof(On), NULL, 0, &Len, NULL, NULL) < 0) {
      fprintf(stderr, "Error while making socket non-blocking!\n");
      //return 0
    }
#else
    if ((flags = fcntl(myConn->socket, F_GETFL, 0)) < 0) {
      fprintf(stderr, "Error getting socket flags!\n");
      //return 0;
    }

    if (fcntl(myConn->socket, F_SETFL, flags | O_NONBLOCK) < 0) {
      fprintf(stderr, "Error setting socket flags!\n");
      //return 0;
    }
#endif
    myConn->connected = 1;
    myConn->connected_at = time(NULL);
    myConn->lastreceived_at = time(NULL);
    myConn->received = 0;
    return 1;
  }
  return 0;
}


// ------------------------------------------------------------
int Qaullib_WgetClose(struct qaul_wget_connection *myConn)
{
	printf("Qaullib_WgetClose\n");

	int success;
	myConn->connected = 0;

	// if connection close fails, the response is not 0
	success = close(myConn->socket);
	myConn->socket = 0;
	if (success > 0)
	{
	  printf("[qaullib] connection closing error: %i", success);
	  return 0;
	}
	return 1;
}

// ------------------------------------------------------------
int Qaullib_WgetSendHeader(struct qaul_wget_connection *myConn, const char *header)
{
	int size;

	size = (int) strlen(header);
	if (send(myConn->socket, header, size, MSG_NOSIGNAL) < 0)
	{
		printf("[qaullib] tcp header error: connection lost!\n");
		Qaullib_WgetClose(myConn);
		return 0;
	}
	return 1;
}

// ------------------------------------------------------------
int Qaullib_WgetReceive(struct qaul_wget_connection *myConn)
{
	char *tmp;
	int bytes = 0;

	if (myConn->connected)
	{
		// check for timeouts
		if(myConn->lastreceived_at < time(NULL) - TIMEOUT_LASTRECEIVED ||
		   myConn->connected_at < time(NULL) - TIMEOUT_CONNECTED)
		{
			printf("[qaullib] socket received time out\n");
			// close connection
			Qaullib_WgetClose(myConn);
			return -1;
		}

		bytes = recv(myConn->socket, (char *)&myConn->buf +myConn->bufpos, BUFFSIZE -myConn->bufpos, 0);

		// connection was closed
		if (bytes == 0)
		{
			printf("[qaullib] bytes == 0: close connection\n");
			Qaullib_WgetClose(myConn);
			return 0;
		}
		// data has been received
		else if (bytes > 0)
		{
			myConn->lastreceived_at = time(NULL);
		}
	}
	return bytes;
}



