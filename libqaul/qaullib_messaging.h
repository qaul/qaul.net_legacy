/*
 * qaul.net is free software
 * licensed under GPL (version 3)
 */

#ifndef _QAULLIB_MESSAGING
#define _QAULLIB_MESSAGING


#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


/**
 * Initialize file table and read files from data base.
 * Called once in qaullib_init.
 */
void Qaullib_MsgInit(void);

/**
 * Fill most recent Messages from DB into LL.
 * This function is only called once at start up.
 */
void Qaullib_MsgDB2LL(void);

/**
 * Get Messages from the DB by executing @a stmt.
 * Put the messages in a temporary LL to be delivered
 * to the web front end.
 * Set the pointer in the @a node to the start item
 * in the LL.
 *
 * @retval 0 no message available
 * @retval 1 messages are available
 */
int Qaullib_MsgDB2LLTmp(struct qaul_msg_LL_node *node, char *stmt);

/**
 * add a new message to data base and LL
 *
 * @retval 0 on error
 * @retval 1 on success
 */
int Qaullib_MsgAdd(struct qaul_msg_LL_item *item);

/**
 * send a public message and save it
 *
 * @retval 0 on error
 * @retval 1 on success
 */
int Qaullib_MsgSendPublic(struct qaul_msg_LL_item *item);

/**
 * send a public message for a web client and save it
 *
 * @retval 0 on error
 * @retval 1 on success
 */
int Qaullib_MsgSendPublicWeb(struct qaul_msg_LL_item *item);

/**
 * send a private message and save it
 *
 * @retval 0 on error
 * @retval 1 on success
 */
int Qaullib_MsgSendPrivate(struct qaul_msg_LL_item *item);

/**
 * receive a public message
 *
 * @retval 0 on error
 * @retval 1 on success
 */
int Qaullib_MsgReceivePublic(struct qaul_msg_LL_item *item);

/**
 * receive a private message
 *
 * @retval 0 on error
 * @retval 1 on success
 */
int Qaullib_MsgReceivePrivate(struct qaul_msg_LL_item *item);


#ifdef __cplusplus
}
#endif // __cplusplus

#endif
