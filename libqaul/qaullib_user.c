/*
 * qaul.net is free software
 * licensed under GPL (version 3)
 */

#include "qaullib_private.h"

void Qaullib_UserInit(void)
{
	Qaullib_User_LL_Init();

	// initialize the connection array
	int i;
	for(i=0; i<MAX_USER_CONNECTIONS; i++)
	{
		userconnections[i].conn.connected = 0;

		// fill in socket defaults
		// FIXME: ipv6
		userconnections[i].conn.ip.sin_family = AF_INET;
		userconnections[i].conn.ip.sin_port = htons(WEB_PORT);
	}
}

// ------------------------------------------------------------
int Qaullib_UserCheckUser(union olsr_ip_addr *ip, char *name)
{
	struct qaul_user_LL_item *user;
	int user_found = 0;

	// check if user exists in LL
	if(Qaullib_User_LL_IpSearch (ip, &user))
	{
		// if user exists: update lastseen_at
		if(user->type < 1)
		{
			// set user name
			strncpy(user->name, name, MAX_USER_LEN);
			memcpy(&user->name[MAX_USER_LEN], "\0", 1);
			user->type = 2;
			user->changed = 1;
		}
		else if(user->changed == 2)
		{
			if(user->type == 2) user->changed = 1;
			else user->changed = 0;
		}

		user->time = time(NULL);
		user_found = 1;
	}
	else
	{
		// if user does not exist: create user
		user = Qaullib_User_LL_Add (ip);
		// set user name
		strncpy(user->name, name, MAX_USER_LEN);
		memcpy(&user->name[MAX_USER_LEN], "\0", 1);
		user->type = 2;
		user->changed = 1;
	}
	return user_found;
}

// ------------------------------------------------------------
void Qaullib_UserTouchIp(union olsr_ip_addr *ip)
{
	struct qaul_user_LL_item *user;

	// check if user exists in LL
	if(Qaullib_User_LL_IpSearch (ip, &user))
	{
		// if user exists: update lastseen_at
		if(user->changed == 2)
		{
			if(user->type == 2) user->changed = 1;
			else user->changed = 0;
		}

		user->time = time(NULL);
	}
	else
	{
		// if user does not exist: create user
		user = Qaullib_User_LL_Add (ip);
	}
}


// ------------------------------------------------------------
void Qaullib_UserCheckNonames(void)
{
	//printf("[LL next] Qaullib_UserCheckNonames\n");

	struct qaul_user_LL_node mynode;
	Qaullib_User_LL_InitNode(&mynode);
	while(Qaullib_User_LL_NextNode(&mynode))
	{
		if(mynode.item->type == 0)
			Qaullib_UserGetInfo(mynode.item);
	}
}

// ------------------------------------------------------------
void Qaullib_UserGetInfo(struct qaul_user_LL_item *user)
{
	int i, success;
	printf("[qaullib] Qaullib_UserGetInfo\n");

	// check for username if there is a free connection
	for(i=0; i<MAX_USER_CONNECTIONS; i++)
	{
		if(userconnections[i].conn.connected == 0)
		{
			userconnections[i].conn.bufpos = 0;
			userconnections[i].user = user;
			// fill in address
			// FIXME: ipv6
			userconnections[i].conn.ip.sin_addr.s_addr = user->ip.v4.s_addr;

			// connect
			success = Qaullib_WgetConnect(&userconnections[i].conn);
			// send http header
			if(success == 1)
			{
				if(Qaullib_WgetSendHeader(&userconnections[i].conn, "GET /pub_users HTTP/1.1\r\n\r\n"))
				{
					user->type = 1;
				}
				else
					user->type = -1;
			}
			else
			{
				printf("[qaullib] connection error %i\n", success);
				user->type = -1;
			}

			break;
		}
	}
}

// ------------------------------------------------------------
void Qaullib_UserCheckSockets(void)
{
	int i, bytes;
	// check user sockets
	for(i=0; i<MAX_USER_CONNECTIONS; i++)
	{
		if(userconnections[i].conn.connected)
		{
			bytes = Qaullib_WgetReceive(&userconnections[i].conn);
			if(bytes <= 0)
			{
				printf("[qaullib] user connection error\n");
				// mark user as node
				userconnections[i].user->type = -1;
			}
			else if(bytes >= sizeof(struct qaul_userinfo_msg))
			{
				// set user name
				strncpy(userconnections[i].user->name, userconnections[i].conn.buf.userinfo.name, MAX_USER_LEN);
				memcpy(&userconnections[i].user->name[MAX_USER_LEN], "\0", 1);
				// set icon name
				strncpy(userconnections[i].user->icon, userconnections[i].conn.buf.userinfo.icon, MAX_FILENAME_LEN);
				memcpy(&userconnections[i].user->icon[MAX_FILENAME_LEN], "\0", 1);
				// set type
				userconnections[i].user->type = 2;
				userconnections[i].user->changed = 1;

				// close connection
				Qaullib_WgetClose(&userconnections[i].conn);
			}
			else if(bytes > 0) userconnections[i].conn.bufpos += bytes;
		}
	}
}

