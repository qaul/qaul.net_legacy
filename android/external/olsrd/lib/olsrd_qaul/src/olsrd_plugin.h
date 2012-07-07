/*
 * written by contact@wachter-jud.net
 * License GPL
 */

#ifndef _OLSRD_PLUGIN_QAUL
#define _OLSRD_PLUGIN_QAUL

#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/times.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include <locale.h>
#include <wchar.h>

#include "olsr.h"
#include "plugin_util.h"
#include "olsr_types.h"
#include "../../../src/olsrd_plugin.h"
#include "neighbor_table.h"
#include "two_hop_neighbor_table.h"
#include "tc_set.h"
#include "hna_set.h"
#include "mid_set.h"
#include "mpr_selector_set.h"
#include "routing_table.h"


// qaul chat message
#define MESSAGE_TYPE 	222
#define PARSER_TYPE		MESSAGE_TYPE

#define PLUGIN_NAME    "qaul mesh plugin"
#define PLUGIN_VERSION "0.1"
#define PLUGIN_AUTHOR   "contact@wachter-jud.net"
#define MOD_DESC PLUGIN_NAME " " PLUGIN_VERSION " by " PLUGIN_AUTHOR
#define PLUGIN_INTERFACE_VERSION 5

// global variables
extern int qaul_ipc_port;


struct allowed_net {
  struct olsr_ip_prefix prefix;
  struct allowed_net *next;
};

/****************************************************************************
 *                Functions that the plugin MUST provide                    *
 ****************************************************************************/

/* Initialization function */
int olsrd_plugin_init(void);

int olsrd_plugin_interface_version(void);

#endif

