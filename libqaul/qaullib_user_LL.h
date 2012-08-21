/*
 * qaul.net is free software
 * licensed under GPL (version 3)
 */

#ifndef _QAULLIB_USER_LL
#define _QAULLIB_USER_LL

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * the user table contains an array of linked lists.
 * the user entries are added to a linked list in the array according
 * to the hash of their ip address.
 * (The hash is created from the last octet of the ip address.)
 */

struct qaul_user_LL_item {
	struct qaul_user_LL_item *next;           /// next node
	struct qaul_user_LL_item *prev;           /// previous node
	union olsr_ip_addr ip;                    /// ip address
	time_t             time;                  /// time when last seen
	float              lq;                    /// link quality
	char               name[MAX_USER_LEN +1]; /// user name
	char               icon[MAX_FILENAME_LEN +1]; /// icon name
	int                type;                  /// type of user: see user types
	int                changed;               /// changes for GUI notifications: see user changed
	int				   favorite;			  /// user is favorite (don't delete it)
};

struct qaul_user_LL_node {
	struct qaul_user_LL_item *item;           /// link to the LL item
	uint32_t                  index;          /// array index of the node
};

/**
 * user types
 * this definitions indicate the type and status of a user
 */
#define QAUL_USERTYPE_UNCHECKED   0 /// this user is unknown yet
#define QAUL_USERTYPE_HIDDEN     -2 /// user shall not be shown in user discovery
#define QAUL_USERTYPE_ERROR      -1 /// ERROR downloading, might be infrastructure node
#define QAUL_USERTYPE_DOWNLOADING 1 /// trying to download the user name from this user
#define QAUL_USERTYPE_KNOWN       2 /// known user

/**
 * user changed
 * this definitions show wheter a update needs to be sent to the GUI
 */
#define QAUL_USERCHANGED_UNCHANGED 0 /// user is online and unchanged
#define QAUL_USERCHANGED_MODIFIED  1 /// user was added or modified
#define QAUL_USERCHANGED_DELETED   2 /// user unreachable (offline), needs to be deleted form GUI
#define QAUL_USERCHANGED_CACHED    3 /// user is offline but user name is stored for a while

/**
 * to be called once at startup to create linked list
 */
void Qaullib_User_LL_Init (void);

/**
 * creates a new list entry for @a ip in the user table.
 *
 * @retval pointer to the item
 */
struct qaul_user_LL_item* Qaullib_User_LL_Add (union olsr_ip_addr *ip);

/**
 * delete @a item from list
 */
void Qaullib_User_LL_Delete_Item (struct qaul_user_LL_item *item);

/**
 * loops through the list and deletes all expired entries
 * mark all clients older than 30 seconds as deleted
 * remove all clients which are older than 5 minutes
 */
void Qaullib_User_LL_Clean (void);

/**
 * loops through the list and checks if @a ip exists
 *
 * @retval 1 ip exists
 * @retval 0 ip does not exist
 */
int  Qaullib_User_LL_IpExists (union olsr_ip_addr *ip);

/**
 * loops through the list and checks if @a ip exists.
 * If it exists, @a item will contain the pointer to it.
 *
 * @retval 1 entry exists
 * @retval 0 entry does not exist
 */
int  Qaullib_User_LL_IpSearch (union olsr_ip_addr *ip, struct qaul_user_LL_item **item);

/**
 * initializes a @a node with the first entry of the user table
 */
void Qaullib_User_LL_InitNode (struct qaul_user_LL_node *node);

/**
 * initializes a @a node of the user table according to the @a ip
 */
void Qaullib_User_LL_InitNodeWithIP(struct qaul_user_LL_node *node, union olsr_ip_addr *ip);

/**
 * checks if there is a next item in the user table
 *
 * @retval 1 next node found, @a node contains the pointer to the item
 * @retval 0 no next node exists
 */
int  Qaullib_User_LL_NextNode (struct qaul_user_LL_node *node);



#ifdef __cplusplus
}
#endif // __cplusplus

#endif
