/*
 * qaul.net is free software
 * licensed under GPL (version 3)
 */

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE 1
#endif

#include "qaullib_private.h"
#include "polarssl/polarssl/config.h"
#include "polarssl/polarssl/sha1.h"

/**
 * copy a file from its @a origin file path to @a destiny
 *
 * @retval file size in Bytes
 * @retval 0 error
 */
static int Qaullib_FileCopy(const char* origin, const char* destiny);

/**
 * writes the suffix of the @a filename into @a suffix
 *
 * @retval 1 success
 * @retval 0 error
 */
static int Qaullib_FileGetSuffix(char *filename, char *suffix);

/**
 * creates the hash of the file in @a filename and writes it into @a hashstr
 *
 * @retval 1 success
 * @retval 0 error
 */
static int Qaullib_FileCreateHashStr(char *filename, char *hashstr);

/**
 * update file @a status for DB id @a dbid
 * @see qaullib_sql.h
 */
static void Qaullib_FileUpdateStatus(struct qaul_file_LL_item *file_item, int status);

/**
 * update file @a downloaded property for DB id @dbid
 * @see qaullib_sql.h
 */
static void Qaullib_FileUpdateDownloaded(struct qaul_file_LL_item *file_item, int downloaded);

/**
 * update file @a size in Bytes for file with DB id @dbid
 */
static void Qaullib_FileUpdateSize(struct qaul_file_LL_item *file_item, int size);

/**
 * create @a hash of @a filename
 *
 * @retval 1 on success
 * @retval 0 on error
 */
static int Qaullib_HashCreate(char *filename, unsigned char *hash);

// ------------------------------------------------------------
void Qaullib_FileInit(void)
{
	if(QAUL_DEBUG)
		printf("Qaullib_FileInit\n");

	Qaullib_File_LL_Init();

	// initialize the connection array
	int i;
	for(i=0; i<MAX_FILE_CONNECTIONS; i++)
	{
		fileconnections[i].conn.connected = 0;

		// fill in socket defaults
		// FIXME: ipv6
		fileconnections[i].conn.ip.sin_family = AF_INET;
		fileconnections[i].conn.ip.sin_port = htons(WEB_PORT);
	}

	// get the files from DB
	Qaullib_FileDB2LL();
}

// ------------------------------------------------------------
void Qaullib_FilePopulate(void)
{
	char buffer[1024];
	char *stmt = buffer;
	char *error_exec=NULL;
	int status, i;
	char local_destiny[MAX_PATH_LEN +1];

	// loop trough entries
	for(i=0; i<MAX_POPULATE_FILE; i++)
	{
		// check if the file exists
		Qaullib_FileCreatePath(local_destiny, qaul_populate_file[i].hash, qaul_populate_file[i].suffix);
		if(Qaullib_FileExists(local_destiny) == 0) status = QAUL_FILESTATUS_NEW;
		else status = QAUL_FILESTATUS_MYFILE;

		// write entry into DB
		sprintf(stmt,
				sql_file_add,
				qaul_populate_file[i].hash,
				qaul_populate_file[i].suffix,
				qaul_populate_file[i].description,
				qaul_populate_file[i].size,
				status,
				qaul_populate_file[i].type,
				"",
				""
				);
		if(sqlite3_exec(db, stmt, NULL, NULL, &error_exec) != SQLITE_OK)
		{
			printf("SQLite error: %s\n",error_exec);
			sqlite3_free(error_exec);
			error_exec=NULL;
		}
	}
}

// ------------------------------------------------------------
int Qaullib_FileExists(char *path)
{
	int exists = 0;
	FILE *file;

	if ((file = fopen(path, "r")) == NULL) {
		if (errno == ENOENT) {
			printf("File doesn't exist: %s \n", path);
		}
		else {
			// Check for other errors too, like EACCES and EISDIR
			printf("Some other error occured: %s \n", path);
		}
	}
	else {
	  fclose(file);
	  exists = 1;
	}

    return exists;
}

// ------------------------------------------------------------
int Qaullib_FileAdd(struct qaul_file_LL_item *file_item)
{
	if(QAUL_DEBUG)
		printf("Qaullib_FileAdd\n");

	// check if file already exists
	if(!Qaullib_File_LL_HashExists(file_item->hash))
	{
		if(QAUL_DEBUG)
			printf("Hash does not exist: create file\n");

		// add to DB
		Qaullib_FileAdd2DB(file_item);

		// add to LL
		Qaullib_File_LL_Add(file_item);

		return 1;
	}

	if(QAUL_DEBUG)
		printf("Hash already exists: do not create the file\n");

	return 0;
}

// ------------------------------------------------------------
int Qaullib_FileAdd2DB(struct qaul_file_LL_item *file_item)
{
	char buffer[1024];
	char *stmt = buffer;
	char *error_exec=NULL;
	char myip[MAX_IP_LEN +1];

	// create IP str
	if(file_item->adv_validip)
	{
		// todo: ipv6
		if(!inet_pton(AF_INET, myip, &file_item->adv_ip.v4))
		{
			sprintf(myip,"");
		}
	}
	else
		sprintf(myip,"");

	// write into DB
	sprintf(stmt,
			sql_file_add,
			file_item->hashstr,
			file_item->suffix,
			file_item->description,
			file_item->size,
			file_item->status,
			file_item->type,
			file_item->adv_name,
			myip
			);
	if(sqlite3_exec(db, stmt, NULL, NULL, &error_exec) != SQLITE_OK)
	{
		printf("SQLite error: %s\n",error_exec);
		sqlite3_free(error_exec);
		error_exec=NULL;
	}

	return 1;
}

// ------------------------------------------------------------
int Qaullib_FileCopyNew(char *path, char *hashstr, char *suffix)
{
	int size;
	char local_destiny[MAX_PATH_LEN +1];

	if(QAUL_DEBUG)
		printf("Qaullib_FileCopyNew\n");

    // create hash & suffix
	if(!Qaullib_FileCreateHashStr(path, hashstr)) return 0;
    Qaullib_FileGetSuffix(path, suffix);
    // create destination filename
    Qaullib_FileCreatePath(local_destiny, hashstr, suffix);

	// copy file
    size = Qaullib_FileCopy(path, local_destiny);
    if(!size) return 0;

    return size;
}

// ------------------------------------------------------------
void Qaullib_FileCheckScheduled(void)
{
	struct qaul_file_LL_node mynode;
	Qaullib_File_LL_InitNode(&mynode);

	if(QAUL_DEBUG)
		printf("Qaullib_FileCheckScheduled\n");

	// loop through files
	while(Qaullib_File_LL_NextNode(&mynode))
	{
		if(mynode.item->status < QAUL_FILESTATUS_DOWNLOADED && mynode.item->status >= QAUL_FILESTATUS_NEW)
		{
			if(mynode.item->status == QAUL_FILESTATUS_NEW)
			{
				Qaullib_FileSendDiscoveryMsg(mynode.item);
			}
			else if(mynode.item->status == QAUL_FILESTATUS_DISCOVERING)
			{
				// check if timeout expired
				if(mynode.item->discovery_timestamp < time(NULL) -QAUL_FILEDISCOVERY_TIMEOUT)
				{
					if(QAUL_DEBUG)
						printf("file discovery timeout, resend discovery msg \n");

					Qaullib_FileSendDiscoveryMsg(mynode.item);
				}
			}
			else if(mynode.item->status == QAUL_FILESTATUS_DISCOVERED)
			{
				// connect it
				Qaullib_FileConnect(mynode.item);
			}
			else if(mynode.item->status == QAUL_FILESTATUS_DOWNLOADING)
			{
				// todo: check if timeout expired

			}
		}
	}
}

// ------------------------------------------------------------
void Qaullib_FileSendDiscoveryMsg(struct qaul_file_LL_item *file_item)
{
	char buffer[1024];
	int size;
	union olsr_message *m;
	m = (union olsr_message *)buffer;

	if(QAUL_DEBUG)
		printf("send file discovery message for: %s\n", file_item->hashstr);

	// set time stamp change file status
	file_item->discovery_timestamp = time(NULL);
	file_item->status = QAUL_FILESTATUS_DISCOVERING;

	// todo: ipv6
	m->v4.olsr_msgtype = QAUL_FILEDISCOVER_MESSAGE_TYPE;
	memcpy(&m->v4.message.filediscover.hash, &file_item->hash, MAX_HASH_LEN);

	size  = sizeof(struct qaul_filediscover_msg);
	size += sizeof(struct olsrmsg);
	m->v4.olsr_msgsize = htons(size);

	// send package
	Qaullib_IpcSend(m);
}

// ------------------------------------------------------------
void Qaullib_FileStopDownload(struct qaul_file_LL_item *file_item)
{
	int i;

	if(QAUL_DEBUG)
		printf("Qaullib_FileStopDownload\n");

	for(i=0; i<MAX_FILE_CONNECTIONS; i++)
	{
		if(fileconnections[i].conn.connected)
		{
			if(fileconnections[i].fileinfo == file_item)
			{
				// deconnect
				Qaullib_WgetClose(&fileconnections[i].conn);
				if(fileconnections[i].conn.connected)
				{
					printf("Qaullib_FileStopDownload ERROR deconnecting file\n");
					fileconnections[i].conn.connected = 0;
				}
			}
		}
	}
}

// ------------------------------------------------------------
int Qaullib_FileDelete(struct qaul_file_LL_item *file_item)
{
	sqlite3_stmt *ppStmt;
	char buffer[1024];
	char* stmt = buffer;
	char *error_exec=NULL;
	int success = 0;
	char path[MAX_PATH_LEN +1];

	if(QAUL_DEBUG)
		printf("Qaullib_FileDelete\n");

	// FIXME: check if file is scheduled
	// unschedule the file
	Qaullib_FileStopDownload(file_item);

  	// create path
	Qaullib_FileCreatePath(path, file_item->hashstr, file_item->suffix);
	// delete file from HD
	if(QAUL_DEBUG)
		printf("Qaullib_FileDelete delete file: %s\n", path);
	if(remove(path) == -1)
		printf("Qaullib_FileDelete ERROR file couldn't be deleted\n", path);

	// delete from DB
	sprintf(stmt, sql_file_delete_hash, file_item->hashstr);
	if(sqlite3_exec(db, stmt, NULL, NULL, &error_exec) != SQLITE_OK)
	{
		// execution failed
		printf("SQLite error: %s\n",error_exec);
		sqlite3_free(error_exec);
		error_exec=NULL;
	}

	// empty the discovery entries
	Qaullib_Filediscovery_LL_EmptyList(file_item);

	// mark file in LL as deleted
	file_item->status = QAUL_FILESTATUS_DELETED;
	file_item->gui_notify = 1;

	return success;
}

// ------------------------------------------------------------
void Qaullib_FileDB2LL(void)
{
	if(QAUL_DEBUG)
		printf("Qaullib_FileDB2LL\n");

	sqlite3_stmt *ppStmt;
	char *error_exec=NULL;
	struct qaul_file_LL_item myitem;
	char myhashstr[MAX_HASHSTR_LEN +1];

	// Select rows from database
	if( sqlite3_prepare_v2(db, sql_file_get_everything, -1, &ppStmt, NULL) != SQLITE_OK )
	{
		printf("SQLite error: %s\n",sqlite3_errmsg(db));
		return;
	}

	while (sqlite3_step(ppStmt) == SQLITE_ROW)
	{
		myitem.adv_validip = 0;

		// For each column
		int jj;
		for(jj=0; jj < sqlite3_column_count(ppStmt); jj++)
		{
		    if(strcmp(sqlite3_column_name(ppStmt,jj), "id") == 0)
			{
		    	myitem.id = sqlite3_column_int(ppStmt, jj);
			}
		    if(strcmp(sqlite3_column_name(ppStmt,jj), "type") == 0)
			{
		    	myitem.type = sqlite3_column_int(ppStmt, jj);
			}
			else if(strcmp(sqlite3_column_name(ppStmt,jj), "hash") == 0)
			{
				sprintf(myitem.hashstr, "%s", sqlite3_column_text(ppStmt, jj));
				if(!Qaullib_StringToHash(myitem.hashstr, myitem.hash))
					printf("ERROR: Qaullib_StringToHash conversion failed! \n");
			}
			else if(strcmp(sqlite3_column_name(ppStmt,jj), "size") == 0)
			{
		    	myitem.size = sqlite3_column_int(ppStmt, jj);
			}
			else if(strcmp(sqlite3_column_name(ppStmt,jj), "suffix") == 0)
			{
				sprintf(myitem.suffix, "%s", sqlite3_column_text(ppStmt, jj));
			}
			else if(strcmp(sqlite3_column_name(ppStmt,jj), "description") == 0)
			{
				sprintf(myitem.description, "%s", sqlite3_column_text(ppStmt, jj));
			}
			else if(strcmp(sqlite3_column_name(ppStmt,jj), "created_at") == 0)
			{
		    	sprintf(myitem.created_at, "%s", sqlite3_column_text(ppStmt, jj));
			}
			else if(strcmp(sqlite3_column_name(ppStmt,jj), "status") == 0)
			{
		    	myitem.status = sqlite3_column_int(ppStmt, jj);
			}
			else if(strcmp(sqlite3_column_name(ppStmt,jj), "downloaded") == 0)
			{
		    	myitem.downloaded = sqlite3_column_int(ppStmt, jj);
			}

		    // todo: to be removed
			else if(strcmp(sqlite3_column_name(ppStmt,jj), "adv_name") == 0)
			{
				sprintf(myitem.adv_name, "%s", sqlite3_column_text(ppStmt, jj));
			}
			else if(strcmp(sqlite3_column_name(ppStmt,jj), "adv_ip") == 0)
			{
				memset(&myitem.adv_ip, 0, sizeof(myitem.adv_ip));
				// check if ip is set
				if(strlen((char *)sqlite3_column_text(ppStmt, jj)) > 4)
				{
					// TODO: ipv6
					if ( inet_pton(AF_INET, (char *)sqlite3_column_text(ppStmt, jj), &myitem.adv_ip.v4) == 0 )
						printf("inet_pton() ipv4 failed");
					else
						myitem.adv_validip = 1;
				}
			}
		}

		// add it to LL
		Qaullib_File_LL_Add(&myitem);
	}
	sqlite3_finalize(ppStmt);
}


// ------------------------------------------------------------
// helper functions
// ------------------------------------------------------------
static void Qaullib_FileUpdateStatus(struct qaul_file_LL_item *file_item, int status)
{
	char buffer[1024];
	char *stmt = buffer;
	char *error_exec=NULL;

	if(QAUL_DEBUG)
		printf("Qaullib_FileUpdateStatus\n");

	file_item->status = status;
	file_item->gui_notify = 1;

	if(
		status == QAUL_FILESTATUS_DOWNLOADED ||
		status == QAUL_FILESTATUS_ERROR
		)
	{
		sprintf(stmt, sql_file_update_status, status, file_item->hashstr);

		if(sqlite3_exec(db, stmt, NULL, NULL, &error_exec) != SQLITE_OK)
		{
			printf("SQLite error: %s\n",error_exec);
			sqlite3_free(error_exec);
			error_exec=NULL;
		}
	}

	printf("Qaullib_FileUpdateStatus finished\n");
}

// ------------------------------------------------------------
static void Qaullib_FileUpdateDownloaded(struct qaul_file_LL_item *file_item, int downloaded)
{
	char buffer[1024];
	char *stmt = buffer;
	char *error_exec=NULL;

	if(QAUL_DEBUG)
		printf("Qaullib_FileUpdateDownloaded\n");

	file_item->downloaded = downloaded;
	file_item->gui_notify = 1;

	sprintf(stmt, sql_file_update_downloaded, downloaded, file_item->hashstr);
	if(sqlite3_exec(db, stmt, NULL, NULL, &error_exec) != SQLITE_OK)
	{
		printf("SQLite error: %s\n",error_exec);
		sqlite3_free(error_exec);
		error_exec=NULL;
	}
}

// ------------------------------------------------------------
static void Qaullib_FileUpdateSize(struct qaul_file_LL_item *file_item, int size)
{
	char buffer[1024];
	char *stmt = buffer;
	char *error_exec=NULL;

	if(QAUL_DEBUG)
		printf("Qaullib_FileUpdateSize\n");

	file_item->size = size;
	file_item->gui_notify = 1;

	sprintf(stmt, sql_file_update_size, size, file_item->hashstr);
	if(sqlite3_exec(db, stmt, NULL, NULL, &error_exec) != SQLITE_OK)
	{
		printf("SQLite error: %s\n",error_exec);
		sqlite3_free(error_exec);
		error_exec=NULL;
	}
}

// ------------------------------------------------------------
void Qaullib_FileCreatePath(char *filepath, char *hash, char *suffix)
{
	strcpy(filepath, webPath);
    strcat(filepath, PATH_SEPARATOR);
    strcat(filepath, "files");
    strcat(filepath, PATH_SEPARATOR);
    strcat(filepath, hash);
    if(strlen(suffix))
    {
    	strcat(filepath, ".");
    	strcat(filepath, suffix);
    }
}

// ------------------------------------------------------------
int Qaullib_FileAvailable(char *hashstr, char *suffix, struct qaul_file_LL_item **file_item)
{
	unsigned char hash[MAX_HASH_LEN];
	struct qaul_file_LL_item *found_file_item;

	if(QAUL_DEBUG)
		printf("Qaullib_FileAvailable\n");

	// convert hashstr to hash
	if(Qaullib_StringToHash(hashstr, hash))
	{
		// loop through file list
		if(Qaullib_File_LL_HashSearch(hash, &found_file_item))
		{
			printf("Qaullib_FileAvailable 2\n");

			// check if file has finished downloading
			if(
				strncmp(suffix, found_file_item->suffix, sizeof(suffix)) == 0 &&
				found_file_item->status >= QAUL_FILESTATUS_DOWNLOADED
				)
			{
				printf("QFA file found: %s\n", found_file_item->hashstr);
				printf("QFA file suffix: %s\n", found_file_item->suffix);

				*file_item = found_file_item;
				return 1;
			}
		}
	}
	return 0;
}

// ------------------------------------------------------------
void Qaullib_FileConnect(struct qaul_file_LL_item *file_item)
{
	int i, success;
	struct sockaddr_in saddr;
	union olsr_ip_addr *ip;
	char buffer[1024];
	char *header;
	header = buffer;

	if(QAUL_DEBUG)
		printf("Qaullib_FileConnect %s\n", file_item->hashstr);

	// check for file if there is a free connection
	for(i=0; i<MAX_FILE_CONNECTIONS; i++)
	{
		if(fileconnections[i].conn.connected == 0)
		{
			if(QAUL_DEBUG)
				printf("Qaullib_FileConnect connection %i\n", i);

			// get best seeder
			if(Qaullib_Filediscovery_LL_GetBestSeeder(file_item, &ip))
			{
				printf("Qaullib_FileConnect 1\n", i);

				if(QAUL_DEBUG)
				{
					char ipbuf[MAX(INET6_ADDRSTRLEN, INET_ADDRSTRLEN)];
					inet_ntop(AF_INET, &ip->v4, (char *)&ipbuf, MAX(INET6_ADDRSTRLEN, INET_ADDRSTRLEN));
					printf("Qaullib_FileConnect seeder found %s\n", ipbuf);
				}

				// link file
				fileconnections[i].fileinfo = file_item;
				// fill in connection info
				fileconnections[i].conn.bufpos = 0;

				// fill in address
				// todo: ipv6
				saddr.sin_family = AF_INET;
				memcpy(&saddr.sin_addr, &ip->v4, sizeof(ip->v4));
				saddr.sin_port = htons(WEB_PORT);
				memcpy(&fileconnections[i].conn.ip, &saddr, sizeof(struct sockaddr_in));

				// connect
				if(Qaullib_WgetConnect(&fileconnections[i].conn))
				{
					// send http header
					sprintf(
							header,
							"GET /pub_filechunk?h=%s&s=%s&c=%i&e=1 HTTP/1.1\r\n\r\n",
							fileconnections[i].fileinfo->hashstr,
							fileconnections[i].fileinfo->suffix,
							fileconnections[i].fileinfo->downloaded);

					if(Qaullib_WgetSendHeader(&fileconnections[i].conn, header))
					{
						// open file for writing
						char local_filepath[MAX_PATH_LEN +1];
						Qaullib_FileCreatePath(local_filepath, fileconnections[i].fileinfo->hashstr, fileconnections[i].fileinfo->suffix);
						fileconnections[i].file = fopen(local_filepath, "wb");

						if(fileconnections[i].file != NULL)
						{
							fseek(fileconnections[i].file, fileconnections[i].fileinfo->downloaded, SEEK_SET);
							Qaullib_FileUpdateStatus(fileconnections[i].fileinfo, QAUL_FILESTATUS_DOWNLOADING);
							return;
						}
						else
						{
							if(QAUL_DEBUG)
								printf("Qaullib_FileConnect file connection failed\n");

							Qaullib_FileEndFailedConnection(&fileconnections[i]);
						}
					}
					else
					{
						if(QAUL_DEBUG)
							printf("Qaullib_FileConnect sending header failed\n");

						Qaullib_FileEndFailedConnection(&fileconnections[i]);
					}
				}
				else
				{
					printf("Qaullib_FileConnect connection error\n");
					Qaullib_FileEndFailedConnection(&fileconnections[i]);
				}
			}
			else
			{
				Qaullib_FileUpdateStatus(fileconnections[i].fileinfo, QAUL_FILESTATUS_NEW);
			}
			break;
		}
	}
}

// ------------------------------------------------------------
void Qaullib_FileCheckSockets(void)
{
	int i, bytes, type, chunksize, success;

	// check file sockets
	for(i=0; i<MAX_FILE_CONNECTIONS; i++)
	{
		// receive bytes if connected
		if(fileconnections[i].conn.connected)
		{
			success = 1;
			bytes = Qaullib_WgetReceive(&fileconnections[i].conn);

			if(bytes > 0)
			{
				printf("Qaullib_FileCheckSockets %i: bytes %i\n", i, bytes);

				if(fileconnections[i].conn.received == 0 && bytes >= sizeof(struct qaul_filechunk_msg))
				{
					// get file message type
					type = ntohl(fileconnections[i].conn.buf.filechunk.type);
					printf("Qaullib_FileCheckSockets type %i\n", type);

					if(type == 1)
					{
						if(Qaullib_FileCompairFileSize(&fileconnections[i], ntohl(fileconnections[i].conn.buf.filechunk.filesize)))
						{
							// get chunk size
							fileconnections[i].chunksize = ntohl(fileconnections[i].conn.buf.filechunk.chunksize);

							printf("[qaullib] file download: %s, filesize %i, chunksize %i\n", fileconnections[i].fileinfo->hashstr, fileconnections[i].fileinfo->size, fileconnections[i].chunksize);

							// write chunk into file
							fwrite(&fileconnections[i].conn.buf.buf[sizeof(struct qaul_filechunk_msg)], bytes -sizeof(struct qaul_filechunk_msg), 1, fileconnections[i].file);
							fileconnections[i].conn.received += bytes -sizeof(struct qaul_filechunk_msg);
							fileconnections[i].conn.bufpos = 0;
						}
						else
							Qaullib_FileEndFailedConnection(&fileconnections[i]);
					}
					else
					{
						printf("Qaullib_FileCheckSockets file download failed: bytes %i msg-type %i %s\n", bytes, type, fileconnections[i].fileinfo->hashstr);
						// end download
						Qaullib_FileEndFailedConnection(&fileconnections[i]);
					}
				}
				else if(fileconnections[i].conn.received > 0)
				{
					// write chunk into file
		        	fwrite(fileconnections[i].conn.buf.buf, bytes, 1, fileconnections[i].file);
		        	fileconnections[i].conn.bufpos = 0;
		        	fileconnections[i].conn.received += bytes;
				}
				else
				{
					fileconnections[i].conn.bufpos = bytes;
				}

				// check if chunk finished downloading
	        	if(
	        			fileconnections[i].conn.connected &&
	        			fileconnections[i].chunksize > 0 &&
	        			fileconnections[i].chunksize <= fileconnections[i].conn.received
	        			)
	        	{
	        		fclose(fileconnections[i].file);
	        		Qaullib_WgetClose(&fileconnections[i].conn);

	        		// update downloaded
	        		fileconnections[i].fileinfo->downloaded += fileconnections[i].chunksize;
	        		Qaullib_FileUpdateDownloaded(fileconnections[i].fileinfo, fileconnections[i].fileinfo->downloaded);

	        		// mark as successfully downloaded
	        		if(fileconnections[i].fileinfo->downloaded >= fileconnections[i].fileinfo->size)
	        		{
						// todo: check if file hash matches!
	        			printf("[qaullib] download finished! filesize %i, downloaded %i\n", fileconnections[i].fileinfo->size, fileconnections[i].fileinfo->downloaded);
	        			Qaullib_FileUpdateStatus(fileconnections[i].fileinfo, QAUL_FILESTATUS_DOWNLOADED);
	        		}
	        		// todo: otherwise reschedule for next download
	        	}

			}
			else if(bytes == 0)
			{
				printf("Qaullib_FileCheckSockets success == 0\n");
				Qaullib_FileEndFailedConnection(&fileconnections[i]);
			}
		}
	}
}

// ------------------------------------------------------------
int Qaullib_FileCompairFileSize(struct qaul_file_connection *fileconnection, int filesize)
{
	if(QAUL_DEBUG)
		printf("Qaullib_FileCompairFileSize\n");

	if(fileconnection->fileinfo->size > 0 && fileconnection->fileinfo->downloaded > 0)
	{
		// check if file size matches
		if(fileconnection->fileinfo->size != filesize)
		{
			if(QAUL_DEBUG)
				printf("Qaullib_FileCheckSockets file sizes didn't mach: %i != %i\n", fileconnection->fileinfo->size, filesize);

			return 0;
		}
		else
			return 1;
	}
	else
	{
		Qaullib_FileUpdateSize(fileconnection->fileinfo, filesize);
	}
	return 1;
}

// ------------------------------------------------------------
void Qaullib_FileEndFailedConnection(struct qaul_file_connection *fileconnection)
{
	union olsr_ip_addr ip;

	if(QAUL_DEBUG)
    	printf("Qaullib_FileEndFailedConnection\n");

	// close file
	if(fileconnection->file != NULL)
		fclose(fileconnection->file);
	// close connection
	Qaullib_WgetClose(&fileconnection->conn);
	// remove this seeder from list
	memcpy(&ip.v4, &fileconnection->conn.ip, sizeof(ip.v4));
	Qaullib_Filediscovery_LL_DeleteSeederIp(fileconnection->fileinfo, &ip);
	// reschedule file
	Qaullib_FileUpdateStatus(fileconnection->fileinfo, QAUL_FILESTATUS_DISCOVERED);
}

// ------------------------------------------------------------
// file manipulation
// ------------------------------------------------------------
static int Qaullib_FileCopy(const char* origin, const char* destiny)
{
	if(QAUL_DEBUG)
		printf("Qaullib_FileCopy %s -> %s\n", origin, destiny);

	size_t filesize = 0;
	size_t len = 0 ;
    char buffer[BUFSIZ] = { '\0' } ;

    FILE* in = fopen( origin, "rb" ) ;
    FILE* out = fopen( destiny, "wb" ) ;

    if( in == NULL || out == NULL )
    {
        perror( "An error occured while opening files!!!" ) ;
        in = out = 0 ;
        return 0;
    }
    else    // add this else clause
    {
        while( (len = fread( buffer, 1, BUFSIZ, in)) > 0 )
        {
        	filesize += len;
        	fwrite( buffer, len, 1, out ) ;
        }

        fclose(in) ;
        fclose(out) ;
    }
    return (int) filesize;
}

// ------------------------------------------------------------
static int Qaullib_FileGetSuffix(char *filename, char *suffix)
{
	if(QAUL_DEBUG)
		printf("Qaullib_FileGetSuffix\n");

	// is there a dot?
	char *local_suffix = strrchr(filename, '.');
	if(!local_suffix) return 0;

	// check suffix size
	if(strlen(local_suffix+1) > 5 || strlen(local_suffix+1) < 1) return 0;

	// TODO: check if it only contains allowed ASCII characters

	// copy into suffix
	strcpy(suffix, local_suffix+1);

	return 1;
}

// ------------------------------------------------------------
static int Qaullib_FileCreateHashStr(char *filename, char *hashstr)
{
	if(QAUL_DEBUG)
		printf("Qaullib_FileCreateHashStr\n");

	unsigned char local_hash[MAX_HASH_LEN];
    // create hash
    if(!Qaullib_HashCreate(filename, local_hash)) return 0;
    // convert binary to hex encoding
    if(!Qaullib_HashToString(local_hash, hashstr)) return 0;
    return 1;
}

// ------------------------------------------------------------
// Hash functions
// ------------------------------------------------------------
static int Qaullib_HashCreate(char *filename, unsigned char *hash)
{
	if(QAUL_DEBUG)
		printf("Qaullib_HashCreate\n");

	int ret = polarssl_sha1_file( filename, hash );
	if(ret == 1) fprintf( stderr, "[qaullib] failed to open: %s\n", filename );;
	if(ret == 2) fprintf( stderr, "[qaullib] failed to open: %s\n", filename );;
	if(ret != 0) return 0;
	return 1;
}

// ------------------------------------------------------------
int Qaullib_HashToString(unsigned char *hash, char *string)
{
	int i;

	if(QAUL_DEBUG)
		printf("Qaullib_HashToString\n");

	// FIXME: big-endian / little-endian
	for(i=0;i<MAX_HASH_LEN;i++)
	{
		sprintf(string+(i*2),"%02x",hash[i]);
	}
	return 1;
}

// ------------------------------------------------------------
int Qaullib_StringToHash(char *string, unsigned char *hash)
{
	int i, j;
	uint8_t mybyte;

	// fill hash with zeros
	memset(hash, 0, MAX_HASH_LEN);

	for(i=0;i<MAX_HASH_LEN;i++)
	{
		mybyte = 0;

		for(j=0;j<2;j++)
		{
			if(strncmp(string+i*2+j,"0",1)==0)
				mybyte |= 0;
			else if(strncmp(string+i*2+j,"1",1)==0)
				mybyte |= 1;
			else if(strncmp(string+i*2+j,"2",1)==0)
				mybyte |= 2;
			else if(strncmp(string+i*2+j,"3",1)==0)
				mybyte |= 3;
			else if(strncmp(string+i*2+j,"4",1)==0)
				mybyte |= 4;
			else if(strncmp(string+i*2+j,"5",1)==0)
				mybyte |= 5;
			else if(strncmp(string+i*2+j,"6",1)==0)
				mybyte |= 6;
			else if(strncmp(string+i*2+j,"7",1)==0)
				mybyte |= 7;
			else if(strncmp(string+i*2+j,"8",1)==0)
				mybyte |= 8;
			else if(strncmp(string+i*2+j,"9",1)==0)
				mybyte |= 9;
			else if(strncmp(string+i*2+j,"a",1)==0 || strncmp(string+i*2+j,"A",1)==0)
				mybyte |= 10;
			else if(strncmp(string+i*2+j,"b",1)==0 || strncmp(string+i*2+j,"B",1)==0)
				mybyte |= 11;
			else if(strncmp(string+i*2+j,"c",1)==0 || strncmp(string+i*2+j,"C",1)==0)
				mybyte |= 12;
			else if(strncmp(string+i*2+j,"d",1)==0 || strncmp(string+i*2+j,"D",1)==0)
				mybyte |= 13;
			else if(strncmp(string+i*2+j,"e",1)==0 || strncmp(string+i*2+j,"E",1)==0)
				mybyte |= 14;
			else if(strncmp(string+i*2+j,"f",1)==0 || strncmp(string+i*2+j,"F",1)==0)
				mybyte |= 15;
			else
				return 0;

			if(j==0)
				mybyte = mybyte << 4;
			else
				memcpy(hash+i, &mybyte, 1);
		}
	}
	return 1;
}
