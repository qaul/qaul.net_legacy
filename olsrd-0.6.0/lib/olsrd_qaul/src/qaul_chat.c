/*
 * written by contact@wachter-jud.net
 * License GPL
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <olsr_protocol.h>
#include "net_olsr.h"
#include "scheduler.h"
#include "parser.h" // olsr_parser_add

#include "olsrd_plugin.h"
#include "qaul_config.h"
#include "qaul_chat.h"
#include "qaul_chat_LL.h"
#include "qaul_ipc.h"

// ------------------------------------------------------------
// variables
// ------------------------------------------------------------
int qaul_chat_counter = 0;

// ------------------------------------------------------------
// Parse olsr chat message
// ------------------------------------------------------------
bool
olsr_chat_parser(union olsr_message *m, struct interface *in_if __attribute__ ((unused)), union olsr_ip_addr *ipaddr)
{
  //struct qaulchatmsg *qaulmessage;
  union olsr_ip_addr originator;
  //olsr_reltime vtime;
  //int size;
  uint16_t seqno;

  olsr_printf(1, "---------------------------------------------------\n");
  olsr_printf(1, "[QAUL] Receiving Message! \n");

  // Fetch the originator & the id of the messsage
  if (olsr_cnf->ip_version == AF_INET) {
    memcpy(&originator, &m->v4.originator, olsr_cnf->ipsize);
    seqno = ntohs(m->v4.seqno);
  } else {
    memcpy(&originator, &m->v6.originator, olsr_cnf->ipsize);
    seqno = ntohs(m->v6.seqno);
  }

  // Check if message originated from this node
  if (ipequal(&originator, &olsr_cnf->main_addr))
    return false;

  // Check for duplicates
  if (Qaul_Chat_LL_IsDuplicate (seqno, &originator))
	  return false;

  // send it to the qaul application via ipc
  qaul_ipc_msg2gui(m);

  // Forward the message
  return true;
}


// ------------------------------------------------------------
// generate and send Chat Message
// ------------------------------------------------------------
void
chat_send_all(struct qaulchatmsg* chat_message)
{
  // send buffer: huge
  char buffer[10240];
  union olsr_message *message = (union olsr_message *)buffer;
  struct interface *ifn;
  int size;
  int my_timeout = 30;

  OLSR_PRINTF(1, "[QAUL] send message: %s %s\n", chat_message->name, chat_message->msg);

  // fill message
  if (olsr_cnf->ip_version == AF_INET) {
    /* IPv4 */
    message->v4.olsr_msgtype = MESSAGE_TYPE;
    message->v4.olsr_vtime = reltime_to_me(my_timeout * MSEC_PER_SEC);
    memcpy(&message->v4.originator, &olsr_cnf->main_addr, olsr_cnf->ipsize);
    message->v4.ttl = MAX_TTL;
    message->v4.hopcnt = 0;
    message->v4.seqno = htons(get_msg_seqno());

#ifdef VERS05
	size = chat_pack_msg (( struct qaulchatmsg *)(ARM_NOWARN_ALIGN)&message->v4.message, chat_message);
#else
	size = chat_pack_msg (( struct qaulchatmsg *)ARM_NOWARN_ALIGN(&message->v4.message), chat_message);
#endif
	size = size + sizeof(struct olsrmsg);

    message->v4.olsr_msgsize = htons(size);
  } else {
    /* IPv6 */
    message->v6.olsr_msgtype = MESSAGE_TYPE;
    message->v6.olsr_vtime = reltime_to_me(my_timeout * MSEC_PER_SEC);
    memcpy(&message->v6.originator, &olsr_cnf->main_addr, olsr_cnf->ipsize);
    message->v6.ttl = MAX_TTL;
    message->v6.hopcnt = 0;
    message->v6.seqno = htons(get_msg_seqno());

#ifdef VERS05
	size = chat_pack_msg (( struct qaulchatmsg *)(ARM_NOWARN_ALIGN)&message->v4.message, chat_message);
#else
	size = chat_pack_msg (( struct qaulchatmsg *)ARM_NOWARN_ALIGN(&message->v6.message), chat_message);
#endif
	size = size + sizeof(struct olsrmsg6);

    message->v6.olsr_msgsize = htons(size);
  }

  /* looping trough interfaces */
  for (ifn = ifnet; ifn; ifn = ifn->int_next) {
    OLSR_PRINTF(1, "QAUL: Generating packet - [%s]\n", ifn->int_name);

    if (net_outbuffer_push(ifn, message, size) != size) {
      /* send data and try again */
      net_output(ifn);
      if (net_outbuffer_push(ifn, message, size) != size) {
        OLSR_PRINTF(1, "QAUL: could not send on interface: %s\n", ifn->int_name);
      }
    }
  }
}


int chat_pack_msg (struct qaulchatmsg *amsg, struct qaulchatmsg* chat_message)
{
	strcpy(amsg->name, chat_message->name);
	strcpy(amsg->msg, chat_message->msg);
	return sizeof( struct qaulchatmsg);
}


// ------------------------------------------------------------
// initialize Chat
// ------------------------------------------------------------

int qaul_chat_init(void)
{
	// init linked list
	Qaul_Chat_LL_Init ();

	// Register Chat Message Parser
	olsr_parser_add_function(&olsr_chat_parser, PARSER_TYPE);

	// schedule message cleaning
	olsr_start_timer(3 * MSEC_PER_SEC, 0, OLSR_TIMER_PERIODIC, &Qaul_Chat_LL_Clean, NULL, 0);

	// automatically send a chat message every 3 seconds
	//olsr_start_timer(3 * MSEC_PER_SEC, 0, OLSR_TIMER_PERIODIC, &qaul_chat_autosend, NULL, 0);

	return 1;
}


// ------------------------------------------------------------
// Test Messages
// ------------------------------------------------------------
// Send every 3 seconds a message

void qaul_chat_autosend(void *foo __attribute__ ((unused)))
{
	struct qaulchatmsg chat_message;
	//olsr_printf(1, "[QAUL] send test: %i\n", qaul_chat_counter);
	strcpy(chat_message.name, "[qaul test]");
	sprintf(chat_message.msg, "%i", qaul_chat_counter);
	chat_send_all(&chat_message);
	qaul_chat_counter++;
	return;
}
