/*
 * qaul.net is free software
 * licensed under GPL (version 3)
 */

#ifndef _QAULLIB_FILESHARING
#define _QAULLIB_FILESHARING

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * defines chunk size of file sharing
 */
int qaul_chunksize;

/**
 * structure of available connections for downloading files
 */
struct qaul_file_connection
{
	struct qaul_wget_connection conn;
	int id; // sqlite id
	char hash[MAX_HASHSTR_LEN +1];
	char suffix[MAX_SUFFIX_LEN +1];
	unsigned int filesize;
	unsigned int chunksize;
	unsigned int downloaded;
	FILE *file;
};

/**
 * array of TCP connections for file downloading
 */
struct qaul_file_connection fileconnections[MAX_FILE_CONNECTIONS];

/**
 * file table
 */
struct qaul_file
{
    int id;
    int type;
    char hash[MAX_HASH_LEN +1];
    char suffix[MAX_SUFFIX_LEN +1];
    char description[MAX_DESCRIPTION_LEN +1];
    int created_at;
    int status;
    int size;
    int downloaded;
	struct sockaddr_in adv_ip;

    int event_nr;

    // connection status
};

/**
 * add existing files to DB
 * called once after installation
 *
 * fat clients contain all executables for all OSs
 * slim clients only contain information about the other versions
 * and need to download the executables from other users.
 */
void Qaullib_FilePopulate(void);

/**
 * add file from @a path to filesharing and analyzes the file.
 * It creates the @a hashstr and fills in the @a suffix
 *
 * @retval 0 on error
 * @retval filesize in Bytes on success
 */
int Qaullib_FileAdd(char *path, char *hashstr, char *suffix);

/**
 * check if any file needs to be downloaded
 */
void Qaullib_FileCheckScheduled(void);

/**
 * create the @a filepath out of the @a hash string and the @a suffix
 */
void Qaullib_FileCreatePath(char *filepath, char *hash, char *suffix);

/**
 * checks if file with @a hash is available from the bytes position @a startbyte
 *
 * @retval 1 file is available
 * @retval 0 file is not available from this position
 */
int Qaullib_FileAvailable(char *hash, char *suffix, int startbyte, struct qaul_file *file);

/**
 * try to download a file
 */
void Qaullib_FileConnect(int dbid, char *hash, char *suffix, int filesize, int downloaded, struct sockaddr_in *saddr);

/**
 * check file sockets for incoming traffic
 */
void Qaullib_FileCheckSockets(void);

/**
 * check if file @a path exists
 *
 * @retval 1 file exists
 * @retval 0 file does not exist
 */
int Qaullib_FileExists(char *path);

/**
 * delete a file by it's database @a id
 *
 * @retval 1 success
 * @retval 0 error
 */
int Qaullib_FileDeleteById(int id);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif
