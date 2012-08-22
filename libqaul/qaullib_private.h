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
#include "qaullib_file_LL.h"
#include "qaullib_filesharing.h"
#include "qaullib_exediscovery.h"
#include "qaullib_ipc.h"
#include "qaullib_voip.h"
#include "qaullib_udp_communication.h"

#ifndef WIN32
#include <unistd.h>             // close()
#endif


/**
 * extension for the Ars Electronica Edition
 *
 * UTC timestamps of the rehearsal of the Klangwolke and of the Klangwolke
 *
 * rehearsal:
 * local time: 30. August 2012 19:00 Ð 21:00
 * UTC:        30. August 2012 17:00 Ð 19:00
 * UTC time stamp: 1346346000 - 1346353200
 *
 * Klangwolke:
 * local time: 1. September 2012 20:00 Ð 22:00
 * UTC:        1. September 2012 18:00 Ð 18:00
 * UTC time stamp: 1346522400 - 1346529600
 */
#define ARS_AUS_1   1346346000
#define ARS_EIN_1   1346353200
#define ARS_AUS_2   1346522400
#define ARS_EIN_2   1346529600


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
int qaul_conf_ios;
int qaul_conf_wifi_set;
int qaul_web_localip_set;
int qaul_exe_available;
char qaullib_AppEventOpenURL[MAX_URL_LEN +1];

char qaullib_FileDownloadFolderPath[MAX_PATH_LEN +1];
int  qaul_conf_filedownloadfolder_set;

union olsr_message *qaul_in_msg;


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
 * create a save file name from the file @a description the @a hashstr of the file and
 * the @a suffix
 *
 * @retval returns the string size of the file name
 */
int Qaullib_StringDescription2Filename(char *filename, struct qaul_file_LL_item *file, int buffer_size);

/**
 * checks and protects a message string
 *
 * @retval protected string size
 */
int Qaullib_StringMsgProtect(char *protected_string, char *unprotected_string, int buffer_size);

/**
 * checks and protects a user name string
 *
 * @retval protected string size
 */
int Qaullib_StringNameProtect(char *protected_string, char *unprotected_string, int buffer_size);

/**
 * protects a string for that it is save to send it via Json
 *
 * @retval the size of the protected string
 */
int Qaullib_StringJsonProtect(char *protected_string, char *unprotected_string, int buffer_size);

/**
 * protects a string to be ready to write it to the data base
 *
 * @retval the size of the protected string
 */
int Qaullib_StringDbProtect(char *protected_string, char *unprotected_string, int buffer_size);

/**
 * removes the protection characters from a protected data base string
 *
 * @retval the size of the unprotected string
 */
int Qaullib_StringDbUnprotect(char *unprotected_string, char *protected_string, int buffer_size);

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

/**
 * convert a unix time stamp into an local ISO date format
 *
 * @retval 1 success
 * @retval 0 error
 */
int Qaullib_Timestamp2Isostr(char *isostr, int timestamp, int buffer_size);


#ifdef __cplusplus
}
#endif // __cplusplus

#endif
