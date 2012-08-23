/*
 * qaul.net is free software
 * licensed under GPL (version 3)
 */

#include "tc_set.h"

#include "qaul_net.h"
#include "qaul_ipc.h"

// ------------------------------------------------------------
// network organisation
// ------------------------------------------------------------
// send network topology information via ipc
void qaul_net_topo2gui(void)
{
	struct tc_entry *tc;
	union olsr_message *m;
	char  buffer[512];
	float linkcost;

	// create new message
	m = (union olsr_message *)buffer;
	// fill message
	m->v4.olsr_msgtype = QAUL_IPCTOPO_MESSAGE_TYPE;
	m->v4.olsr_msgsize = htons(sizeof(struct qaul_node_msg) + sizeof(struct olsrmsg));

	// loop through the topology and add it to the message
	OLSR_FOR_ALL_TC_ENTRIES(tc)
	{
		struct tc_edge_entry *tc_edge;
		OLSR_FOR_ALL_TC_EDGE_ENTRIES(tc, tc_edge)
		{
		  if (tc_edge->edge_inv)
		  {
			  struct qaul_node_msg *node = (struct qaul_node_msg *) &m->v4.message;
			  // fill the message
			  linkcost = (float)tc_edge->cost / 1024;
			  memcpy(&node->ip, &tc_edge->T_dest_addr, sizeof(union olsr_ip_addr));
			  memcpy(&node->gateway, &tc->addr, sizeof(union olsr_ip_addr));
			  memcpy(&node->lq, &linkcost, sizeof(uint32_t));

			  // send the message
			  qaul_ipc_msg2gui(m);
		  }
		} OLSR_FOR_ALL_TC_EDGE_ENTRIES_END(tc, tc_edge);
	} OLSR_FOR_ALL_TC_ENTRIES_END(tc);

}




