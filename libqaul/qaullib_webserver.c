/*
 * qaul.net is free software
 * licensed under GPL (version 3)
 */

#include "qaullib.h"
#include "qaullib_private.h"
#include "mongoose_extra.h"
#include "urlcode/urlcode.h"

#include <math.h>

// ------------------------------------------------------------
// static declarations
// ------------------------------------------------------------
static void Qaullib_WwwSetName(struct mg_connection *conn, const struct mg_request_info *request_info);
static void Qaullib_WwwSetLocale(struct mg_connection *conn, const struct mg_request_info *request_info);
static void Qaullib_WwwGetName(struct mg_connection *conn, const struct mg_request_info *request_info);

/**
 * Quit program
 */
static void Qaullib_WwwQuit(struct mg_connection *conn, const struct mg_request_info *request_info);

/**
 * process call handling
 */
static void Qaullib_WwwCallStart(struct mg_connection *conn, const struct mg_request_info *request_info);
static void Qaullib_WwwCallEnd(struct mg_connection *conn, const struct mg_request_info *request_info);
static void Qaullib_WwwCallAccept(struct mg_connection *conn, const struct mg_request_info *request_info);
static void Qaullib_WwwCallEvent(struct mg_connection *conn, const struct mg_request_info *request_info);
static void Qaullib_WwwSetPageName(struct mg_connection *conn, const struct mg_request_info *request_info);
static void Qaullib_WwwGetConfig(struct mg_connection *conn, const struct mg_request_info *request_info);

/**
 * get messages
 * different type searches
 * type: 1 (send all new messages)
 * type: 5 (search for users)
 * type: 6 (search for this )
 */
static void Qaullib_WwwGetMsgs(struct mg_connection *conn, const struct mg_request_info *request_info);
static void Qaullib_WwwSendMsg(struct mg_connection *conn, const struct mg_request_info *request_info);
static void Qaullib_WwwGetUsers(struct mg_connection *conn, const struct mg_request_info *request_info);
static void Qaullib_WwwGetEvents(struct mg_connection *conn, const struct mg_request_info *request_info);
static void Qaullib_WwwFileList(struct mg_connection *conn, const struct mg_request_info *request_info);
static void Qaullib_WwwFileAdd(struct mg_connection *conn, const struct mg_request_info *request_info);
static void Qaullib_WwwFilePick(struct mg_connection *conn, const struct mg_request_info *request_info);
static void Qaullib_WwwFilePickCheck(struct mg_connection *conn, const struct mg_request_info *request_info);
static void Qaullib_WwwFileOpen(struct mg_connection *conn, const struct mg_request_info *request_info);
static void Qaullib_WwwFileDelete(struct mg_connection *conn, const struct mg_request_info *request_info);
static void Qaullib_WwwFileSchedule(struct mg_connection *conn, const struct mg_request_info *request_info);
static void Qaullib_WwwPubUsers(struct mg_connection *conn, const struct mg_request_info *request_info);
static void Qaullib_WwwPubFilechunk(struct mg_connection *conn, const struct mg_request_info *request_info);
static void Qaullib_WwwPubMsg(struct mg_connection *conn, const struct mg_request_info *request_info);
static void Qaullib_WwwPubInfo(struct mg_connection *conn, const struct mg_request_info *request_info);
static void Qaullib_WwwExtBinaries(struct mg_connection *conn, const struct mg_request_info *request_info);
static void Qaullib_WwwLoading(struct mg_connection *conn, const struct mg_request_info *request_info);
static int  Qaullib_WwwGetMsgsCallback(void *NotUsed, int number_of_lines, char **column_value, char **column_name);
static void Qaullib_WwwJsonFiles(struct mg_connection *conn, const char* sqlStmt);
static void get_qsvar(const struct mg_request_info *request_info, const char *name, char *dst, size_t dst_len);


// ------------------------------------------------------------
// web server functions
// ------------------------------------------------------------
// event handler
void *Qaullib_WwwEvent_handler(enum mg_event event, struct mg_connection *conn, const struct mg_request_info *request_info)
{
  void *processed = "yes";

  if (event == MG_NEW_REQUEST) {
	// local jqm gui
	if (strcmp(request_info->uri, "/getmsgs.json") == 0)
	{
	  Qaullib_WwwGetMsgs(conn, request_info);
	}
	else if (strcmp(request_info->uri, "/getevents.json") == 0)
	{
	  Qaullib_WwwGetEvents(conn, request_info);
	}
	else if (strcmp(request_info->uri, "/getusers.json") == 0)
	{
	  Qaullib_WwwGetUsers(conn, request_info);
	}
	else if (strcmp(request_info->uri, "/sendmsg") == 0)
	{
	  Qaullib_WwwSendMsg(conn, request_info);
	}
	else if (strcmp(request_info->uri, "/getname") == 0)
    {
      Qaullib_WwwGetName(conn, request_info);
    }
    // call handling
	else if (strcmp(request_info->uri, "/call_event") == 0)
    {
      Qaullib_WwwCallEvent(conn, request_info);
    }
	else if (strcmp(request_info->uri, "/call_start") == 0)
    {
      Qaullib_WwwCallStart(conn, request_info);
    }
	else if (strcmp(request_info->uri, "/call_end") == 0)
    {
      Qaullib_WwwCallEnd(conn, request_info);
    }
	else if (strcmp(request_info->uri, "/call_accept") == 0)
    {
      Qaullib_WwwCallAccept(conn, request_info);
    }
    // file handling
	else if (strcmp(request_info->uri, "/file_list.json") == 0)
	{
	  Qaullib_WwwFileList(conn, request_info);
	}
	else if (strcmp(request_info->uri, "/file_add.json") == 0)
	{
	  Qaullib_WwwFileAdd(conn, request_info);
	}
	else if (strcmp(request_info->uri, "/file_pick.json") == 0)
	{
	  Qaullib_WwwFilePick(conn, request_info);
	}
	else if (strcmp(request_info->uri, "/file_pickcheck.json") == 0)
	{
	  Qaullib_WwwFilePickCheck(conn, request_info);
	}
	else if (strcmp(request_info->uri, "/file_open.json") == 0)
	{
	  Qaullib_WwwFileOpen(conn, request_info);
	}
	else if (strcmp(request_info->uri, "/file_delete.json") == 0)
	{
	  Qaullib_WwwFileDelete(conn, request_info);
	}
	else if (strcmp(request_info->uri, "/file_schedule.json") == 0)
	{
	  Qaullib_WwwFileSchedule(conn, request_info);
	}
	// configuration
	else if (strcmp(request_info->uri, "/getconfig.json") == 0)
	{
	  Qaullib_WwwGetConfig(conn, request_info);
	}
    else if (strcmp(request_info->uri, "/setlocale") == 0)
    {
      Qaullib_WwwSetLocale(conn, request_info);
    }
    else if (strcmp(request_info->uri, "/setname") == 0)
    {
      Qaullib_WwwSetName(conn, request_info);
    }
    else if (strcmp(request_info->uri, "/setpagename") == 0)
    {
      Qaullib_WwwSetPageName(conn, request_info);
    }
    else if (strcmp(request_info->uri, "/quit") == 0)
    {
    	Qaullib_WwwQuit(conn, request_info);
    }
	// other qaul users (pub = public)
    else if (strcmp(request_info->uri, "/pub_users") == 0)
    {
      Qaullib_WwwPubUsers(conn, request_info);
    }
    else if (strcmp(request_info->uri, "/pub_msg") == 0)
    {
      Qaullib_WwwPubMsg(conn, request_info);
    }
    else if (strcmp(request_info->uri, "/pub_info.json") == 0)
    {
      Qaullib_WwwPubInfo(conn, request_info);
    }
    else if (strcmp(request_info->uri, "/pub_filechunk") == 0)
    {
    	Qaullib_WwwPubFilechunk(conn, request_info);
    }
	// loading
    else if (strcmp(request_info->uri, "/loading.json") == 0)
    {
    	Qaullib_WwwLoading(conn, request_info);
    }
	// external access without qaul : qaul download & info pages (ext = external)
    else if (strcmp(request_info->uri, "/ext_binaries.json") == 0)
    {
    	Qaullib_WwwExtBinaries(conn, request_info);
    }
    else
    {
      // No suitable handler found, mark as not processed. Mongoose will
      // try to serve the request.
      processed = NULL;
    }
  }
  else
  {
    processed = NULL;
  }

  return processed;
}

// ------------------------------------------------------------
static void Qaullib_WwwSetName(struct mg_connection *conn, const struct mg_request_info *request_info)
{
	char *content_length;
	int length;

	// Fetch user name
	//get_qsvar(request_info, "n", qaul_username, sizeof(qaul_username));
	content_length = (char *)mg_get_header(conn, "Content-Length");
	length = atoi(content_length);
	char *post = (char *)malloc(length+length/8+1);
	mg_read(conn, post, length); //read post data
	// extract variable
	mg_get_var(post, strlen(post == NULL ? "" : post), "n", qaul_username, sizeof(qaul_username));

	printf("save username: ");
	printf("%s  \n", qaul_username);
	Qaullib_SetUsername(qaul_username);
	printf("username saved  \n", qaul_username);

	mg_printf(conn, "%s", "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=utf-8\r\n\r\n");

	free(post);
}

// ------------------------------------------------------------
static void Qaullib_WwwSetLocale(struct mg_connection *conn, const struct mg_request_info *request_info)
{
	char *content_length;
	int length;

	// Fetch locale
	content_length = (char *)mg_get_header(conn, "Content-Length");
	length = atoi(content_length);
	char *post = (char *)malloc(length+length/8+1);
	mg_read(conn, post, length); //read post data
	// extract variable
	mg_get_var(post, strlen(post == NULL ? "" : post), "l", qaul_locale, sizeof(qaul_locale));

	printf("save locale: %s\n", qaul_locale);
	Qaullib_SetLocale(qaul_locale);

	mg_printf(conn, "%s", "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=utf-8\r\n\r\n");

	free(post);
}

// ------------------------------------------------------------
static void Qaullib_WwwSetPageName(struct mg_connection *conn, const struct mg_request_info *request_info)
{
	// Fetch screen name
	get_qsvar(request_info, "p", qaullib_GuiPageName, sizeof(qaullib_GuiPageName));
	qaul_gui_pagename_set = 1;

	mg_printf(conn, "%s", "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=utf-8\r\n\r\n");
}

// ------------------------------------------------------------
static void Qaullib_WwwQuit(struct mg_connection *conn, const struct mg_request_info *request_info)
{
	app_event = 99;

	mg_printf(conn, "%s", "HTTP/1.1 200 OK\r\nContent-Type: application/json; charset=utf-8\r\n\r\n");
	mg_printf(conn, "{}");
}

// ------------------------------------------------------------
static void Qaullib_WwwCallEvent(struct mg_connection *conn, const struct mg_request_info *request_info)
{
	mg_printf(conn, "%s", "HTTP/1.1 200 OK\r\nContent-Type: application/json; charset=utf-8\r\n\r\n");
	mg_printf(conn, "{\"event\":%i", qaul_voip_event);
	if(qaul_voip_event == 5)
		mg_printf(conn, ",\"code\":%i", qaul_voip_event_code);
	mg_printf(conn, "}");
	qaul_voip_event = 0;
}

static void Qaullib_WwwCallStart(struct mg_connection *conn, const struct mg_request_info *request_info)
{
	char call_ip[MAX_IP_LEN +1];
	// extract variables
	get_qsvar(request_info, "ip", call_ip, sizeof(call_ip));
	// call user
	Qaullib_VoipCallStart(call_ip);

	mg_printf(conn, "%s", "HTTP/1.1 200 OK\r\nContent-Type: application/json; charset=utf-8\r\n\r\n");
	mg_printf(conn, "{}");
}

static void Qaullib_WwwCallEnd(struct mg_connection *conn, const struct mg_request_info *request_info)
{
	Qaullib_VoipCallEnd();

	mg_printf(conn, "%s", "HTTP/1.1 200 OK\r\nContent-Type: application/json; charset=utf-8\r\n\r\n");
	mg_printf(conn, "{}");
}

static void Qaullib_WwwCallAccept(struct mg_connection *conn, const struct mg_request_info *request_info)
{
	Qaullib_VoipCallAccept();

	mg_printf(conn, "%s", "HTTP/1.1 200 OK\r\nContent-Type: application/json; charset=utf-8\r\n\r\n");
	mg_printf(conn, "{}");
}

// ------------------------------------------------------------
static void Qaullib_WwwGetConfig(struct mg_connection *conn, const struct mg_request_info *request_info)
{
	// send header
	mg_printf(conn, "%s", "HTTP/1.1 200 OK\r\nContent-Type: application/json; charset=utf-8\r\n\r\n");

	mg_printf(conn, "{");

	// send UI language
	if(Qaullib_ExistsLocale()) mg_printf(conn, "\"locale\":\"%s\",", Qaullib_GetLocale());
	// send user name
	if(Qaullib_ExistsUsername()) mg_printf(conn, "\"name\":\"%s\",", Qaullib_GetUsername());

	// send all the rest
	// TODO: make configurable
	mg_printf(conn, "\"msg_max\":20,");
	if(qaul_conf_quit)
		mg_printf(conn, "\"c_quit\":true,");
	else
		mg_printf(conn, "\"c_quit\":false,");
	if(qaul_conf_voip)
		mg_printf(conn, "\"c_voip\":true,");
	else
		mg_printf(conn, "\"c_voip\":false,");
	mg_printf(conn, "\"c_debug\":false");

	mg_printf(conn, "}");
}


// ------------------------------------------------------------
static void Qaullib_WwwGetEvents(struct mg_connection *conn, const struct mg_request_info *request_info)
{
	// send header
	mg_printf(conn, "%s", "HTTP/1.1 200 OK\r\nContent-Type: application/json; charset=utf-8\r\n\r\n");

	mg_printf(conn, "{");

	// get number of waiting incoming messages
	mg_printf(conn, "\"m_pub\":%i,",0);
	mg_printf(conn, "\"m_priv\":%i,",0);

	// get newly downloaded files
	mg_printf(conn, "\"files\":%i,",0);

	// check call events
	if(qaul_voip_new_call)
	{
		mg_printf(conn, "\"call\":%i",1);
		mg_printf(conn, ",\"callee\":\"%s\"",qaul_voip_caller_name);
		qaul_voip_new_call = 0;
	}
	else
		mg_printf(conn, "\"call\":%i",0);

	mg_printf(conn, "}");
}


// ------------------------------------------------------------
static void Qaullib_WwwGetName(struct mg_connection *conn, const struct mg_request_info *request_info)
{
  // Fetch user name
  //printf("send username: %s \n", qaul_username);
  mg_printf(conn, "%s", "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=utf-8\r\n\r\n");
  mg_printf(conn, "%s", qaul_username);
}

// ------------------------------------------------------------
static void Qaullib_WwwGetMsgs(struct mg_connection *conn, const struct mg_request_info *request_info)
{
	sqlite3_stmt *ppStmt;
	char buffer[10240];
	char* stmt = buffer;
	char *error_exec=NULL;
	char local_type[MAX_INTSTR_LEN +1];
	char local_id[MAX_INTSTR_LEN +1];
	char local_tag[MAX_FILENAME_LEN +1];
	char local_name[MAX_USER_LEN +1];
	int type, id;
	char *content_length;
	char *post;
	int post_set = 0;
	int length = 0;

	//printf("[qaullib] Qaullib_WwwGetMsgs\n");

	mg_printf(conn, "%s", "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=utf-8\r\n\r\n");
	mg_printf(conn, "{\"name\":\"%s\",\"messages\":[", qaul_username);

	// get get variables
	// message type
	get_qsvar(request_info, "t", local_type, sizeof(local_type));
	type = atoi(local_type);

	if(type > 4)
	{
		// get id
		get_qsvar(request_info, "id", local_id, sizeof(local_id));
		id = atoi(local_id);
	}

	// prepare statements
	// user related
	if(type == 5)
	{
		get_qsvar(request_info, "v", local_name, sizeof(local_name));
/*
		// extract variable v
		mg_get_var(post, strlen(post == NULL ? "" : post), "v", local_name, sizeof(local_name));
*/
		// prepare statement
		if(id == 0)
			sprintf(stmt, sql_msg_get_user0, local_name, "%", local_name, "%");
		else
			sprintf(stmt, sql_msg_get_user, id, local_name, "%", local_name, "%");
	}
	// with tag
	else if(type == 6)
	{
		get_qsvar(request_info, "v", local_tag, sizeof(local_tag));
/*
		// extract variable v
		mg_get_var(post, strlen(post == NULL ? "" : post), "v", local_tag, sizeof(local_tag));
*/
		// prepare statement
		if(id == 0)
			sprintf(stmt, sql_msg_get_tag0, "%", local_tag, "%");
		else
			sprintf(stmt, sql_msg_get_tag, id, "%", local_tag, "%");
	}
	// get newest
	else
	{
		sprintf(stmt, "%s", sql_msg_get_new);
	}

	if(type > 4 || qaul_new_msg)
	{
		if(type == 1) qaul_new_msg = 0;

		// Select rows from database
		if( sqlite3_prepare_v2(db, stmt, -1, &ppStmt, NULL) != SQLITE_OK )
		{
			printf("SQLite error: %s\n",sqlite3_errmsg(db));
		}

		// For each row returned
		int first = 1;
		int myid;
		while (sqlite3_step(ppStmt) == SQLITE_ROW)
		{
			if(first) first = 0;
			else mg_printf(conn, "%s", ",");
			mg_printf(conn, "{");

		  // For each collumn
		  int jj;
		  for(jj=0; jj < sqlite3_column_count(ppStmt); jj++)
		  {
				if(strcmp(sqlite3_column_name(ppStmt,jj), "id") == 0)
				{
			    	if(jj>0) mg_printf(conn, ",");
			    	myid = sqlite3_column_int(ppStmt, jj);
			    	mg_printf(conn, "\"id\":%i",sqlite3_column_int(ppStmt, jj));
				}
				else if(strcmp(sqlite3_column_name(ppStmt,jj), "type") == 0)
				{
			    	if(jj>0) mg_printf(conn, ",");
			    	mg_printf(conn, "\"type\":%i",sqlite3_column_int(ppStmt, jj));
				}
				else if(strcmp(sqlite3_column_name(ppStmt,jj), "name") == 0)
				{
			    	if(jj>0) mg_printf(conn, ",");
			    	mg_printf(conn, "\"name\":\"%s\"",sqlite3_column_text(ppStmt, jj));
				}
				else if(strcmp(sqlite3_column_name(ppStmt,jj), "msg") == 0)
				{
			    	if(jj>0) mg_printf(conn, ",");
			    	mg_printf(conn, "\"msg\":\"%s\"",sqlite3_column_text(ppStmt, jj));
				}
				else if(strcmp(sqlite3_column_name(ppStmt,jj), "ip") == 0)
				{
			    	if(jj>0) mg_printf(conn, ",");
			    	mg_printf(conn, "\"ip\":\"%s\"",sqlite3_column_text(ppStmt, jj));
				}
				else if(strcmp(sqlite3_column_name(ppStmt,jj), "time") == 0)
				{
			    	if(jj>0) mg_printf(conn, ",");
			    	mg_printf(conn, "\"time\":\"%s\"",sqlite3_column_text(ppStmt, jj));
				}
		  }
			mg_printf(conn, "}");

			if(type == 1)
			{
				// update row
				sprintf(stmt, sql_msg_update_read, myid);
				//printf("stmt: %s\n",stmt);
				if(sqlite3_exec(db, stmt, NULL, NULL, &error_exec) != SQLITE_OK)
				{
					// execution failed
					printf("SQLite error: %s\n",error_exec);
					sqlite3_free(error_exec);
					error_exec=NULL;
				}
			}
		}
		sqlite3_finalize(ppStmt);
	}
	mg_printf(conn, "%s", "]}");

	// free memory
	if(post_set)
	{
		printf("free(post)\n");
		free(post);
	}
}

// ------------------------------------------------------------
static void Qaullib_WwwSendMsg(struct mg_connection *conn, const struct mg_request_info *request_info)
{
	char *content_length;
	int length;
	char buffer[10240];
	char* stmt = buffer;
	char *error_exec=NULL;
	char local_msg[MAX_MESSAGE_LEN +1];
	char local_name[MAX_USER_LEN +1];
	char local_type[3];
	int type;
	union olsr_message *m = (union olsr_message *)buffer;
	int size;

	printf("Qaullib_WwwSendMsg\n");

	// Fetch Message
	//get_qsvar(request_info, "m", qaul_msg, sizeof(qaul_msg));
	content_length = (char *)mg_get_header(conn, "Content-Length");
	length = atoi(content_length);
	char *post = (char *)malloc(length+length/8+1);
	mg_read(conn, post, length); //read post data
	// get type
	mg_get_var(post, strlen(post == NULL ? "" : post), "t", local_type, sizeof(local_type));
	type = atoi(local_type);
	printf("[qaullib] msg-type: %i",type);
	// get msg
	mg_get_var(post, strlen(post == NULL ? "" : post), "m", local_msg, sizeof(local_msg));
	// get name
	if(type == 12)
	{
		mg_get_var(post, strlen(post == NULL ? "" : post), "n", local_name, sizeof(local_name));
	}
	else memcpy(&local_name[0], "\0", 1);

	// todo: ipv6
  	// save Message to database
	sprintf(stmt, sql_msg_set_my,
		type,
		local_name,
		local_msg,
		"",
		4
	);

	//printf("statement: %s\n", stmt);
	if(sqlite3_exec(db, stmt, NULL, NULL, &error_exec) != SQLITE_OK)
	{
		// execution failed
		printf("SQLite error: %s\n",error_exec);
		sqlite3_free(error_exec);
		error_exec=NULL;
	}

	// pack chat into olsr message
	if(type == 11)
	{
		// ipv4 only at the moment
		m->v4.olsr_msgtype = 222;
		//m.v4.ttl = MAX_TTL;
		//m.v4.hopcnt = 0;
		memcpy(&m->v4.message.chat.name, qaul_username, MAX_USER_LEN);
		memcpy(&m->v4.message.chat.msg, local_msg, MAX_MESSAGE_LEN);
		size = sizeof( struct qaulchatmsg);
		size = size + sizeof(struct olsrmsg);
		m->v4.olsr_msgsize = htons(size);

		// send package
		Qaullib_IpcSend(m);
	}

	// todo: check wether sending was successful...
	// everything went fine
	//mg_printf(conn, "%s message: %s", "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=utf-8\r\n\r\n", qaul_msg);
	mg_printf(conn, "%s", "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=utf-8\r\n\r\n");

	// free memory
	free(post);

	printf("Qaullib_WwwSendMsg end\n");
}


// ------------------------------------------------------------
static void Qaullib_WwwGetUsers(struct mg_connection *conn, const struct mg_request_info *request_info)
{
	//printf("[LL next] Qaullib_WwwGetUsers\n");

	int first = 0;
	char ipbuf[MAX(INET6_ADDRSTRLEN, INET_ADDRSTRLEN)];

	mg_printf(conn, "%s", "HTTP/1.1 200 OK\r\nContent-Type: application/json; charset=utf-8\r\n\r\n");

	// print Users
	mg_printf(conn, "{\"users\":[");
	// loop through LL
	first = 0;
	struct qaul_user_LL_node mynode;
	Qaullib_User_LL_InitNode(&mynode);
	while(Qaullib_User_LL_NextNode(&mynode))
	{
		// TODO: only send changed items
		// check if node was changed
		if(mynode.item->type == 2)
		{
			if(!first) first = 1;
			else mg_printf(conn, ",");
			// FIXME: ipv6
			mg_printf(conn,
					"{\"id\":%i,\"name\":\"%s\",\"ip\":\"%s\",\"lq\":%f}",
					mynode.item->id,
					mynode.item->name,
					inet_ntop(AF_INET, &mynode.item->ip.v4.s_addr, (char *)&ipbuf, sizeof(ipbuf)),
					mynode.item->lq);
		}
	}
	mg_printf(conn, "]}");
}

// ------------------------------------------------------------
static void Qaullib_WwwFileList(struct mg_connection *conn, const struct mg_request_info *request_info)
{
	//printf("Qaullib_WwwFileList\n");
	mg_printf(conn, "%s", "HTTP/1.1 200 OK\r\nContent-Type: application/json; charset=utf-8\r\n\r\n");
	mg_printf(conn, "{");
	Qaullib_WwwJsonFiles(conn, sql_file_get_all);
	mg_printf(conn, "}");
}

// ------------------------------------------------------------
static void Qaullib_WwwFileAdd(struct mg_connection *conn, const struct mg_request_info *request_info)
{
	char *content_length;
	char buffer[10240];
	char* stmt = buffer;
	char *error_exec=NULL;
	char local_description[MAX_DESCRIPTION_LEN +1];
	char local_msg[MAX_MESSAGE_LEN +1];
	char local_path[MAX_PATH_LEN +1];
	char local_advertise[2];
	char local_hash[MAX_HASHSTR_LEN +1];
	char local_suffix[MAX_SUFFIX_LEN +1];
	union olsr_message *m = (union olsr_message *)buffer;
	int length, local_size, advertise, size;

	printf("Qaullib_WwwFileAdd\n");

	// extract post variables
	content_length = (char *)mg_get_header(conn, "Content-Length");
	length = atoi(content_length);
	char *post = (char *)malloc(length+length/8+1);
	mg_read(conn, post, length); //read post data

	printf("POST:\n%s\n", post);
	// get path
	mg_get_var(post, strlen(post == NULL ? "" : post), "p", local_path, sizeof(local_path));
	// get msg
	mg_get_var(post, strlen(post == NULL ? "" : post), "m", local_description, sizeof(local_description));
	// get advertise
	mg_get_var(post, strlen(post == NULL ? "" : post), "a", local_advertise, sizeof(local_advertise));
	advertise = atoi(local_advertise);

	printf("a: %i | %s, m: %s, p: %s\n", advertise, local_advertise, local_msg, local_path);

	// copy file into directory & make hash
	local_size = Qaullib_FileAdd(local_path, local_hash, local_suffix);
	printf("file size: %i\n", local_size);

	if(local_size > 0)
	{
		// write to db
		sprintf(stmt, sql_file_set,
			local_hash,
			local_suffix,
			local_description,
			local_size
		);
		printf("statement: %s\n", stmt);
		if(sqlite3_exec(db, stmt, NULL, NULL, &error_exec) != SQLITE_OK)
		{
			// execution failed
			printf("SQLite error: %s\n",error_exec);
			sqlite3_free(error_exec);
			error_exec=NULL;
		}

		// FIXME: make ipv6 compatible
		// pack chat into olsr message
		if(advertise)
		{
			printf("send advertise message\n");

			// ipv4 only at the moment
			m->v4.olsr_msgtype = 222;
			memcpy(&m->v4.message.chat.name, qaul_username, MAX_USER_LEN);
			// create message
			strcpy(local_msg, local_hash);
			if(strlen(local_suffix) > 0)
			{
				strcat(local_msg, ".");
				strcat(local_msg, local_suffix);
			}
			strcat(local_msg, " ");
			strcat(local_msg, local_description);

			memcpy(&m->v4.message.chat.msg, local_msg, MAX_MESSAGE_LEN);
			size = sizeof( struct qaulchatmsg);
			size = size + sizeof(struct olsrmsg);
			m->v4.olsr_msgsize = htons(size);

			printf("olsr message: name: %s, msg: %s, size:%i\n", qaul_username, local_msg, size);

			// send package
			Qaullib_IpcSend(m);
		}

		// todo: check wether sending was successful...
	}
	// send answer
	mg_printf(conn, "%s", "HTTP/1.1 200 OK\r\nContent-Type: application/json; charset=utf-8\r\n\r\n");
	mg_printf(conn, "{");
	mg_printf(conn, "\"success\":%i", 1);
	mg_printf(conn, "}");

	// free memory
	free(post);

	// check if any file needs to be downloaded
	Qaullib_FileCheckScheduled();
}

// ------------------------------------------------------------
static void Qaullib_WwwFilePick(struct mg_connection *conn, const struct mg_request_info *request_info)
{
	printf("Qaullib_WwwFilePick\n");

	mg_printf(conn, "%s", "HTTP/1.1 200 OK\r\nContent-Type: application/json; charset=utf-8\r\n\r\n");

	// open file picker
	pickFileCheck = 1;
	app_event = 100;

	// deliver answer
	mg_printf(conn, "{");
	mg_printf(conn, "}");
}

// ------------------------------------------------------------
static void Qaullib_WwwFilePickCheck(struct mg_connection *conn, const struct mg_request_info *request_info)
{
	printf("Qaullib_WwwFilePickCheck\n");

	mg_printf(conn, "%s", "HTTP/1.1 200 OK\r\nContent-Type: application/json; charset=utf-8\r\n\r\n");

	// deliver answer
	mg_printf(conn, "{\"picked\":%i", pickFileCheck);
	if(pickFileCheck == 2)
	{
		int backslash = '\\';
		if(strchr(pickFilePath, backslash))
		{
			mg_printf(conn, ",\"path\":\"");
			// protect backslashes
			int i;
			for(i=0; i<strlen(pickFilePath); i++)
			{
				if(pickFilePath[i] == backslash) mg_printf(conn, "\\");
				mg_printf(conn, "%c", pickFilePath[i]);
			}
			mg_printf(conn, "\",");
		}
		else
			mg_printf(conn, ",\"path\":\"%s\",", pickFilePath);
		char *local_file = strrchr(pickFilePath, PATH_SEPARATOR_INT);
		mg_printf(conn, "\"name\":\"%s\",", local_file+1);
		// FIXME: use correct size and date
		mg_printf(conn, "\"size\":1024,");
		mg_printf(conn, "\"create\":\"2012-02-02 19:59:42\"");
		pickFileCheck = 0;
	}
	mg_printf(conn, "}");
}

// ------------------------------------------------------------
static void Qaullib_WwwFileOpen(struct mg_connection *conn, const struct mg_request_info *request_info)
{
	char local_filename[MAX_FILENAME_LEN +1];

	printf("Qaullib_WwwFileOpen\n");

	// get file variable
	get_qsvar(request_info, "f", local_filename, MAX_FILENAME_LEN);
	memcpy(&local_filename[MAX_FILENAME_LEN], "\0", 1);
	// create path
    strcpy(qaullib_AppEventOpenPath, webPath);
    strcat(qaullib_AppEventOpenPath, PATH_SEPARATOR);
    strcat(qaullib_AppEventOpenPath, "files");
    strcat(qaullib_AppEventOpenPath, PATH_SEPARATOR);
    strcat(qaullib_AppEventOpenPath, local_filename);

	// open file
	app_event = 101;

	// deliver answer
	mg_printf(conn, "%s", "HTTP/1.1 200 OK\r\nContent-Type: application/json; charset=utf-8\r\n\r\n");
	mg_printf(conn, "{");
	mg_printf(conn, "}");
}

// ------------------------------------------------------------
static void Qaullib_WwwFileDelete(struct mg_connection *conn, const struct mg_request_info *request_info)
{
	char local_id[MAX_INTSTR_LEN +1];
	int id, success;

	printf("Qaullib_WwwFileDelete\n");

	// get file variable
	get_qsvar(request_info, "id", local_id, sizeof(local_id));
	id = atoi(local_id);

	// delete file
	success = Qaullib_FileDeleteById(id);

	// deliver answer
	mg_printf(conn, "%s", "HTTP/1.1 200 OK\r\nContent-Type: application/json; charset=utf-8\r\n\r\n");
	mg_printf(conn, "{");
	mg_printf(conn, "}");
}

// ------------------------------------------------------------
static void Qaullib_WwwFileSchedule(struct mg_connection *conn, const struct mg_request_info *request_info)
{
	char *content_length;
	char buffer[10240];
	char* stmt = buffer;
	char *error_exec=NULL;
	char local_hash[MAX_HASHSTR_LEN +1];
	char local_suffix[MAX_SUFFIX_LEN +1];
	char local_description[MAX_DESCRIPTION_LEN +1];
	char local_name[MAX_USER_LEN +1];
	char local_ip[MAX_IP_LEN +1];
	char local_size[MAX_INTSTR_LEN +1];
	int  length, local_size_int;

	printf("Qaullib_WwwFileSchedule\n");

	// extract post variables
	content_length = (char *)mg_get_header(conn, "Content-Length");
	length = atoi(content_length);
	char *post = (char *)malloc(length+length/8+1);
	mg_read(conn, post, length); //read post data

	// get hash
	mg_get_var(post, strlen(post == NULL ? "" : post), "hash", local_hash, MAX_HASHSTR_LEN +1);
	// get suffix
	mg_get_var(post, strlen(post == NULL ? "" : post), "suffix", local_suffix, MAX_SUFFIX_LEN +1);
	// get description
	mg_get_var(post, strlen(post == NULL ? "" : post), "description", local_description, MAX_DESCRIPTION_LEN +1);
	// get size
	mg_get_var(post, strlen(post == NULL ? "" : post), "size", local_size, MAX_INTSTR_LEN +1);
	local_size_int = atoi(local_size);
	// get advertised by
	mg_get_var(post, strlen(post == NULL ? "" : post), "ip", local_ip, MAX_IP_LEN +1);
	mg_get_var(post, strlen(post == NULL ? "" : post), "name", local_name, MAX_USER_LEN +1);

	// write to db
	sprintf(stmt, sql_file_schedule,
		local_hash,
		local_suffix,
		local_description,
		local_size_int,
		local_name,
		local_ip
	);
	if(sqlite3_exec(db, stmt, NULL, NULL, &error_exec) != SQLITE_OK)
	{
		// execution failed
		printf("SQLite error: %s\n",error_exec);
		sqlite3_free(error_exec);
		error_exec=NULL;
	}

	// send answer
	mg_printf(conn, "%s", "HTTP/1.1 200 OK\r\nContent-Type: application/json; charset=utf-8\r\n\r\n");
	mg_printf(conn, "{");
	mg_printf(conn, "\"success\":%i", 1);
	mg_printf(conn, "}");

	// free memory
	free(post);

	// check all scheduled files
	Qaullib_FileCheckScheduled();
}

// ------------------------------------------------------------
static void Qaullib_WwwPubUsers(struct mg_connection *conn, const struct mg_request_info *request_info)
{
	char buf[sizeof(struct qaul_userinfo_msg)];
	struct qaul_userinfo_msg *msg = (struct qaul_userinfo_msg *) buf;

	// pack message
	memcpy(&msg->name, qaul_username, MAX_USER_LEN);
	memcpy(&msg->icon, "\0", 1);

	// send message
	mg_write(conn, buf, (size_t) sizeof(struct qaul_userinfo_msg));
}

// ------------------------------------------------------------
static void Qaullib_WwwPubMsg(struct mg_connection *conn, const struct mg_request_info *request_info)
{
	//int length;
	char buffer[1024];
	char* stmt = buffer;
	char *error_exec = NULL;
	char encoded_msg[3*MAX_MESSAGE_LEN +1];
	char encoded_name[3*MAX_USER_LEN +1];
	char *local_msg;
	char *local_name;
	uint32_t ipv4;
	char ip[MAX_IP_LEN +1];

	// Fetch Message
/*
	char *content_length = (char *)mg_get_header(conn, "Content-Length");
	length = atoi(content_length);
	char *post = (char *)malloc(length+length/8+1);
	mg_read(conn, post, length); //read post data
	// get msg
	mg_get_var(post, strlen(post == NULL ? "" : post), "m", encoded_msg, sizeof(encoded_msg));
	mg_get_var(post, strlen(post == NULL ? "" : post), "n", encoded_name, sizeof(encoded_name));
*/
	// get msg
	get_qsvar(request_info, "m", encoded_msg, sizeof(encoded_msg));
	local_msg = Qaullib_UrlDecode(encoded_msg);
	// get name
	get_qsvar(request_info, "n", encoded_name, sizeof(encoded_name));
	local_name = Qaullib_UrlDecode(encoded_name);

  	// save Message to database
	// todo: ipv6
	sprintf(stmt, sql_msg_set_received,
		2,
		local_name,
		local_msg,
		inet_ntoa(conn->client.rsa.u.sin.sin_addr),
		4,
		0,
		0,
		0,
		0
	);
	//printf("statement: %s\n", stmt);

	if(sqlite3_exec(db, stmt, NULL, NULL, &error_exec) != SQLITE_OK)
	{
		// execution failed
		printf("SQLite error: %s\n",error_exec);
		sqlite3_free(error_exec);
		error_exec=NULL;
	}

	// set new messages flag
	qaul_new_msg = 1;

	// return 200 ok
	mg_printf(conn, "%s", "HTTP/1.1 200 OK\r\nContent-Type: application/json; charset=utf-8\r\n\r\n");
	// json callback
	mg_printf(conn, "%s", "abc({})");

	// check if user is in the db
	// todo: ipv6
	union olsr_ip_addr myip;
	myip.v4 = conn->client.rsa.u.sin.sin_addr;
	Qaullib_UserCheckUser(&myip, local_name);

	// free memory
	free(local_msg);
	free(local_name);
}

// ------------------------------------------------------------
static void Qaullib_WwwPubInfo(struct mg_connection *conn, const struct mg_request_info *request_info)
{
	printf("Qaullib_WwwGetFiles\n");
	mg_printf(conn, "%s", "HTTP/1.1 200 OK\r\nContent-Type: application/json; charset=utf-8\r\n\r\n");
	mg_printf(conn, "abc({");
	mg_printf(conn, "\"name\":\"%s\",",qaul_username);
	Qaullib_WwwJsonFiles(conn, sql_file_get_pub);
	mg_printf(conn, "})");
}

// ------------------------------------------------------------
static void Qaullib_WwwLoading(struct mg_connection *conn, const struct mg_request_info *request_info)
{
	printf("Qaullib_WwwLoading\n");
	mg_printf(conn, "%s", "HTTP/1.1 200 OK\r\nContent-Type: application/json; charset=utf-8\r\n\r\n");
	mg_printf(conn, "{");
	if(qaul_loading_wait == 1)
	{
		// wait
		mg_printf(conn, "\"change\":0");
	}
	else if(Qaullib_ExistsLocale() == 0)
	{
		// show set user name
		mg_printf(conn, "\"change\":1,\"page\":\"#page_config_locale\"");
	}
	else if(Qaullib_ExistsUsername() == 0)
	{
		// show set user name
		mg_printf(conn, "\"change\":1,\"page\":\"#page_config_name\"");
	}
	else if(qaul_configured == 1 && qaul_gui_pagename_set == 1)
	{
		// show configured page name
		mg_printf(conn, "\"change\":1,\"page\":\"#%s\"", qaullib_GuiPageName);
	}
	else if(qaul_configured == 1)
	{
		// configuration finished, show chat
		mg_printf(conn, "\"change\":1,\"page\":\"#page_chat\"");
	}
	// TODO: show error message if an error occurred
	else
	{
		// still loading
		mg_printf(conn, "\"change\":0");
	}
	mg_printf(conn, "}");
}

// ------------------------------------------------------------
static void Qaullib_WwwPubFilechunk(struct mg_connection *conn, const struct mg_request_info *request_info)
{
	char local_hash[MAX_HASHSTR_LEN +1];
	char local_suffix[MAX_SUFFIX_LEN +1];
	char local_chunkpos[MAX_INTSTR_LEN +1];
	char local_file[MAX_PATH_LEN +1];
	int  chunkpos, mychunksize;
	struct qaul_file myfile;
	union qaul_inbuf msgbuf;


	size_t len = 0 ;
    char buffer[BUFSIZ] = { '\0' } ;

	printf("Qaullib_WwwPubFilechunk\n");

	// get hash
	get_qsvar(request_info, "h", local_hash, MAX_HASHSTR_LEN +1);
	// get suffix
	get_qsvar(request_info, "s", local_suffix, MAX_SUFFIX_LEN +1);
	// get chunk starting position
	get_qsvar(request_info, "c", local_chunkpos, MAX_SUFFIX_LEN +1);
	chunkpos = atoi(local_chunkpos);

	// check if file exists
	if(Qaullib_FileAvailable(local_hash, local_suffix, chunkpos, &myfile))
	{
		printf("Qaullib_WwwPubFilechunk size: %i\n", myfile.size);

		// check if file is big enough
		if(myfile.size < chunkpos)
		{
			msgbuf.filechunk.type = htonl(3);
			msgbuf.filechunk.filesize = htonl(myfile.size);
			mg_write(conn, msgbuf.buf, sizeof(struct qaul_filechunk_msg));
			return;
		}

		printf("Qaullib_WwwPubFilechunk 1\n");

		// send file chunk
		Qaullib_FileCreatePath(local_file, local_hash, local_suffix);
		FILE* sendfile = fopen(local_file, "rb") ;
		if(sendfile != NULL)
		{
			printf("Qaullib_WwwPubFilechunk 2\n");

			// send type
			msgbuf.filechunk.type = htonl(1);
			// send file size
			msgbuf.filechunk.filesize = htonl(myfile.size);
			// send chunk size
			if(chunkpos + qaul_chunksize > myfile.size) mychunksize = myfile.size - chunkpos;
			else mychunksize = qaul_chunksize;
			msgbuf.filechunk.chunksize = htonl(mychunksize);

			printf("chunkpos %i, filesize %i, chunksize %i, BUFSIZ %i, iterations %i\n", chunkpos, myfile.size, mychunksize, BUFSIZ, (int) ceil(mychunksize/BUFSIZ));

			// TODO: send chunk hash
			memcpy(msgbuf.filechunk.chunkhash, "01234567890123456789", MAX_HASH_LEN);

			// send buf
			mg_write(conn, msgbuf.buf, sizeof(struct qaul_filechunk_msg));

			// loop through file and send it
			fseek(sendfile, chunkpos, SEEK_SET);
			int i;
	        for(i=0; BUFSIZ*i < mychunksize; i++)
	        {
	        	int mybuf = BUFSIZ;
	        	if(BUFSIZ*(i+1)>mychunksize)
	        	{
	        		mybuf = mychunksize - (BUFSIZ*i);
	        	}
	        	len = fread( buffer, 1, mybuf, sendfile);
	        	mg_write(conn, buffer, len);
	        }
			/*
	        while( (len = fread( buffer, 1, BUFSIZ, sendfile)) > 0 )
	        {
	            mg_write(conn, buffer, len);
	        }
	        */
	        // close file
	        fclose(sendfile);
		}
		else
		{
			printf("Qaullib_WwwPubFilechunk 3\n");

			msgbuf.filechunk.type = htonl(0);
			mg_write(conn, msgbuf.buf, sizeof(struct qaul_filechunk_msg));
		}
	}
	else
	{
		printf("Qaullib_WwwPubFilechunk 4\n");

		// send error
		msgbuf.filechunk.type = htonl(0);
		mg_write(conn, msgbuf.buf, sizeof(struct qaul_filechunk_msg));
	}
	printf("Qaullib_WwwPubFilechunk 5\n");
}


// ------------------------------------------------------------
static void Qaullib_WwwJsonFiles(struct mg_connection *conn, const char* sqlStmt)
{
	sqlite3_stmt *ppStmt;
	//char buffer[10240];
	//char* stmt = buffer;
	char *error_exec=NULL;
	int first;

	//printf("Qaullib_WwwJsonFiles\n");

	// Select rows from database
	if( sqlite3_prepare_v2(db, sqlStmt, -1, &ppStmt, NULL) != SQLITE_OK )
	{
		printf("SQLite error: %s\n",sqlite3_errmsg(db));
	}

	mg_printf(conn, "\"files\":[");
	// For each row returned
	first = 0;
	while (sqlite3_step(ppStmt) == SQLITE_ROW)
	{
	  if(!first) first = 1;
	  else mg_printf(conn, ",");
	  mg_printf(conn, "{");

	  // For each column
	  int jj;
	  for(jj=0; jj < sqlite3_column_count(ppStmt); jj++)
	  {
		    if(strcmp(sqlite3_column_name(ppStmt,jj), "id") == 0)
			{
		    	if(jj>0) mg_printf(conn, ",");
		    	mg_printf(conn, "\"id\":%i", sqlite3_column_int(ppStmt, jj));
			}
			else if(strcmp(sqlite3_column_name(ppStmt,jj), "hash") == 0)
			{
		    	if(jj>0) mg_printf(conn, ",");
				mg_printf(conn, "\"hash\":\"%s\"", sqlite3_column_text(ppStmt, jj));
			}
			else if(strcmp(sqlite3_column_name(ppStmt,jj), "size") == 0)
			{
		    	if(jj>0) mg_printf(conn, ",");
				mg_printf(conn, "\"size\":%i", sqlite3_column_int(ppStmt, jj));
			}
			else if(strcmp(sqlite3_column_name(ppStmt,jj), "suffix") == 0)
			{
		    	if(jj>0) mg_printf(conn, ",");
				mg_printf(conn, "\"suffix\":\"%s\"", sqlite3_column_text(ppStmt, jj));
			}
			else if(strcmp(sqlite3_column_name(ppStmt,jj), "adv_name") == 0)
			{
		    	if(jj>0) mg_printf(conn, ",");
				mg_printf(conn, "\"adv_name\":\"%s\"", sqlite3_column_text(ppStmt, jj));
			}
			else if(strcmp(sqlite3_column_name(ppStmt,jj), "adv_ip") == 0)
			{
		    	if(jj>0) mg_printf(conn, ",");
				mg_printf(conn, "\"adv_ip\":\"%s\"", sqlite3_column_text(ppStmt, jj));
			}
			else if(strcmp(sqlite3_column_name(ppStmt,jj), "description") == 0)
			{
		    	if(jj>0) mg_printf(conn, ",");
				mg_printf(conn, "\"description\":\"%s\"", sqlite3_column_text(ppStmt, jj));
			}
			else if(strcmp(sqlite3_column_name(ppStmt,jj), "created_at") == 0)
			{
		    	if(jj>0) mg_printf(conn, ",");
				mg_printf(conn, "\"time\":\"%s\"", sqlite3_column_text(ppStmt, jj));
			}
			else if(strcmp(sqlite3_column_name(ppStmt,jj), "status") == 0)
			{
		    	if(jj>0) mg_printf(conn, ",");
				mg_printf(conn, "\"status\":%i", sqlite3_column_int(ppStmt, jj));
			}
			else if(strcmp(sqlite3_column_name(ppStmt,jj), "downloaded") == 0)
			{
		    	if(jj>0) mg_printf(conn, ",");
		    	int downloaded = sqlite3_column_int(ppStmt, jj);
				mg_printf(conn, "\"downloaded\":%i", downloaded);
			}
	  }
	  mg_printf(conn, "}");
	}
	mg_printf(conn, "]");
	sqlite3_finalize(ppStmt);
}

// ------------------------------------------------------------
static void Qaullib_WwwExtBinaries(struct mg_connection *conn, const struct mg_request_info *request_info)
{
	printf("Qaullib_WwwGetFiles\n");
	mg_printf(conn, "%s", "HTTP/1.1 200 OK\r\nContent-Type: application/json; charset=utf-8\r\n\r\n");
	mg_printf(conn, "{");
	mg_printf(conn, "\"name\":\"%s\",",qaul_username);
	Qaullib_WwwJsonFiles(conn, sql_file_get_pub_binaries);
	mg_printf(conn, "}");
}


// ------------------------------------------------------------
static void get_qsvar(const struct mg_request_info *request_info, const char *name, char *dst, size_t dst_len)
{
  const char *qs = request_info->query_string;
  mg_get_var(qs, strlen(qs == NULL ? "" : qs), name, dst, dst_len);
}
