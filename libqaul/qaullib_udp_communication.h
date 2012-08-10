/*
 * qaul.net is free software
 * licensed under GPL (version 3)
 */

#ifndef _QAULLIB_UDP_COMMUNICATION
#define _QAULLIB_UDP_COMMUNICATION

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stdio.h>      // for printf
#include <stdlib.h>
#include <string.h>     // for string and memset etc


int qaul_UDP_socket;
int qaul_UDP_started;

/**
 * Start UDP Server on port 8083
 */
int  Qaullib_UDP_StartServer(void);

/**
 * Send the message to the
 */
void Qaullib_UDP_SendFileavailabeMsg(struct qaul_fileavailable_msg *msg, union olsr_ip_addr *ip);

/**
 * Send the message to the
 */
void Qaullib_UDP_SendExeavailabeMsg(struct qaul_exeavailable_msg *msg, union olsr_ip_addr *ip);

/**
 * Check for incoming messages
 */
void Qaullib_UDP_CheckSocket(void);


#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _QAULLIB_UDP_COMMUNICATION
