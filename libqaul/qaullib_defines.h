/*
 * qaul.net is free software
 * licensed under GPL (version 3)
 */

#ifndef _QAULLIB_DEFINES
#define _QAULLIB_DEFINES

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


#define TIMEOUT_LASTRECEIVED 20
#define TIMEOUT_CONNECTED 60

#define MAX_SUFFIX_LEN 5
#define MAX_DESCRIPTION_LEN 80
#define MAX_HASH_LEN 20
#define MAX_HASHSTR_LEN 40
#define MAX_INTSTR_LEN 10
#define MAX_PAGE_LEN 20
#define MAX_LOCALE_LEN 6
#define MAX_VARCHAR_LEN 255

#ifdef WIN32
#define PATH_SEPARATOR "\\"
#define PATH_SEPARATOR_INT '\\'
#define MAX_PATH_LEN MAX_PATH
#else
#define PATH_SEPARATOR "/"
#define PATH_SEPARATOR_INT '/'
#define MAX_PATH_LEN PATH_MAX
#endif


/********************************************//**
 * Qaul Messages
 *
 * message structures for the interprocess communication
 ***********************************************/

struct qaul_userinfo_msg
{
	union olsr_ip_addr ip;
	char name[MAX_USER_LEN];
	char icon[MAX_HASH_LEN];
	char suffix[MAX_SUFFIX_LEN];
};

struct qaul_filechunk_msg
{
	uint32_t type;  // type 0 => error: file does not exists
					// type 1 => chunk
					// type 3 => error: requested chunk position is bigger than file
	uint32_t filesize;
	uint32_t chunksize;
	char chunkhash[MAX_HASH_LEN];
	//char chunk;
};

union qaul_inbuf
{
	char buf[BUFFSIZE + 1];
	struct qaul_userinfo_msg userinfo;
	struct qaul_filechunk_msg filechunk;
};

#ifdef __cplusplus
}
#endif // __cplusplus

#endif
