/*
 * qaul.net is free software
 * licensed under GPL (version 3)
 */

/**
 * implements a SIP voice chat (uses pjsip library)
 *
 * functions in the public API
 * @see qaullib.h
 */

#ifndef _QAULLIB_VOIP
#define _QAULLIB_VOIP

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * 0 = no new call
 * 1 = new call
 */
int qaul_voip_new_call;

/**
 * name of the sip caller
 */
char qaul_voip_caller_name[MAX_USER_LEN +1];

/**
 * displays whether a new call event occurred
 *
 * 0 = no new event
 * 1 = ringing
 * 2 = calling - another user is calling us
 * 3 = connecting
 * 4 = connected
 * 5 = call ended - check for message
 */
int qaul_voip_event;

/**
 * sip code of call endings
 *
 * 486 = callee is busy
 * 487 = call was terminated
 * 4XX = user not reachable
 */
int qaul_voip_event_code;

/**
 * start VoIP
 *
 * @retval 1 sucessfully started
 * @retval 0 failed
 */
int Qaullib_VoipStart(void);

/**
 * stop VoIP and free memory
 */
int Qaullib_VoipStop(void);

/**
 * invoke a call to @a ip
 */
void Qaullib_VoipCallStart(char* ip);

/**
 * accept call
 */
void Qaullib_VoipCallAccept(void);

/**
 * end / reject current call
 */
void Qaullib_VoipCallEnd(void);


#ifdef __cplusplus
}
#endif // __cplusplus

#endif
