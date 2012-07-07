/*
 * written by contact@wachter-jud.net
 * Licence GPL
 */

#ifndef _OLSRD_QAUL_CHAT
#define _OLSRD_QAUL_CHAT


#include "olsr.h"
#include "olsr_types.h"
#include "link_set.h"
#include "qaul_messages.h"


// -------------------------------------------------------
bool olsr_chat_parser(union olsr_message *, struct interface *, union olsr_ip_addr *);
void chat_send_all(struct qaulchatmsg* chat_message);
int chat_pack_msg (struct qaulchatmsg* amsg, struct qaulchatmsg* chat_message);
int qaul_chat_init(void);
void qaul_chat_autosend(void *foo __attribute__ ((unused)));


#endif
