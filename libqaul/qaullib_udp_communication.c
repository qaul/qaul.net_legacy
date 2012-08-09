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
	int option;

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
	sendstatus = setsockopt(DNSSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&option, sizeof(option));
	//// bind a socket to a device name (might not work on all systems):
	//optval2 = "eth1"; // 4 bytes long, so 4, below:
	//setsockopt(s2, SOL_SOCKET, SO_BINDTODEVICE, optval2, 4);

	printf("UDP server started\n");
}

// ------------------------------------------------------------
void Qaullib_UDP_SendFileavailabeMsg(struct qaul_fileavailable_msg *msg, union olsr_ip_add *ip)
{
	struct sockaddr destAddr;
	int status;

	status = sendto(
					qaul_UDP_socket,
					msg,
					sizeof(struct qaul_fileavailable_msg),
					0,
					(struct sockaddr *)&destAddr,
					sizeof(destAddr)
					);

	if(status < 0)
		perror("Qaullib_UDP_SendFileavailabeMsg Error sending file available message: %i\n", status);
}

// ------------------------------------------------------------
void Qaullib_UDP_SendExeavailabeMsg(struct qaul_exeavailable_msg *msg, union olsr_ip_add *ip)
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



// takes server_ip in the following form: char server_ip = {}
void *Qaullib_DNS_Server(void *server_ip)
{
	char buf[1024];
	DNS_TYPE * DNS_Buffer = (DNS_TYPE *)buf;
	struct sockaddr_in myAddr;
	struct sockaddr_in sourceAddr;
	socklen_t socklen;
	char * buf_ptr;
	int status, sendstatus, option, qlen, alen;
	uint16_t dp_labelpointer = htons(0xc00c);
	uint16_t dp_type = htons(1);
	uint16_t dp_class = htons(1);
	uint32_t dp_ttl = htonl(60);
	uint16_t dp_len = htons(4);
	char     dp_ip[] = {10,0,0,1};
	memcpy( &dp_ip, server_ip, 4);
	//char     option[] = {0,0,0,1};
	static int DNSSocket = -1;

	memset(&myAddr,0,sizeof(myAddr));
	memset(&sourceAddr,0,sizeof(sourceAddr));

	myAddr.sin_family = AF_INET;
	myAddr.sin_port = htons(DNS_PORT); // 53 = DNS port // 8053 for firewall redirection
	myAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	DNSSocket = socket(PF_INET, SOCK_DGRAM, 0);
	if(DNSSocket == INVALID_SOCKET) printf("unable to create DNSSocket\n");

	status = bind(DNSSocket, (struct sockaddr *)&myAddr, sizeof(myAddr));
	if(status < 0) perror("bind DNSSocket error");

	// set socket options
	// set reuse flag
	option = 1;
	sendstatus = setsockopt(DNSSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&option, sizeof(option));
	//// bind a socket to a device name (might not work on all systems):
	//optval2 = "eth1"; // 4 bytes long, so 4, below:
	//setsockopt(s2, SOL_SOCKET, SO_BINDTODEVICE, optval2, 4);

	socklen = sizeof(struct sockaddr_in);

	printf("DNS server started\n");

	while ( 1 )
	{
		status = recvfrom( DNSSocket, buf, sizeof(buf), 0 , (struct sockaddr *)&sourceAddr, &socklen);

		if ( status > 0 )
		{
			alen = sizeof(DNS_TYPE);

			printf("DNS query received \n");
			//struct sockaddr_in *myaddr = (struct sockaddr_in *)&sourceAddr;
			//printf("DNS query from %s : %i, status = %i, minlen: %i\n", inet_ntoa(myaddr->sin_addr), myaddr->sin_port, status, alen);
			printf("questions: %i\n", (int)DNS_Buffer->dp_questions);
			//printf("dp_flags: %i\n", (int)DNS_Buffer->dp_flags);

			if(status > alen && DNS_Buffer->dp_flags & 0x8000 == 0x8000  &&  DNS_Buffer->dp_questions > 0)
			{
				// get name
				qlen = strlen(&buf[alen]);
				printf("query len = %i\n", qlen);
				printf("%s\n", &buf[alen]);

				if(qlen > 1 && qlen < 129)
				{
					// set response flags
					DNS_Buffer->dp_flags = htons(0x8180);
					DNS_Buffer->dp_questions = htons(1);
					DNS_Buffer->dp_answers = htons(1);
					DNS_Buffer->dp_authR = htons(0);
					DNS_Buffer->dp_additionalR = htons(0);

					// write answer
					alen += qlen +1+4;
					// compressed label
					memcpy( &buf[alen], &dp_labelpointer, 2);
					alen += 2;
					// type
					memcpy( &buf[alen], &dp_type, 2);
					alen += 2;
					// class
					memcpy( &buf[alen], &dp_class, 2);
					alen += 2;
					// ttl
					memcpy( &buf[alen], &dp_ttl, 4);
					alen += 4;
					// ip len
					memcpy( &buf[alen], &dp_len, 2);
					alen += 2;
					// ip
					memcpy( &buf[alen], &dp_ip, 4);
					alen += 4;

					sendstatus = sendto(DNSSocket,
										buf,
										alen,
										0,
										(struct sockaddr *)&sourceAddr,
										sizeof(sourceAddr));
					if(sendstatus < 0) perror("sendto error");
				}
			}
		}

#ifdef _WIN32
		Sleep(100);
#endif
	}
}

