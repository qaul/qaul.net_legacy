/*
 * written by contact@wachter-jud.net
 * License GPL
 */

#ifndef _OLSRD_QAUL_MESSAGES
#define _OLSRD_QAUL_MESSAGES

// ------------------------------------------------------------
// defines
// ------------------------------------------------------------
#define MAX_USER_LEN 20
#define MAX_MESSAGE_LEN 140
#define MAX_IP_LEN 40
#define MAX_FILE_LEN 33

// -------------------------------------------------------
// message definitions
// -------------------------------------------------------
// message to send
struct qaulchatmsg
{
  char name[MAX_USER_LEN];
  char msg[MAX_MESSAGE_LEN];
};

union qaulipc
{
	//char string[250];
	int integer;
};

struct qaulipcmsg
{
	int type;
	union qaulipc msg;
};

struct qaulnodemsg
{
	union olsr_ip_addr ip;
	union olsr_ip_addr gateway;
	uint32_t lq;
};

/*
 * todo: bundle the topo messages together
struct qaul_topomsg
{
	int count;
	struct qaul_topo topo[];
};
*/
#endif
