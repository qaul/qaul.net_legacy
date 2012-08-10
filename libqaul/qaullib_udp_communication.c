/*
 * qaul.net is free software
 * licensed under GPL (version 3)
 */

/**
 * UDP server and client for file sharing messages
 */

#include "qaullib_private.h"

// ------------------------------------------------------------
int  Qaullib_UDP_StartServer(void)
{
	struct sockaddr_in myAddr;
	int option, option_status;

	qaul_UDP_socket = -1;
	qaul_UDP_started = 0;

	// todo: ipv6
	myAddr.sin_family = AF_INET;
	myAddr.sin_port = htons(UDP_PORT);
	myAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	qaul_UDP_socket = socket(PF_INET, SOCK_DGRAM, 0);

	if(qaul_UDP_socket == INVALID_SOCKET)
		printf("unable to create UDP socket\n");

	qaul_UDP_started = bind(qaul_UDP_socket, (struct sockaddr *)&myAddr, sizeof(myAddr));
	if(qaul_UDP_started < 0)
		perror("UDP socket bind error\n");

	// set socket options
	// set reuse flag
	option = 1;
	option_status = setsockopt(qaul_UDP_socket, SOL_SOCKET, SO_REUSEADDR, (const char*)&option, sizeof(option));
	//// bind a socket to a device name (might not work on all systems):
	//optval2 = "eth1"; // 4 bytes long, so 4, below:
	//setsockopt(s2, SOL_SOCKET, SO_BINDTODEVICE, optval2, 4);

	printf("UDP server started\n");

	return qaul_UDP_started;
}

// ------------------------------------------------------------
void Qaullib_UDP_SendFileavailabeMsg(struct qaul_fileavailable_msg *msg, union olsr_ip_addr *ip)
{
	struct sockaddr_in destAddr;
	int status;

	destAddr.sin_family = AF_INET;
	destAddr.sin_port = htons(UDP_PORT);
	// todo: ipv6
	destAddr.sin_addr.s_addr = htonl(ip->v4.s_addr);

	status = sendto(
					qaul_UDP_socket,
					msg,
					sizeof(struct qaul_fileavailable_msg),
					0,
					(struct sockaddr *)&destAddr,
					sizeof(destAddr)
					);

	if(status < 0)
		printf("Qaullib_UDP_SendFileavailabeMsg Error sending file available message: %i\n", status);
}

// ------------------------------------------------------------
void Qaullib_UDP_SendExeavailabeMsg(struct qaul_exeavailable_msg *msg, union olsr_ip_addr *ip)
{
	printf("Qaullib_UDP_SendExeavailabeMsg \n");
}

// ------------------------------------------------------------
void Qaullib_UDP_CheckSocket(void)
{
	char buffer[1024];
	struct qaul_fileavailable_msg *fileavailabe;
	struct qaul_exeavailable_msg *exeavailabe;
	struct sockaddr sourceAddr;
	int received;
	uint8_t msgtype;
	socklen_t socklen;

	socklen = sizeof(struct sockaddr_in);
	received = 1;
	while(received > 0)
	{
		received = recvfrom(
							qaul_UDP_socket,
							buffer,
							sizeof(buffer),
							0,
							(struct sockaddr *)&sourceAddr,
							&socklen
							);

		if(received > 0)
		{
			// check which message we received
			uint16_t msgtype = htons(fileavailabe->msgtype);

			if(msgtype == QAUL_FILEAVAILABLE_MESSAGE_TYPE)
			{
				// add discovery to LL

			}
			else if(msgtype == QAUL_EXEAVAILABLE_MESSAGE_TYPE)
			{
				// add exe to DB & LL

			}
		}
	}
}

