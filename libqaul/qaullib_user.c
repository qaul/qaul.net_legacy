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
		userconnections[i].conn.type = QAUL_WGET_USER;

		// fill in socket defaults
		// FIXME: ipv6
		userconnections[i].conn.ip.sin_family = AF_INET;
		userconnections[i].conn.ip.sin_port = htons(WEB_PORT);

		// start thread
		qaullib_pthread_start((qaullib_thread_func_t) Qaullib_WgetRunThread, &userconnections[i].conn);
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
		if(user->type < QAUL_USERTYPE_KNOWN)
		{
			// set user name
			strncpy(user->name, name, MAX_USER_LEN);
			memcpy(&user->name[MAX_USER_LEN], "\0", 1);

			// hide empty users
			if(strlen(user->name) > 0)
			{
				user->type = QAUL_USERTYPE_KNOWN;
				user->changed = QAUL_USERCHANGED_MODIFIED;
			}
			else
			{
				user->type = QAUL_USERTYPE_HIDDEN;
			}
		}
		else if(user->changed >= QAUL_USERCHANGED_DELETED)
		{
			if(user->changed == QAUL_USERCHANGED_DELETED)
				user->changed = QAUL_USERCHANGED_UNCHANGED;
			else
				user->changed = QAUL_USERCHANGED_MODIFIED;
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

		// hide empty users
		if(strlen(user->name) > 0)
		{
			user->type = QAUL_USERTYPE_KNOWN;
			user->changed = QAUL_USERCHANGED_MODIFIED;
		}
		else
		{
			user->type = QAUL_USERTYPE_HIDDEN;
		}
	}
	return user_found;
}

// ------------------------------------------------------------
void Qaullib_UserTouchIp(union olsr_ip_addr *ip, float linkcost)
{
	struct qaul_user_LL_item *user;

	if(QAUL_DEBUG)
		printf("Qaullib_UserTouchIp \n");

	// check if user exists in LL
	if(Qaullib_User_LL_IpSearch (ip, &user))
	{
		if(QAUL_DEBUG)
			printf("user exists \n");

		// if user exists: update lastseen_at
		if(user->changed >= QAUL_USERCHANGED_DELETED)
		{
			if(user->type == QAUL_USERTYPE_KNOWN)
				user->changed = QAUL_USERCHANGED_MODIFIED;
			else
				user->changed = QAUL_USERCHANGED_UNCHANGED;
		}

		// set link cost
		if(linkcost > 20.0)
		{
			// delete this user
			if(user->changed < QAUL_USERCHANGED_DELETED)
				user->changed = QAUL_USERCHANGED_DELETED;
		}
		else
		{
			if( user->changed >= QAUL_USERCHANGED_DELETED &&
				linkcost <= 10.0)
			{
				user->changed = QAUL_USERCHANGED_MODIFIED;
			}
			else if(Qaullib_UserLinkcost2Img(user->lq) != Qaullib_UserLinkcost2Img(linkcost))
			{
				user->changed = QAUL_USERCHANGED_MODIFIED;
			}
		}
		user->lq = linkcost;

		// set last seen time stamp
		user->time = time(NULL);
	}
	else
	{
		if(QAUL_DEBUG)
			printf("user does not exist, create it \n");

		// if user does not exist: create user
		user = Qaullib_User_LL_Add (ip);
		user->lq = linkcost;
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
		if(mynode.item->type == QAUL_USERTYPE_UNCHECKED)
			if(Qaullib_UserGetInfo(mynode.item) == 0)
				break;
	}
}

// ------------------------------------------------------------
int Qaullib_UserGetInfo(struct qaul_user_LL_item *user)
{
	int i, success;
	printf("[qaullib] Qaullib_UserGetInfo\n");

	// check for user name if there is a free connection
	for(i=0; i<MAX_USER_CONNECTIONS; i++)
	{
		if(userconnections[i].conn.connected == 0)
		{
			userconnections[i].conn.received = 0;
			userconnections[i].conn.bufpos = 0;
			userconnections[i].user = user;
			// fill in address
			// FIXME: ipv6
			userconnections[i].conn.ip.sin_addr.s_addr = user->ip.v4.s_addr;

			// set header
			strcpy(userconnections[i].conn.header, "GET /pub_users HTTP/1.1\r\n\r\n");

			// set connection reference
			userconnections[i].conn.download_ref = (void *)&userconnections[i];

			// set user to downloading
			userconnections[i].user->type = QAUL_USERTYPE_DOWNLOADING;

			// set connection flag
			userconnections[i].conn.connected = 1;

			return 1;
		}
	}
	return 0;
}

// ------------------------------------------------------------
int Qaullib_UserDownloadProcess(struct qaul_user_connection *userconnection, int bytes)
{
	int bufpos;

	if(QAUL_DEBUG)
		printf("Qaullib_UserDownloadProcess\n");

	bufpos = 0;

	if(bytes >= sizeof(struct qaul_userinfo_msg))
	{
		printf("Qaullib_UserDownloadProcess received\n");
		// check for first info (usually requested user)
		if(memcmp(&userconnection->user->ip, &userconnection->conn.buf.userinfo.ip, sizeof(union olsr_ip_addr)) == 0)
		{
			printf("Qaullib_UserDownloadProcess first is asked client\n");

			strncpy(userconnection->user->name, userconnection->conn.buf.userinfo.name, MAX_USER_LEN);
			memcpy(&userconnection->user->name[MAX_USER_LEN], "\0", 1);

			if(QAUL_DEBUG)
				printf("Qaullib_UserDownloadProcess first user %s\n", userconnection->user->name);

			// hide empty users
			if(strlen(userconnection->user->name) > 0)
			{
				userconnection->user->type = QAUL_USERTYPE_KNOWN;
				userconnection->user->changed = QAUL_USERCHANGED_MODIFIED;
			}
			else
			{
				userconnection->user->type = QAUL_USERTYPE_HIDDEN;
			}
		}
		else
			Qaullib_UserAddInfo(&userconnection->conn.buf.userinfo);

		// check all further users
		bufpos = sizeof(struct qaul_userinfo_msg);
		while(bytes -bufpos >= sizeof(struct qaul_userinfo_msg))
		{
			if(QAUL_DEBUG)
				printf("Qaullib_UserDownloadProcess further user info\n");

			// process information
			Qaullib_UserAddInfo((struct qaul_userinfo_msg *)&userconnection->conn.buf.buf[bufpos]);
			// set new bufpos
			bufpos += sizeof(struct qaul_userinfo_msg);
		}

		// todo: do this for more users
		return 0;
	}
	else if(bytes > 0)
	{
		//userconnection->conn.bufpos += bytes;
		Qaullib_UserDownloadFailed(userconnection);
		return 0;
	}
	else if(userconnection->conn.received < sizeof(struct qaul_userinfo_msg))
	{
		Qaullib_UserDownloadFailed(userconnection);
		return 0;
	}
	else
		return 0;
	return 1;
}

// ------------------------------------------------------------
void Qaullib_UserDownloadFailed(struct qaul_user_connection *userconnection)
{
	if(QAUL_DEBUG)
		printf("Qaullib_UserDownloadFailed\n");

	// mark user as node
	if(userconnection->user->type == QAUL_USERTYPE_DOWNLOADING)
		userconnection->user->type = QAUL_USERTYPE_ERROR;
}

/*
// ------------------------------------------------------------
void Qaullib_UserCheckSockets(void)
{
	int i, bytes, bufpos;

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
				if(userconnections[i].user->type == QAUL_USERTYPE_DOWNLOADING)
					userconnections[i].user->type = QAUL_USERTYPE_ERROR;
			}
			else if(bytes >= sizeof(struct qaul_userinfo_msg))
			{
				printf("Qaullib_UserCheckSockets received\n");
				// check for first info (usually requested user)
				if(memcmp(&userconnections[i].user->ip, &userconnections[i].conn.buf.userinfo.ip, sizeof(union olsr_ip_addr)) == 0)
				{
					printf("Qaullib_UserCheckSockets first is asked client\n");
					if(userconnections[i].user->type == QAUL_USERTYPE_DOWNLOADING)
					{
						strncpy(userconnections[i].user->name, userconnections[i].conn.buf.userinfo.name, MAX_USER_LEN);
						memcpy(&userconnections[i].user->name[MAX_USER_LEN], "\0", 1);

						// hide empty users
						if(strlen(userconnections[i].user->name) > 0)
						{
							userconnections[i].user->type = QAUL_USERTYPE_KNOWN;
							userconnections[i].user->changed = QAUL_USERCHANGED_MODIFIED;
						}
						else
						{
							userconnections[i].user->type = QAUL_USERTYPE_HIDDEN;
						}
					}
				}
				else
					Qaullib_UserAddInfo(&userconnections[i].conn.buf.userinfo);

				// check all further users
				bufpos = sizeof(struct qaul_userinfo_msg);
				while(bytes -bufpos >= sizeof(struct qaul_userinfo_msg))
				{
					printf("Qaullib_UserCheckSockets further user info\n");
					// process information
					Qaullib_UserAddInfo((struct qaul_userinfo_msg *)&userconnections[i].conn.buf.buf[bufpos]);
					// set new bufpos
					bufpos += sizeof(struct qaul_userinfo_msg);
				}
				// close connection
				Qaullib_WgetClose(&userconnections[i].conn);
			}
			else if(bytes > 0)
				userconnections[i].conn.bufpos += bytes;
		}
	}
}
*/

void Qaullib_UserAddInfo(struct qaul_userinfo_msg *userinfo)
{
	Qaullib_UserAdd(	&userinfo->ip,
						userinfo->name,
						userinfo->icon,
						userinfo->suffix);
}

void Qaullib_UserAdd(union olsr_ip_addr *ip, char *name, char *iconhash, char *suffix)
{
	struct qaul_user_LL_item *myuseritem;

	printf("Qaullib_UserAdd\n");

	// search for user
	if(Qaullib_User_LL_IpSearch (ip, &myuseritem))
	{
		printf("Qaullib_UserAddInfo user found in LL\n");

		// check if it is already known
		if(myuseritem->type < QAUL_USERTYPE_KNOWN)
		{
			printf("Qaullib_UserAddInfo name not known yet\n");
			// set type
			myuseritem->type = QAUL_USERTYPE_KNOWN;
			if(myuseritem->changed == QAUL_USERCHANGED_UNCHANGED)
				myuseritem->changed = QAUL_USERCHANGED_MODIFIED;
		}
		else
			return;
	}
	else
	{
		printf("Qaullib_UserAddInfo user not found in LL: create it\n");
		// create the user if it doesn't exist
		myuseritem = Qaullib_User_LL_Add (ip);
		// set user to cache
		myuseritem->changed = QAUL_USERCHANGED_CACHED;
	}

	// fill in name
	strncpy(myuseritem->name, name, MAX_USER_LEN);
	memcpy(&myuseritem->name[MAX_USER_LEN], "\0", 1);
	// todo: add icon info
	memcpy(&myuseritem->icon[0], "\0", 1);
	printf("Qaullib_User survived\n");

	if(strlen(myuseritem->name) > 0)
		myuseritem->type = QAUL_USERTYPE_KNOWN;
	else
		myuseritem->type = QAUL_USERTYPE_HIDDEN;

	if(QAUL_DEBUG)
		printf("Qaullib_UserAdd added: %s\n", name);
}

// ------------------------------------------------------------
void Qaullib_UserFavoriteAdd(char *name, char *ipstr)
{
	char buffer[1024];
	char name_dbprotected[MAX_USER_LEN*2 +1];
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
	Qaullib_StringDbProtect(name_dbprotected, name, sizeof(name_dbprotected));
	sprintf(stmt, sql_user_set_ipv4, name_dbprotected, "", myipint);
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
	  myitem->type = QAUL_USERTYPE_KNOWN;
	  myitem->favorite = 1;
	  myitem->changed = QAUL_USERCHANGED_CACHED;

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

// ------------------------------------------------------------
int Qaullib_UserLinkcost2Img(float linkcost)
{
	if(linkcost < 1.25)
		return 4;
	else if(linkcost < 1.6)
		return 3;
	else if(linkcost < 2.5)
		return 2;
	else if(linkcost < 5)
		return 1;
	else
		return 0;
}


