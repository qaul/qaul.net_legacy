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


#ifdef __cplusplus
}
#endif // __cplusplus

#endif
