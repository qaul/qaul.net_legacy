/*
 * qaul.net is free software
 * licensed under GPL (version 3)
 */

#ifndef _QAULLIB_WEBCLIENT
#define _QAULLIB_WEBCLIENT

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * structure of web client connection
 */
// FIXME: ipv6
struct qaul_wget_connection
{
	struct sockaddr_in ip;
	int connected;
	int socket;
	int max_ttl;
	int max_lastreceived;
	time_t connected_at;
	time_t lastreceived_at;
	int received;
	int bufsize;
	int bufpos;
	union qaul_inbuf buf;
};


/**
 * connect the @a myConn info
 *
 * @retval -1 socket error
 * @retval  0 not able to connect
 * @retval  1 successfully connected
 */
int Qaullib_WgetConnect(struct qaul_wget_connection *myConn);

/**
 * close the @a myConn connection
 *
 * @retval 1 successfully closed
 * @retval 0 error in closing
 */
int Qaullib_WgetClose(struct qaul_wget_connection *myConn);

/**
 * send @a header via @a myConn connection
 *
 * @retval 1 successfully sent
 * @retval 0 connection error
 */
int Qaullib_WgetSendHeader(struct qaul_wget_connection *myConn, const char *header);

/**
 * loop through all sockets and check if something was received
 */
void Qaullib_WgetCheckSockets(void);

/**
 * check if connection @a myConn received something
 *
 * @returnval received Bytes
 */
int Qaullib_WgetReceive(struct qaul_wget_connection *myConn);


#ifdef __cplusplus
}
#endif // __cplusplus

#endif
