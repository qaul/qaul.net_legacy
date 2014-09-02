/*
 * qaul.net is free software
 * licensed under GPL (version 3)
 */

#include "qaullib_private.h"


/**
 * add message to data base
 *
 * @retval 0 on error
 * @retval 1 on success
 */
static int Qaullib_MsgAdd2DB(struct qaul_msg_LL_item *msg_item);


// ------------------------------------------------------------
void Qaullib_MsgInit(void)
{
	if(QAUL_DEBUG)
		printf("Qaullib_MsgInit\n");

	Qaullib_File_LL_Init();

	// get messages from DB
	Qaullib_MsgDB2LL();
}

// ------------------------------------------------------------
void Qaullib_MsgDB2LL(void)
{
	sqlite3_stmt *ppStmt;
	char *error_exec=NULL;
	struct qaul_msg_LL_item myitem;
	int jj;

	if(QAUL_DEBUG)
		printf("Qaullib_MsgDB2LL\n");

	// Select rows from database
	if( sqlite3_prepare_v2(db, sql_msg_get_latest, -1, &ppStmt, NULL) != SQLITE_OK )
	{
		printf("SQLite error: %s\n", sqlite3_errmsg(db));
		return;
	}

	while (sqlite3_step(ppStmt) == SQLITE_ROW)
	{
		// For each column
		for(jj=0; jj < sqlite3_column_count(ppStmt); jj++)
		{
			if(strcmp(sqlite3_column_name(ppStmt,jj), "id") == 0)
			{
		    	myitem.id = sqlite3_column_int(ppStmt, jj);
			}
		    else if(strcmp(sqlite3_column_name(ppStmt,jj), "type") == 0)
			{
		    	myitem.type = sqlite3_column_int(ppStmt, jj);
			}
			else if(strcmp(sqlite3_column_name(ppStmt,jj), "name") == 0)
			{
				sprintf(myitem.name, "%s", sqlite3_column_text(ppStmt, jj));
			}
			else if(strcmp(sqlite3_column_name(ppStmt,jj), "msg") == 0)
			{
				sprintf(myitem.msg, "%s", sqlite3_column_text(ppStmt, jj));
			}
			else if(strcmp(sqlite3_column_name(ppStmt,jj), "ipv") == 0)
			{
		    	myitem.ipv = sqlite3_column_int(ppStmt, jj);
			}
			else if(strcmp(sqlite3_column_name(ppStmt,jj), "ip") == 0)
			{
				sprintf(myitem.ip, "%s", sqlite3_column_text(ppStmt, jj));
			}
			else if(strcmp(sqlite3_column_name(ppStmt,jj), "time") == 0)
			{
		    	myitem.time = sqlite3_column_int(ppStmt, jj);
			}
			else if(strcmp(sqlite3_column_name(ppStmt,jj), "read") == 0)
			{
		    	myitem.read = sqlite3_column_int(ppStmt, jj);
			}
		}

		printf("Qaullib_MsgDB2LL 2; id: %i\n", myitem.id);

		// add it to LL
		Qaullib_Msg_LL_Add(&myitem);

		printf("Qaullib_MsgDB2LL 3\n");
	}
	printf("Qaullib_MsgDB2LL 4\n");

	sqlite3_finalize(ppStmt);

	printf("Qaullib_MsgDB2LL end\n");
}

// ------------------------------------------------------------
int Qaullib_MsgDB2LLTmp(struct qaul_msg_LL_node *node, char *stmt)
{
	sqlite3_stmt *ppStmt;
	char *error_exec=NULL;
	struct qaul_msg_LL_item myitem;
	struct qaul_msg_LL_node tmp_node;
	int count;

	if(QAUL_DEBUG)
		printf("Qaullib_MsgDB2LL\n");

	// Select rows from database
	if( sqlite3_prepare_v2(db, stmt, -1, &ppStmt, NULL) != SQLITE_OK )
	{
		printf("SQLite error: %s\n", sqlite3_errmsg(db));
		return 0;
	}

	count = 0;
	while (sqlite3_step(ppStmt) == SQLITE_ROW)
	{
		// For each column
		int jj;
		for(jj=0; jj < sqlite3_column_count(ppStmt); jj++)
		{
		    if(strcmp(sqlite3_column_name(ppStmt,jj), "id") == 0)
			{
		    	myitem.id = sqlite3_column_int(ppStmt, jj);
			}
		    else if(strcmp(sqlite3_column_name(ppStmt,jj), "type") == 0)
			{
		    	myitem.type = sqlite3_column_int(ppStmt, jj);
			}
			else if(strcmp(sqlite3_column_name(ppStmt,jj), "name") == 0)
			{
				sprintf(myitem.name, "%s", sqlite3_column_text(ppStmt, jj));
			}
			else if(strcmp(sqlite3_column_name(ppStmt,jj), "msg") == 0)
			{
				sprintf(myitem.msg, "%s", sqlite3_column_text(ppStmt, jj));
			}
			else if(strcmp(sqlite3_column_name(ppStmt,jj), "ipv") == 0)
			{
		    	myitem.ipv = sqlite3_column_int(ppStmt, jj);
			}
			else if(strcmp(sqlite3_column_name(ppStmt,jj), "ip") == 0)
			{
				sprintf(myitem.ip, "%s", sqlite3_column_text(ppStmt, jj));
			}
			else if(strcmp(sqlite3_column_name(ppStmt,jj), "time") == 0)
			{
		    	myitem.time = sqlite3_column_int(ppStmt, jj);
			}
			else if(strcmp(sqlite3_column_name(ppStmt,jj), "read") == 0)
			{
		    	myitem.read = sqlite3_column_int(ppStmt, jj);
			}
		}

		// add it to LL
		Qaullib_Msg_LL_AddTmp(&myitem, &tmp_node);

		if(count == 0)
			node->item = tmp_node.item;

		count++;
	}
	sqlite3_finalize(ppStmt);

	if(count > 0)
		return 1;

	return 0;
}

// ------------------------------------------------------------
int Qaullib_MsgAdd(struct qaul_msg_LL_item *item)
{
	struct qaul_msg_LL_item msg_item;

	if(QAUL_DEBUG)
		printf("Qaullib_MsgAdd\n");

	// protect & validate item values
	msg_item.id = item->id;
	msg_item.type = item->type;
	Qaullib_StringMsgProtect(msg_item.msg, item->msg, sizeof(msg_item.msg));
	Qaullib_StringNameProtect(msg_item.name, item->name, sizeof(msg_item.name));
	msg_item.time = item->time;
	msg_item.read = item->read;
	msg_item.ipv = item->ipv;
	strncpy(msg_item.ip, item->ip, sizeof(msg_item.ip));
	memcpy(&msg_item.ip_union, &item->ip_union, sizeof(msg_item.ip_union));

	// add to DB
	Qaullib_MsgAdd2DB(&msg_item);

	// add to LL
	Qaullib_Msg_LL_Add(&msg_item);

	// set new message flag
	if(msg_item.type != QAUL_MSGTYPE_VOIP_IN && msg_item.type != QAUL_MSGTYPE_VOIP_OUT)
		qaul_new_msg++;

	// check if user is in data base
	if(msg_item.type == QAUL_MSGTYPE_PUBLIC_IN && msg_item.type == QAUL_MSGTYPE_PRIVATE_IN)
	{
		Qaullib_UserCheckUser(&msg_item.ip_union, msg_item.name);
	}

	return 1;
}

// ------------------------------------------------------------
int Qaullib_MsgSendPublic(struct qaul_msg_LL_item *item)
{
	char buffer[1024];
	union olsr_message *m;
	int size;

	m = (union olsr_message *)buffer;

	// pack chat into olsr message
	// TODO: ipv6
	m->v4.olsr_msgtype = QAUL_CHAT_MESSAGE_TYPE;
	memcpy(&m->v4.message.chat.name, item->name, MAX_USER_LEN);
	memcpy(&m->v4.message.chat.msg, item->msg, MAX_MESSAGE_LEN);
	size = sizeof(struct qaul_chat_msg);
	size = size + sizeof(struct olsrmsg);
	m->v4.olsr_msgsize = htons(size);

	// send package
	Qaullib_IpcSend(m);

	// save message
	Qaullib_MsgAdd(item);

	return 1;
}

// ------------------------------------------------------------
int Qaullib_MsgSendPublicWeb(struct qaul_msg_LL_item *item)
{
	char buffer[1024];
	union olsr_message *m;
	int size;

	m = (union olsr_message *)buffer;

	// pack chat into olsr message
	// TODO: ipv6
	m->v4.olsr_msgtype = QAUL_CHAT_MESSAGE_TYPE;
	memcpy(&m->v4.message.chat.name, item->name, MAX_USER_LEN);
	memcpy(&m->v4.message.chat.msg, item->msg, MAX_MESSAGE_LEN);
	size = sizeof(struct qaul_chat_msg);
	size = size + sizeof(struct olsrmsg);
	m->v4.olsr_msgsize = htons(size);

	// send package
	Qaullib_IpcSend(m);

	// save message
	Qaullib_MsgAdd(item);

	return 1;
}

// ------------------------------------------------------------
int Qaullib_MsgSendPrivate(struct qaul_msg_LL_item *item)
{
	// save message
	Qaullib_MsgAdd(item);

	return 1;
}

// ------------------------------------------------------------
int Qaullib_MsgReceivePublic(struct qaul_msg_LL_item *item)
{

}

// ------------------------------------------------------------
int Qaullib_MsgReceivePrivate(struct qaul_msg_LL_item *item)
{

}


// ------------------------------------------------------------
// helper functions
// ------------------------------------------------------------
static int Qaullib_MsgAdd2DB(struct qaul_msg_LL_item *item)
{
	char buffer[1024];
	char *stmt;
	char *error_exec;
	char msg_dbprotected[2*MAX_MESSAGE_LEN +1];
	char name_dbprotected[2*MAX_USER_LEN +1];

	error_exec = NULL;
	stmt = buffer;

	if(QAUL_DEBUG)
		printf("Qaullib_MsgAdd2DB\n");

	// protect message for data base
	Qaullib_StringDbProtect(msg_dbprotected, item->msg, sizeof(msg_dbprotected));
	Qaullib_StringDbProtect(name_dbprotected, item->name, sizeof(name_dbprotected));

  	// save Message to database
	sprintf(stmt,
			sql_msg_set,
			item->type,
			name_dbprotected,
			msg_dbprotected,
			item->ip,
			item->ipv,
			item->time,
			item->read
	);

	if(QAUL_DEBUG)
		printf("statement: %s\n", stmt);

	if(sqlite3_exec(db, stmt, NULL, NULL, &error_exec) != SQLITE_OK)
	{
		// execution failed
		printf("SQLite error: %s\n",error_exec);
		sqlite3_free(error_exec);
		error_exec=NULL;
		return 0;
	}

	// get message id
	item->id = (int)sqlite3_last_insert_rowid(db);

	return 1;
}
