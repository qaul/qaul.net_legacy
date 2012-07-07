/*
 * qaul.net is free software
 * licensed under GPL (version 3)
 */

#ifndef _QAULLIB
#define _QAULLIB

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


#define QAUL_EVENT_QUIT        99
#define QAUL_EVENT_CHOOSEFILE 100
#define QAUL_EVENT_OPENFILE   101

#define QAUL_ERROR_NOWIFI       1
#define QAUL_ERROR_NOROOT       2

/**
 * configuration procedure in the qaul
 *
 * initialize qaullib
 * invoke configuration functions
 *
 * startup configuration (0):
 *   start web server
 *
 * startup configuration (10):
 *   check if you have sufficient authorization rights
 *   request them if not
 *
 * startup configuration (20):
 *   check if wifi interface is available
 *   start wifi interface
 *   configure address
 *   connect to qaul.net
 *
 * startup configuration (30):
 *   check if user name was set
 *   wait until user name has been set
 *
 * startup configuration (40):
 *   start olsrd routing
 *
 * startup configuration (45):
 *   connect ipc
 *
 * startup configuration (50):
 *   start captive portal
 *   start port forwarding
 *   start timers to continuously invoke
 *   tell qaullib that configuration is finished
 */

/**
 * initialize qaullib
 * invoke this function once at the beginning, before any other qaullib function
 * @a resourcePath is the absolute path to the directory containing the olsrd and netcat executables
 * and the www folder
 */
void Qaullib_Init(const char* resourcePath);

/**
 * configure qaullib
 * enable Quit button in GUI
 * this is only used if the window has no exit button e.g. in android
 */
void Qaullib_SetConfQuit(void);

/**
 * configure qaullib
 * enable VoIP calls in GUI
 */
void Qaullib_SetConfVoIP(void);

/**
 * start web server
 *
 * @retval 1 web server started successfully
 * @retval 0 web server start error
 */
int Qaullib_WebserverStart(void);

/**
 * start GUI configuration (language, user name)
 * make sure all data is copied successfully
 */
void Qaullib_ConfigStart(void);

/**
 * start interprocess communication with olsrd_qaul plugin
 *
 * @retval 1 successfully connected
 * @retval 0 IPC connection failed
 */
int Qaullib_IpcConnect(void);

/**
 * check if user name exists
 *
 * @retval 1 user name exists
 * @retval 0 user name not set yet
 */
int Qaullib_ExistsUsername(void);

/**
 * get IP protocol version
 *
 * @retval 4 IPv4
 * @retval 6 IPv6
 */
int Qaullib_GetNetProtocol(void);

/**
 * get network mask as integer
 *
 * @retval  8 = "255.0.0.0"
 * @retval 16 = "255.255.0.0"
 * @retval 24 = "255.255.255.0"
 */
int Qaullib_GetNetMask(void);

/**
 * get network gateway
 *
 * @retval string of gateway e.g. "0.0.0.0"
 */
const char* Qaullib_GetNetGateway(void);

/**
 * get network ibss name
 *
 * @retval string of ibss name e.g. "qaul.net"
 */
const char* Qaullib_GetNetIbss(void);

/**
 * check if the bss id is set
 *
 * @retval 1 bss id is set
 * @retval 0 bss id not set
 */
int Qaullib_GetNetBssIdSet(void);

/**
 * get network ibss id
 *
 * @retval string of bss id e.g. "B6:B5:B3:F5:AB:E4"
 */
const char* Qaullib_GetNetBssId(void);

/**
 * get IP
 *
 * @retval string of IP e.g. "10.33.234.12"
 */
const char* Qaullib_GetIP(void);

/**
 * start captive portal
 *
 * @retval 1 sucessfully started
 * @retval 0 failed
 */
int Qaullib_CaptiveStart(void);

/**
 * tell qaullib to exit waiting screen
 */
void Qaullib_ConfigurationFinished(void);


/**
 * timed events must be called regularly from the qaul application
 */

/**
 * check if an event has occurred that needs action from the qaul application
 * to be called every 10ms / 100ms
 *
 * @retval QAUL_EVENT_QUIT       quit app
 * @retval QAUL_EVENT_CHOOSEFILE open file picker
 * @retval QAUL_EVENT_OPENFILE   open file
 */
int  Qaullib_TimedCheckAppEvent(void);

/**
 * check sockets for incoming traffic
 * to be called every 100ms
 */
void Qaullib_TimedSocketReceive(void);

/**
 * check if a file or a username needs to be downloaded
 * to be called every 15s
 */
void Qaullib_TimedDownload(void);

/**
 * send ipc command
 * check for network topology every 15 seconds
 *
 * @param commandId == 0: exit command for olsrd
 * @param commandId == 1: check network topology / nodes
 */
void Qaullib_IpcSendCom(int commandId);

/**
 * get the path of the file to open
 * to be called after Qaullib_TimedCheckEvent() received QAUL_EVENT_CHOOSEFILE
 */
const char* Qaullib_GetAppEventOpenPath(void);

/**
 * tell qaullib @a check if the user has picked a file
 * and send the @a path to the file.
 */
void Qaullib_FilePicked(int check, const char* path);

/**
 * invoke this function before exiting qaul
 */
void Qaullib_Exit(void);


#ifdef __cplusplus
}
#endif // __cplusplus

#endif
