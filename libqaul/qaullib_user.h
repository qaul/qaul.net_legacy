/*
 * qaul.net is free software
 * licensed under GPL (version 3)
 */

/**
 * user discovery and user management
 *
 * functions in the public API
 *   int Qaullib_ExistsUsername(void);
 * @see qaullib.h
 */

#ifndef _QAULLIB_USER
#define _QAULLIB_USER

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


/**
 * user connection structure
 */
struct qaul_user_connection
{
	struct qaul_wget_connection conn;
	struct qaul_user_LL_item   *user;
};

/**
 * array of user connections
 */
struct qaul_user_connection userconnections[MAX_USER_CONNECTIONS];

/**
 * initialize user tables
 * called once in qaullib_init
 */
void Qaullib_UserInit(void);

/**
 * get user name from DB
 */
const char* Qaullib_GetUsername(void);

/**
 * set user name
 */
int Qaullib_SetUsername(const char* name);

/**
 * check if Name for that ip exists
 *
 * @retval 0 user does not exist
 * @retval 1 user exists
 */
int Qaullib_UserCheckUser(union olsr_ip_addr *ip, char *name);

/**
 * check if ip exists & create it otherwise
 * insert @a ip of user in olsr table
 */
void Qaullib_UserTouchIp(union olsr_ip_addr *ip);

/**
 * schedule unnamed users for name download
 */
void Qaullib_UserCheckNonames(void);

/**
 * get user info via TCP from the users ip
 */
void Qaullib_UserGetInfo(struct qaul_user_LL_item *user);

/**
 * check user sockets (check if anything was received)
 */
void Qaullib_UserCheckSockets(void);

/**
 * add discovered user information to LL
 * wrapper for @see Qaullib_UserAdd()
 */
void Qaullib_UserAddInfo(struct qaul_userinfo_msg *userinfo);

/**
 * add discovered user information to LL
 * this function adds names to the user entries in the user LL
 * it creates new entries if a user does not exist yet
 */
void Qaullib_UserAdd(union olsr_ip_addr *ip, char *name, char *iconhash, char *suffix);

/**
 * add a favorite
 */
void Qaullib_UserFavoriteAdd(char *name, char *ipstr);

/**
 * delete a favorite
 */
void Qaullib_UserFavoriteRemove(char *ipstr);

/**
 * fill favorites from DB into LL
 */
void Qaullib_UserFavoritesDB2LL(void);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif
