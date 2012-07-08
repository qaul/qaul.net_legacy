#ifndef _QAUL_CHAT_LL
#define _QAUL_CHAT_LL

#include "olsr_types.h"

void Qaul_Chat_LL_Init (void);
int  Qaul_Chat_LL_IsDuplicate (uint16_t seqno, union olsr_ip_addr *ip);
void Qaul_Chat_LL_Clean (void *foo __attribute__ ((unused)));


#endif
