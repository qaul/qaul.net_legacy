/*
 * qaul.net is free software
 * licensed under GPL (version 3)
 */

#ifndef _QAULLIB_PRIVATE
#define _QAULLIB_PRIVATE

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <stdint.h>

#ifdef WIN32
#include <winsock2.h>
#define SHUT_RDWR 2
#else
#include <sys/select.h> // fd_set (Android-NDK)
#include <netinet/in.h> // IPv6 address format in6_addr

// OpenBSD:
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#endif

#include "qaullib.h"
#include "olsrd_types.h"
#include "qaullib_defines.h"
#include "qaullib_webclient.h"
#include "qaullib_user_LL.h"
#include "qaullib_user.h"
#include "qaullib_threads.h"
#include "olsrd/mantissa.h"
#include "mongoose/mongoose.h"
#include "qaullib_webserver.h"
#include "sqlite/sqlite3.h"
#include "qaullib_sql.h"
#include "bstrlib/bstrlib.h"
#include "qaullib_filesharing.h"
#include "qaullib_ipc.h"
#include "qaullib_voip.h"

#ifndef WIN32
#include <unistd.h>             // close()
#endif


/**
 * for debugging messages switch this to 1
 */
#define QAUL_DEBUG              1

/**
 * global variables and structures
 */
struct mg_context *ctx;
sqlite3 *db;
char dbPath[MAX_PATH_LEN +1];
char webPath[MAX_PATH_LEN +1];
char qaullib_AppEventOpenPath[MAX_PATH_LEN +1];
int qaul_gui_pagename_set;
char qaullib_GuiPageName[MAX_PAGE_LEN +1];
struct sockaddr_in pin;
int ipc_socket, ipc_conn;
int qaul_new_msg;
int app_event;
int ipc_connected;
int qaul_loading_wait;
int qaul_conf_quit;
int qaul_conf_debug;
int qaul_conf_voip;
int qaul_web_localip_set;

union olsr_message *qaul_in_msg;

// todo: delete because not used anymore
/*
struct q_message_result
{
	int id;
	int type;
	char name[MAX_USER_LEN +1];
	char msg[MAX_MESSAGE_LEN +1];
	char ip[MAX_IP_LEN +1];
	int time;
};

struct q_user
{
	int id;
	char name[MAX_USER_LEN +1];
	char ip[MAX_IP_LEN +1];
	float lq;
	int time;
};
*/

/********************************************//**
 * configuration variables
 ***********************************************/
char qaul_username[MAX_USER_LEN +1];
int qaul_username_set;
char qaul_ip_str[MAX_IP_LEN +1];    // string of the IP
union olsr_ip_addr qaul_ip_addr;	// binary IP address
// ip
int qaul_ip_version;                // IP version 4/6
int qaul_ip_size;
int qaul_ip_set;
// network
char qaul_net_gateway[MAX_IP_LEN +1]; // string of the gateway IP
char qaul_net_ibss[255 +1];           // string of the IBSS name
char qaul_net_bssid[17 +1];           // string of the BSSID
// locale i18n
int qaul_locale_set;
char qaul_locale[MAX_LOCALE_LEN +1];
// finished
int qaul_configured;

/**
 * buffer and checker to write file path of
 * @see Qaullib_FilePicked(int, const char*)
 */
char pickFilePath[MAX_PATH_LEN +1];
int pickFileCheck;


/**
 * protect input string @a unprotected_string to insert it into the DB
 *
 * @retval protected string
 */
const char* Qaullib_ProtectString(const char* unprotected_string);

/**
 * unprotect protected string @a protected_string
 *
 * @retval unprotected string
 */
const char* Qaullib_UnprotectString(const char* protected_string);

/**
 * set IP @a protocol version (4 | 6)
 */
void Qaullib_SetProtocol(int protocol);

/**
 * set @a IP address
 *
 * @retval 1 success
 * @retval 0 error
 */
int Qaullib_SetIP(const char* IP);

/**
 * create an @a IP
 */
void Qaullib_CreateIP(char* IP);

/**
 * init the data base and check if table exists
 * called once at the beginning
 */
int  Qaullib_DbInit(void);

/**
 * insert a @a value for a configuration @a key
 */
void Qaullib_DbSetConfigValue(const char* key, const char* value);

/**
 * get config @a value from data base for @a key
 */
int  Qaullib_DbGetConfigValue(const char* key, char *value);

/**
 * write default configuration into config table
 */
void Qaullib_DbPopulateConfig(void);

/**
 * check if a UI language was selected by the user
 * if not show the language selection screen.
 */
int Qaullib_ExistsLocale(void);

/**
 * @retval UI language selection
 */
const char* Qaullib_GetLocale(void);

/**
 * set @a locale UI language
 */
void Qaullib_SetLocale(const char* locale);


#ifdef __cplusplus
}
#endif // __cplusplus

#endif
