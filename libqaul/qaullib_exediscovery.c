/*
 * qaul.net is free software
 * licensed under GPL (version 3)
 */

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE 1
#endif

#include "qaullib_private.h"


// ------------------------------------------------------------
void Qaullib_ExeInit(void)
{
	int i;
	char buffer[1024];
	char *key;

	key = buffer;
	qaul_exe_available = 1;

	if(QAUL_DEBUG)
		printf("Qaullib_ExeInit\n");

	// loop through files and check if they are already discovered
	for(i=0; i<MAX_POPULATE_FILE; i++)
	{
		// fill in the common values
		qaul_exe_array[i].OS_flag = qaul_populate_file[i].OS_flag;
		qaul_exe_array[i].max_size = qaul_populate_file[i].max_size;
		strncpy(qaul_exe_array[i].description, qaul_populate_file[i].description, MAX_DESCRIPTION_LEN);
		memcpy(&qaul_exe_array[i].description[MAX_DESCRIPTION_LEN], "\0", 1);
		strncpy(qaul_exe_array[i].suffix, qaul_populate_file[i].suffix, MAX_SUFFIX_LEN);
		memcpy(&qaul_exe_array[i].suffix[MAX_SUFFIX_LEN], "\0", 1);

		// create key
		sprintf(key, "exe.%i", qaul_populate_file[i].OS_flag);

		// check if this file has been discovered
		if(Qaullib_DbGetConfigValue(key, qaul_exe_array[i].hashstr))
		{
			// fill in specific values
			strncpy(qaul_exe_array[i].hashstr, qaul_populate_file[i].hashstr, MAX_HASHSTR_LEN);
			memcpy(&qaul_exe_array[i].hashstr[MAX_HASHSTR_LEN], "\0", 1);

			// create hash from hash string
			Qaullib_StringToHash(qaul_exe_array[i].hashstr, qaul_exe_array[i].hash);

			// mark this file as discovered
		    qaul_exe_array[i].discovered = 1;
		    qaul_exe_array[i].discovery_timestamp = time(NULL);
		}
		else
		{
			qaul_exe_available = 0;
			qaul_exe_array[i].discovered = 0;
			memset(&qaul_exe_array[i].discovery_timestamp, 0, sizeof(time_t));
		}
	}
}

// ------------------------------------------------------------
void Qaullib_ExeScheduleDiscovery(void)
{
	char buffer[1024];
	int size, i, OS_flag;
	union olsr_message *m;

	m = (union olsr_message *)buffer;
	OS_flag = 0;

	if(QAUL_DEBUG)
		printf("Qaullib_ExeScheduleDiscovery\n");

	for(i=0; i<MAX_POPULATE_FILE; i++)
	{
		if(
			qaul_exe_array[i].discovered == 0 &&
			qaul_exe_array[i].discovery_timestamp < time(NULL) -QAUL_FILEDISCOVERY_TIMEOUT
			)
		{
			if(QAUL_DEBUG)
				printf("Qaullib_ExeScheduleDiscovery send exe discovery message\n");

			// update timestamp
			qaul_exe_array[i].discovery_timestamp = time(NULL);

			// set all needed OS_flags
			OS_flag = qaul_exe_array[i].OS_flag;
			i++;
			for(; i<MAX_POPULATE_FILE; i++)
			{
				if(qaul_exe_array[i].discovered == 0)
				{
					OS_flag += qaul_exe_array[i].OS_flag;
					qaul_exe_array[i].discovery_timestamp = time(NULL);
				}
			}

			// fill in exe discovery message
			// todo: ipv6
			size  = sizeof(struct qaul_exediscover_msg);
			size += sizeof(struct olsrmsg);
			m->v4.olsr_msgsize = htons(size);
			m->v4.olsr_msgtype = QAUL_EXEDISCOVER_MESSAGE_TYPE;
			m->v4.message.exediscover.OS_flag = htonl(OS_flag);

			// send message
			Qaullib_IpcSend(m);
		}
	}
}

// ------------------------------------------------------------
void Qaullib_ExeProcessAvailableMsg(struct qaul_exeavailable_msg *msg)
{
	char buffer[1024];
	char *key  = buffer;
	int i, OS_flag, size;
	struct qaul_file_LL_item file_item;

	if(QAUL_DEBUG)
		printf("Qaullib_ExeProcessAvailableMsg\n");

	OS_flag = ntohl(msg->OS_flag);
	size = ntohl(msg->filesize);

	// check if we still need it
	for(i=0; i<MAX_POPULATE_FILE; i++)
	{
		if(qaul_exe_array[i].OS_flag & OS_flag)
		{
			if(
				qaul_exe_array[i].discovered == 0 &&
				qaul_exe_array[i].max_size > size &&
				strncmp(qaul_exe_array[i].suffix, msg->suffix, MAX_SUFFIX_LEN) == 0
				)
			{
				// create file entry
				file_item.type = QAUL_FILETYPE_EXECUTABLE;
				file_item.status = QAUL_FILESTATUS_NEW;
				file_item.size = size;
				file_item.downloaded = 0;
				file_item.downloaded_chunk = 0;

				strncpy(file_item.description, qaul_exe_array[i].description, MAX_DESCRIPTION_LEN);
				memcpy(&file_item.description[MAX_DESCRIPTION_LEN], "\0", 1);

				strncpy(file_item.suffix, msg->suffix, MAX_SUFFIX_LEN);
				memcpy(&file_item.suffix[MAX_SUFFIX_LEN], "\0", 1);

				memcpy(file_item.hash, msg->hash, MAX_HASH_LEN);
				Qaullib_HashToString(msg->hash, file_item.hashstr);

				// add exe to DB & LL
				if(Qaullib_FileAdd(&file_item))
				{
					// fill exe discover struct
					qaul_exe_array[i].size = size;
					memcpy(qaul_exe_array[i].hash, msg->hash, MAX_HASH_LEN);
					strncpy(qaul_exe_array[i].hashstr, file_item.hashstr, MAX_HASHSTR_LEN);
					memcpy(&file_item.hashstr[MAX_HASHSTR_LEN], "\0", 1);

					// write into config table
					sprintf(key, "exe.%i", qaul_exe_array[i].OS_flag);
					Qaullib_DbSetConfigValue(key, qaul_exe_array[i].hashstr);

					qaul_exe_array[i].discovered = 1;
				}
			}
			return;
		}
	}
}
