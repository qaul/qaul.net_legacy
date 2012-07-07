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
static void Qaullib_FileUpdateStatus(int dbid, int status);

/**
 * update file @a downloaded property for DB id @dbid
 * @see qaullib_sql.h
 */
static void Qaullib_FileUpdateDownloaded(int dbid, int downloaded);

/**
 * update file @a size in Bytes for file with DB id @dbid
 */
static void Qaullib_FileUpdateSize(int dbid, int size);

/**
 * create @a hash of @a filename
 *
 * @retval 1 on success
 * @retval 0 on error
 */
static int Qaullib_HashCreate(char *filename, unsigned char *hash);

/**
 * create @a string from @a hash
 *
 * @retval 1 on success
 * @retval 0 on error
 */
static int Qaullib_HashToString(unsigned char *hash, char *string);


// ------------------------------------------------------------
void Qaullib_FilePopulate(void)
{
	char buffer[10240];
	char *stmt = buffer;
	char *error_exec=NULL;
	int status, i;
	char local_destiny[MAX_PATH_LEN +1];

	// loop trough entries
	for(i=0; i<MAX_POPULATE_FILE; i++)
	{
		// check if the file exists
		Qaullib_FileCreatePath(local_destiny, qaul_populate_file[i].hash, qaul_populate_file[i].suffix);
		if(Qaullib_FileExists(local_destiny) == 0) status = 0;
		else status = 2;

		// write entry into DB
		sprintf(stmt,
				sql_file_set_all,
				qaul_populate_file[i].hash,
				qaul_populate_file[i].suffix,
				qaul_populate_file[i].description,
				qaul_populate_file[i].size,
				status,
				qaul_populate_file[i].type
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
int Qaullib_FileAdd(char *path, char *hashstr, char *suffix)
{
	int size;
	char local_destiny[MAX_PATH_LEN +1];

	printf("Qaullib_FileAdd\n");

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
	sqlite3_stmt *ppStmt;
	int freesocket;
	char *error_exec=NULL;

	printf("Qaullib_FileCheckScheduled ...\n");

	// get scheduled files from database
	if( sqlite3_prepare_v2(db, sql_file_get_scheduled, -1, &ppStmt, NULL) != SQLITE_OK )
	{
		printf("SQLite error [file scheduled]: %s\n",sqlite3_errmsg(db));
	}
	else
	{
		printf("loop through results\n");

		while (sqlite3_step(ppStmt) == SQLITE_ROW)
		{
			int jj, local_id, local_size, local_downloaded, ipv, validaddr;
			char local_hash[MAX_HASHSTR_LEN +1];
			char local_suffix[MAX_SUFFIX_LEN +1];
			char local_ip[MAX_IP_LEN +1];
			struct sockaddr_in saddr;
			validaddr = 0;

			printf("something found\n");

			for(jj=0; jj < sqlite3_column_count(ppStmt); jj++)
			{
				if(strcmp(sqlite3_column_name(ppStmt, jj),"id") == 0)
				{
					local_id = sqlite3_column_int(ppStmt, jj);
				}
				else if(strcmp(sqlite3_column_name(ppStmt, jj),"adv_ip") == 0)
				{
					// check if ip is set
					if(strlen(sqlite3_column_text(ppStmt, jj)) > 4)
					{
						printf("adv_ip: %s \n", sqlite3_column_text(ppStmt, jj));

						// TODO: ipv6
						if(ipv == 6)
						{
							if ( inet_pton(AF_INET6, sqlite3_column_text(ppStmt, jj), &saddr.sin_addr) == 0 )
								printf("inet_pton() ipv6 failed");
							else validaddr = 1;
							saddr.sin_family = AF_INET6;
						}
						else
						{
							if ( inet_pton(AF_INET, sqlite3_column_text(ppStmt, jj), &saddr.sin_addr) == 0 )
								printf("inet_pton() ipv4 failed");
							else validaddr = 1;
							saddr.sin_family = AF_INET;
						}
					}
				}
				else if(strcmp(sqlite3_column_name(ppStmt, jj),"hash") == 0)
				{
					sprintf(local_hash, "%s", sqlite3_column_text(ppStmt, jj));
				}
				else if(strcmp(sqlite3_column_name(ppStmt, jj),"suffix") == 0)
				{
					sprintf(local_suffix, "%s", sqlite3_column_text(ppStmt, jj));
				}
				else if(strcmp(sqlite3_column_name(ppStmt, jj),"size") == 0)
				{
					local_size = sqlite3_column_int(ppStmt, jj);
				}
				else if(strcmp(sqlite3_column_name(ppStmt, jj),"downloaded") == 0)
				{
					local_downloaded = sqlite3_column_int(ppStmt, jj);
				}
			}

			if(validaddr > 0)
			{
				printf("valid addr\n");
				Qaullib_FileConnect(local_id, local_hash, local_suffix, local_size, local_downloaded, &saddr);
			}
			else printf("no valid addr\n");
		}
		sqlite3_finalize(ppStmt);
	}
}

// ------------------------------------------------------------
int Qaullib_FileDeleteById(int id)
{
	sqlite3_stmt *ppStmt;
	char buffer[1024];
	char* stmt = buffer;
	char *error_exec=NULL;
	int success = 0;
	char path[MAX_PATH_LEN +1];
	char myhash[MAX_HASHSTR_LEN +1];
	char mysuffix[MAX_SUFFIX_LEN +1];

	printf("Qaullib_FileDeleteById %i\n", id);

	// get file from DB
	sprintf(stmt, sql_file_get_id, id);
	if( sqlite3_prepare_v2(db, stmt, -1, &ppStmt, NULL) != SQLITE_OK )
	{
		printf("SQLite error: %s\n",sqlite3_errmsg(db));
		return 0;
	}
	while (sqlite3_step(ppStmt) == SQLITE_ROW)
	{
		// For each collumn
		int jj;
		for(jj=0; jj < sqlite3_column_count(ppStmt); jj++)
		{
			if(strcmp(sqlite3_column_name(ppStmt,jj), "hash") == 0)
			{
				sprintf(myhash,"%s",sqlite3_column_text(ppStmt, jj));
			}
			else if(strcmp(sqlite3_column_name(ppStmt,jj), "suffix") == 0)
			{
				sprintf(mysuffix,"%s",sqlite3_column_text(ppStmt, jj));
			}
		}
	  	// create path
		strcpy(path, webPath);
		strcat(path, PATH_SEPARATOR);
		strcat(path, "files");
		strcat(path, PATH_SEPARATOR);
		strcat(path, myhash);
		if(strlen(mysuffix) > 0)
		{
			strcat(path, ".");
			strcat(path, mysuffix);
		}
	}
	sqlite3_finalize(ppStmt);

	// mark file in DB as deleted
	sprintf(stmt, sql_file_update_status, -2, id);
	if(sqlite3_exec(db, stmt, NULL, NULL, &error_exec) != SQLITE_OK)
	{
		// execution failed
		printf("SQLite error: %s\n",error_exec);
		sqlite3_free(error_exec);
		error_exec=NULL;
		return 0;
	}

	// delete file
	printf("[qaullib] delete file: %s\n", path);
	if(remove(path) == -1) success = 0;
	else success = 1;

	return success;
}


// ------------------------------------------------------------
// helper functions
// ------------------------------------------------------------
static void Qaullib_FileUpdateStatus(int dbid, int status)
{
	char buffer[10240];
	char *stmt = buffer;
	char *error_exec=NULL;

	// set username
	sprintf(stmt, sql_file_update_status, status, dbid);
	if(sqlite3_exec(db, stmt, NULL, NULL, &error_exec) != SQLITE_OK)
	{
		printf("SQLite error: %s\n",error_exec);
		sqlite3_free(error_exec);
		error_exec=NULL;
	}
}

// ------------------------------------------------------------
static void Qaullib_FileUpdateDownloaded(int dbid, int downloaded)
{
	char buffer[10240];
	char *stmt = buffer;
	char *error_exec=NULL;

	// set username
	sprintf(stmt, sql_file_update_downloaded, downloaded, dbid);
	if(sqlite3_exec(db, stmt, NULL, NULL, &error_exec) != SQLITE_OK)
	{
		printf("SQLite error: %s\n",error_exec);
		sqlite3_free(error_exec);
		error_exec=NULL;
	}
}

// ------------------------------------------------------------
static void Qaullib_FileUpdateSize(int dbid, int size)
{
	char buffer[10240];
	char *stmt = buffer;
	char *error_exec=NULL;

	// set username
	sprintf(stmt, sql_file_update_size, size, dbid);
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
int Qaullib_FileAvailable(char *hash, char *suffix, int startbyte, struct qaul_file *file)
{
	// TODO: loop through file list
	// for now get everything from DB
	sqlite3_stmt *ppStmt;
	char buffer[1024];
	char* stmt = buffer;
	char *error_exec=NULL;
	int success = 0;

	printf("Qaullib_FileAvailable? hash %s\n", hash);

	// Select row from database
	sprintf(stmt, sql_file_get_hash, hash);
	if( sqlite3_prepare_v2(db, stmt, -1, &ppStmt, NULL) != SQLITE_OK )
	{
		printf("SQLite error: %s\n",sqlite3_errmsg(db));
	}

	// there should only be one row
	while (sqlite3_step(ppStmt) == SQLITE_ROW)
	{
	  // For each column
	  int jj;
	  for(jj=0; jj < sqlite3_column_count(ppStmt); jj++)
	  {
		    if(strcmp(sqlite3_column_name(ppStmt,jj), "id") == 0)
			{
		    	file->id = sqlite3_column_int(ppStmt, jj);
			}
			else if(strcmp(sqlite3_column_name(ppStmt,jj), "hash") == 0)
			{
		    	strcpy(file->hash, sqlite3_column_text(ppStmt, jj));
			}
			else if(strcmp(sqlite3_column_name(ppStmt,jj), "suffix") == 0)
			{
				strcpy(file->suffix, sqlite3_column_text(ppStmt, jj));
			}
			else if(strcmp(sqlite3_column_name(ppStmt,jj), "description") == 0)
			{
				strcpy(file->description, sqlite3_column_text(ppStmt, jj));
			}
			else if(strcmp(sqlite3_column_name(ppStmt,jj), "size") == 0)
			{
		    	file->size = sqlite3_column_int(ppStmt, jj);
			}
			else if(strcmp(sqlite3_column_name(ppStmt,jj), "status") == 0)
			{
				file->status = sqlite3_column_int(ppStmt, jj);
			}
			else if(strcmp(sqlite3_column_name(ppStmt,jj), "created_at") == 0)
			{
		    	file->created_at = sqlite3_column_int(ppStmt, jj);
			}
			else if(strcmp(sqlite3_column_name(ppStmt,jj), "adv_ip") == 0)
			{
		    	// convert to ip
				// todo: make ipv6
				file->adv_ip.sin_family = AF_INET;
				file->adv_ip.sin_port = htons(WEB_PORT);
				if(file->status == 4 || file->status < 0)
					inet_pton(AF_INET, "127.0.0.1", &file->adv_ip.sin_addr); // numeric IP only
				else
					inet_pton(AF_INET, sqlite3_column_text(ppStmt, jj), &file->adv_ip.sin_addr); // numeric IP only
			}
			else if(strcmp(sqlite3_column_name(ppStmt,jj), "downloaded") == 0)
			{
		    	file->downloaded = sqlite3_column_int(ppStmt, jj);
			}
	  }
	  success = 1;
	  printf("[qaullib] file found in db\n");
	}
	sqlite3_finalize(ppStmt);
	return success;
}

// ------------------------------------------------------------
void Qaullib_FileConnect(int dbid, char *hash, char *suffix, int filesize, int downloaded, struct sockaddr_in *saddr)
{
	int i, success;
	printf("[qaullib] Qaullib_FileConnect %s\n", hash);

	// check for file if there is a free connection
	for(i=0; i<MAX_FILE_CONNECTIONS; i++)
	{
		if(fileconnections[i].conn.connected == 0)
		{
			printf("[qaullib] i %i to connect\n", i);

			// fill in connection info
			fileconnections[i].conn.bufpos = 0;
			fileconnections[i].id = dbid;
			fileconnections[i].filesize = filesize;
			fileconnections[i].downloaded = downloaded;

			// fill in address
			saddr->sin_port = htons(WEB_PORT);
			memcpy(&fileconnections[i].conn.ip, saddr, sizeof(struct sockaddr_in));

			// fill in additional info
			memcpy(&fileconnections[i].hash, hash, MAX_HASHSTR_LEN);
			memcpy(&fileconnections[i].hash[MAX_HASHSTR_LEN], "\0", 1);
			memcpy(&fileconnections[i].suffix, suffix, MAX_SUFFIX_LEN);
			memcpy(&fileconnections[i].suffix[MAX_SUFFIX_LEN], "\0", 1);

			// TODO: search better connection
			// connect
			success = Qaullib_WgetConnect(&fileconnections[i].conn);

			// send http header
			if(success == 1)
			{
				char buffer[1024];
				char *header = buffer;
				sprintf(header, "GET /pub_filechunk?h=%s&s=%s&c=%i&e=1 HTTP/1.1\r\n\r\n", hash, suffix, downloaded);
				success = Qaullib_WgetSendHeader(&fileconnections[i].conn, header);
				if(success)
				{
					Qaullib_FileUpdateStatus(dbid, 1);

					// open file for writing
					char local_filepath[MAX_PATH_LEN +1];
					Qaullib_FileCreatePath(local_filepath, hash, suffix);
					fileconnections[i].file = fopen(local_filepath, "wb");
					if(fileconnections[i].file == NULL)
						success = 0;
					else
						fseek(fileconnections[i].file, downloaded, SEEK_SET);
				}
			}

			if(success == 0)
			{
				printf("[qaullib] connection error %i\n", success);
				Qaullib_WgetClose(&fileconnections[i].conn);
				Qaullib_FileUpdateStatus(dbid, -1);
			}
			else
			{
				printf("successfully connected to download %s\n", hash);
			}

			break;
		}
	}
}

// ------------------------------------------------------------
void Qaullib_FileCheckSockets(void)
{
	int i, bytes, type, filesize, chunksize, success;
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
				printf("[qaullib] check sockets: bytes %i\n", bytes);

				if(fileconnections[i].conn.received == 0 && bytes >= sizeof(struct qaul_filechunk_msg))
				{
					// get file message type
					type = ntohl(fileconnections[i].conn.buf.filechunk.type);
					printf("[qaullib] type %i\n", type);

					if(type == 1)
					{
						// get file size
						filesize = ntohl(fileconnections[i].conn.buf.filechunk.filesize);
						if(fileconnections[i].filesize > 0 && fileconnections[i].downloaded > 0)
						{
							// check if file size matches
							if(fileconnections[i].filesize != filesize) success = 0;
						}
						else
						{
							// write file size into db
							fileconnections[i].filesize = filesize;
						}

						// get chunk size
						fileconnections[i].chunksize = ntohl(fileconnections[i].conn.buf.filechunk.chunksize);

						printf("[qaullib] file download: id %i, filesize %i, chunksize %i\n", fileconnections[i].id, fileconnections[i].filesize, fileconnections[i].chunksize);

						// write chunk into file
			        	fwrite(&fileconnections[i].conn.buf.buf[sizeof(struct qaul_filechunk_msg)], bytes -sizeof(struct qaul_filechunk_msg), 1, fileconnections[i].file);
			        	fileconnections[i].conn.received += bytes -sizeof(struct qaul_filechunk_msg);
			        	fileconnections[i].conn.bufpos = 0;
					}
					else
					{
						// was unsuccessful
						// todo: reschedule file
						printf("[qaullib] file download failed: bytes %i msg-type %i id %i\n", bytes, type, fileconnections[i].id);
						success = 0;
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
	        	if(fileconnections[i].chunksize > 0 && fileconnections[i].chunksize <= fileconnections[i].conn.received)
	        	{
	        		fclose(fileconnections[i].file);
	        		Qaullib_WgetClose(&fileconnections[i].conn);

	        		// update downloaded
	        		fileconnections[i].downloaded += fileconnections[i].chunksize;
	        		Qaullib_FileUpdateDownloaded(fileconnections[i].id, fileconnections[i].downloaded);


	        		// mark as successfully downloaded
	        		if(fileconnections[i].downloaded >= fileconnections[i].filesize)
	        		{
						printf("[qaullib] download finished! filesize %i, downloaded %i\n", fileconnections[i].filesize, fileconnections[i].downloaded);
	        			Qaullib_FileUpdateStatus(fileconnections[i].id, 2);
	        		}
	        		// todo: otherwise reschedule for next download
	        	}

			}
			else if(bytes == 0) success = 0;

			if(success == 0)
			{
				printf("Qaullib_FileCheckSockets success == 0\n");

				// an error occurred, take measures
				// todo: reschedule
				// search for hosts
				// connect to host

				// mark file as download failure
				Qaullib_FileUpdateStatus(fileconnections[i].id, -1);
		        // close file
		        fclose(fileconnections[i].file);
			}
		}
	}
}

// ------------------------------------------------------------
// file manipulation
// ------------------------------------------------------------
static int Qaullib_FileCopy(const char* origin, const char* destiny)
{
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
	printf("Qaullib_FileCreateHashStr\n");

	unsigned char local_hash[MAX_HASH_LEN];
    // create hash
    if(!Qaullib_HashCreate(filename, local_hash)) return 0;
    printf("hashcreation survived\n");
    // convert binary to hex encoding
    if(!Qaullib_HashToString(local_hash, hashstr)) return 0;
    printf("hashtostring survived\n");
    return 1;
}

// ------------------------------------------------------------
// Hash functions
// ------------------------------------------------------------
static int Qaullib_HashCreate(char *filename, unsigned char *hash)
{
	printf("Qaullib_HashCreate\n");
	int ret = sha1_file( filename, hash );
	if(ret == 1) fprintf( stderr, "[qaullib] failed to open: %s\n", filename );;
	if(ret == 2) fprintf( stderr, "[qaullib] failed to open: %s\n", filename );;
	if(ret != 0) return 0;
	return 1;
}

// ------------------------------------------------------------
static int Qaullib_HashToString(unsigned char *hash, char *string)
{
	printf("Qaullib_HashToString\n");
	int i;
	for(i=0;i<20;i++)
	{
		sprintf(string+(i*2),"%02x",hash[i]);
	}
	return 1;
}
