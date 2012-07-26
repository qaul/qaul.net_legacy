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

	// fill in the favorites
	Qaullib_UserFavoritesDB2LL();
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
		if(user->type < 2)
		{
			// set user name
			strncpy(user->name, name, MAX_USER_LEN);
			memcpy(&user->name[MAX_USER_LEN], "\0", 1);
			user->type = 2;
			user->changed = 1;
		}
		else if(user->changed >= 2)
		{
			if(user->changed == 2) user->changed = 0;
			else user->changed = 1;
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

// ------------------------------------------------------------
void Qaullib_UserFavoriteAdd(char *name, char *ipstr)
{
	char buffer[1024];
	char *stmt = buffer;
	char *error_exec=NULL;
	union olsr_ip_addr myip;
	struct qaul_user_LL_item *myitem;
	int myipint;

	// create ip
	if ( inet_pton(AF_INET, ipstr, &myip.v4) == 0 )
	{
		printf("inet_pton() ipv4 failed");
		return;
	}

	// change it at user LL
	if( Qaullib_User_LL_IpSearch (&myip, &myitem) )
		myitem->favorite = 0;

	// add it to DB
	memcpy(&myipint, &myip.v4, sizeof(int));
	sprintf(stmt, sql_user_set_ipv4, name, "", myipint);
	if(sqlite3_exec(db, stmt, NULL, NULL, &error_exec) != SQLITE_OK)
	{
		printf("SQLite error: %s\n",error_exec);
		sqlite3_free(error_exec);
		error_exec=NULL;
	}
}

// ------------------------------------------------------------
void Qaullib_UserFavoriteRemove(char *ipstr)
{
	char buffer[1024];
	char *stmt = buffer;
	char *error_exec=NULL;
	union olsr_ip_addr myip;
	struct qaul_user_LL_item *myitem;
	int myipint;

	// create ip
	if ( inet_pton(AF_INET, ipstr, &myip.v4) == 0 )
	{
		printf("inet_pton() ipv4 failed");
		return;
	}

	// change it at user LL
	if( Qaullib_User_LL_IpSearch (&myip, &myitem) )
		myitem->favorite = 0;

	// remove it from DB
	memcpy(&myipint, &myip.v4, sizeof(int));
	sprintf(stmt, sql_user_delete_ipv4, myipint);
	if(sqlite3_exec(db, stmt, NULL, NULL, &error_exec) != SQLITE_OK)
	{
		printf("SQLite error: %s\n",error_exec);
		sqlite3_free(error_exec);
		error_exec=NULL;
	}
}

// ------------------------------------------------------------
void Qaullib_UserFavoritesDB2LL(void)
{
	sqlite3_stmt *ppStmt;
	union olsr_ip_addr myip;
	struct qaul_user_LL_item *myitem;
	int myipint;

	// Select rows from database
	if( sqlite3_prepare_v2(db, sql_user_get_all, -1, &ppStmt, NULL) != SQLITE_OK )
	{
		printf("SQLite error: %s\n",sqlite3_errmsg(db));
		return;
	}

	// For each row returned
	while (sqlite3_step(ppStmt) == SQLITE_ROW)
	{
	  // For each column
	  int jj;
	  // search for ip
	  for(jj=0; jj < sqlite3_column_count(ppStmt); jj++)
	  {
			if(strcmp(sqlite3_column_name(ppStmt,jj), "ipv4") == 0)
			{
				myipint = sqlite3_column_int(ppStmt, jj);
				memcpy(&myip.v4, &myipint, sizeof(struct sockaddr_in));
				break;
			}
	  }

	  myitem = Qaullib_User_LL_Add(&myip);
	  myitem->type = 2;
	  myitem->favorite = 1;
	  myitem->changed = 3;

	  // fill in rest
	  for(jj=0; jj < sqlite3_column_count(ppStmt); jj++)
	  {
			if(strcmp(sqlite3_column_name(ppStmt,jj), "name") == 0)
			{
				sprintf(myitem->name,"%s",sqlite3_column_text(ppStmt, jj));
				break;
			}
	  }
	}
	sqlite3_finalize(ppStmt);
}
