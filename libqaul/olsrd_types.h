/*
 * qaul.net is free software
 * licensed under GPL (version 3)
 */

#ifndef _OLSRD_TYPES
#define _OLSRD_TYPES

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#ifdef WIN32
#include <winsock2.h>
#include <ws2tcpip.h>

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;
#else
#ifndef INVALID_SOCKET
	#define INVALID_SOCKET (-1)
#endif
#include <sys/types.h>
#include <inttypes.h>
#endif

#include "olsrd/mantissa.h"
#include "olsrd/olsr_types.h"

// --------------------------------------------
// OLSRD Datatypes
// --------------------------------------------
#ifdef WIN32
typedef unsigned char olsr_u8_t;
typedef unsigned short olsr_u16_t;
typedef unsigned int olsr_u32_t;
typedef char olsr_8_t;
typedef short olsr_16_t;
typedef int olsr_32_t;
#else
typedef u_int8_t olsr_u8_t;
typedef u_int16_t olsr_u16_t;
typedef u_int32_t olsr_u32_t;
typedef int8_t olsr_8_t;
typedef int16_t olsr_16_t;
typedef int32_t olsr_32_t;
#endif


#define MAX(x,y)	((x) > (y) ? (x) : (y))
#define CLOSE(fd)  do { close(fd); (fd) = -1; } while (0)

// olsr_reltime is a relative timestamp measured in microseconds
//typedef uint32_t olsr_reltime;


/*
struct ipv6_addr {
  unsigned char s6_addr[16];
};
*/
/*
union olsr_ip_addr {
  olsr_u32_t v4;
  //struct in6_addr v6;
  //struct ipv6_addr v6;
};
*/


// ------------------------------------------------------------
// defines
// ------------------------------------------------------------
#define MAX_USER_LEN 20
#define MAX_MESSAGE_LEN 140
#define MAX_FILENAME_LEN 46
#define MAX_IP_LEN 40
#define IPC_PORT 8112
#define BUFFSIZE 8192
#define IPC_ADDR "127.0.0.1"
#define CHAT_PORT "8081"
#define WEB_PORT 8081
#define MAX_USER_CONNECTIONS 3
#define MAX_FILE_CONNECTIONS 5

// --------------------------------------------
// Qaul Olsr Messages
// --------------------------------------------
// Message Codes
// 222: qaul chat message
// 223: ipc commands
// 224: network topology
//
// Ipc Commands
// 0: close olsrd
//

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

// --------------------------------------------
// Olsrd Messages
// --------------------------------------------

/*
 *The HELLO message
 */

/*
 *Hello info
 */
struct hellinfo {
  olsr_u8_t link_code;
  olsr_u8_t reserved;
  olsr_u16_t size;
  olsr_u32_t neigh_addr[1];            /* neighbor IP address(es) */
};

struct hellomsg {
  olsr_u16_t reserved;
  olsr_u8_t htime;
  olsr_u8_t willingness;
  struct hellinfo hell_info[1];
};

/*
 *IPv6
 */

struct hellinfo6 {
  olsr_u8_t link_code;
  olsr_u8_t reserved;
  olsr_u16_t size;
  struct in6_addr neigh_addr[1];       /* neighbor IP address(es) */
};

struct hellomsg6 {
  olsr_u16_t reserved;
  olsr_u8_t htime;
  olsr_u8_t willingness;
  struct hellinfo6 hell_info[1];
};

/*
 * Topology Control packet
 */

struct neigh_info {
  olsr_u32_t addr;
};

struct olsr_tcmsg {
  olsr_u16_t ansn;
  olsr_u16_t reserved;
  struct neigh_info neigh[1];
};

/*
 *IPv6
 */

struct neigh_info6 {
  struct in6_addr addr;
};

struct olsr_tcmsg6 {
  olsr_u16_t ansn;
  olsr_u16_t reserved;
  struct neigh_info6 neigh[1];
};

/*
 *Multiple Interface Declaration message
 */

/*
 * Defined as s struct for further expansion
 * For example: do we want to tell what type of interface
 * is associated whit each address?
 */
struct midaddr {
  olsr_u32_t addr;
};

struct midmsg {
  struct midaddr mid_addr[1];
};

/*
 *IPv6
 */
struct midaddr6 {
  struct in6_addr addr;
};

struct midmsg6 {
  struct midaddr6 mid_addr[1];
};

/*
 * Host and Network Association message
 */
struct hnapair {
  olsr_u32_t addr;
  olsr_u32_t netmask;
};

struct hnamsg {
  struct hnapair hna_net[1];
};

/*
 *IPv6
 */

struct hnapair6 {
  struct in6_addr addr;
  struct in6_addr netmask;
};

struct hnamsg6 {
  struct hnapair6 hna_net[1];
};

/*
 * OLSR message (several can exist in one OLSR packet)
 */

struct olsrmsg {
  olsr_u8_t olsr_msgtype;
  olsr_u8_t olsr_vtime;
  olsr_u16_t olsr_msgsize;
  olsr_u32_t originator;
  olsr_u8_t ttl;
  olsr_u8_t hopcnt;
  olsr_u16_t seqno;

  union {
    struct hellomsg hello;
    struct olsr_tcmsg tc;
    struct hnamsg hna;
    struct midmsg mid;
    
    // my messages
    struct qaulchatmsg chat;
    struct qaulipcmsg ipc;
    struct qaulnodemsg node;
  } message;

};

/*
 *IPv6
 */

struct olsrmsg6 {
  olsr_u8_t olsr_msgtype;
  olsr_u8_t olsr_vtime;
  olsr_u16_t olsr_msgsize;
  struct in6_addr originator;
  olsr_u8_t ttl;
  olsr_u8_t hopcnt;
  olsr_u16_t seqno;

  union {
    struct hellomsg6 hello;
    struct olsr_tcmsg6 tc;
    struct hnamsg6 hna;
    struct midmsg6 mid;
    
    // my messages
    struct qaulchatmsg chat;
    struct qaulipcmsg ipc;
    struct qaulnodemsg node;
  } message;

};

/*
 * Generic OLSR packet
 */

struct olsr {
  olsr_u16_t olsr_packlen;             /* packet length */
  olsr_u16_t olsr_seqno;
  struct olsrmsg olsr_msg[1];          /* variable messages */
};

struct olsr6 {
  olsr_u16_t olsr_packlen;             /* packet length */
  olsr_u16_t olsr_seqno;
  struct olsrmsg6 olsr_msg[1];         /* variable messages */
};

/* IPv4 <-> IPv6 compability */

union olsr_message {
  struct olsrmsg v4;
  struct olsrmsg6 v6;
};

union olsr_packet {
  struct olsr v4;
  struct olsr6 v6;
};


// -----------------------------------------------------

#ifdef __cplusplus
}
#endif // __cplusplus

#endif


