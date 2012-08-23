/*
 * qaul.net is free software
 * licensed under GPL (version 3)
 */

#include "qaullib_private.h"
#include "olsrd/hashing.h"
#include "qaullib_user_LL.h"
#include "qaullib_threads.h"


int qaul_user_LL_count = 0;
int qaul_user_LL_id = 0;
struct qaul_user_LL_item Qaul_user_LL_table[HASHSIZE];


// ------------------------------------------------------------
void Qaullib_User_LL_Init (void)
{
  int i;

  for (i = 0; i < HASHSIZE; i++)
  {
	  Qaul_user_LL_table[i].next = &Qaul_user_LL_table[i];
	  Qaul_user_LL_table[i].prev = &Qaul_user_LL_table[i];
  }
}

// ------------------------------------------------------------
void Qaullib_User_LL_InitNode(struct qaul_user_LL_node *node)
{
	node->index = 0;
	node->item = &Qaul_user_LL_table[0];
}

void Qaullib_User_LL_InitNodeWithIP(struct qaul_user_LL_node *node, union olsr_ip_addr *ip)
{
	node->index = olsr_ip_hashing(ip);
	node->item = &Qaul_user_LL_table[node->index];
}

int Qaullib_User_LL_NextNode (struct qaul_user_LL_node *node)
{
	for(; node->index < HASHSIZE;)
	{
		if(node->item->next != &Qaul_user_LL_table[node->index])
		{
			node->item = node->item->next;
			return 1;
		}
		node->index++;
		node->item =  &Qaul_user_LL_table[node->index];
	}
	return 0;
}

int Qaullib_User_LL_NextItem (struct qaul_user_LL_node *node)
{
	if(node->item->next != &Qaul_user_LL_table[node->index])
	{
		node->item = node->item->next;
		return 1;
	}
	return 0;
}

// ------------------------------------------------------------
struct qaul_user_LL_item* Qaullib_User_LL_Add (union olsr_ip_addr *ip)
{
	struct qaul_user_LL_item *new_item;
	new_item = (struct qaul_user_LL_item *)malloc(sizeof(struct qaul_user_LL_item));

	if(QAUL_DEBUG)
		printf("Qaullib_User_LL_Add\n");

	// get index
	uint32_t hash = olsr_ip_hashing(ip);

	// fill in content
	qaul_user_LL_id++;
	new_item->time = time(NULL);
	new_item->type = QAUL_USERTYPE_UNCHECKED;
	new_item->changed = QAUL_USERCHANGED_UNCHANGED;
	new_item->lq = 10.1;
	new_item->favorite = 0;
	memcpy((char *)&new_item->ip, ip, sizeof(union olsr_ip_addr));

	// lock
	pthread_mutex_lock( &qaullib_mutex_userLL );
	// create links
	new_item->prev = &Qaul_user_LL_table[hash];
	new_item->next = Qaul_user_LL_table[hash].next;

	Qaul_user_LL_table[hash].next = new_item;
	new_item->next->prev = new_item;
	qaul_user_LL_count++;

	// unlock
	pthread_mutex_unlock( &qaullib_mutex_userLL );

	return new_item;
}


// ------------------------------------------------------------
void Qaullib_User_LL_Delete_Item (struct qaul_user_LL_item *item)
{
	if(QAUL_DEBUG)
		printf("Qaullib_User_LL_Delete_Item ");

	// lock
	pthread_mutex_lock( &qaullib_mutex_userLL );

	item->prev->next = item->next;
	item->next->prev = item->prev;
	qaul_user_LL_count--;

	// unlock
	pthread_mutex_unlock( &qaullib_mutex_userLL );

	free(item);
}


// ------------------------------------------------------------
void Qaullib_User_LL_Clean (void)
{
	struct qaul_user_LL_node mynode;
	Qaullib_User_LL_InitNode(&mynode);

	while(Qaullib_User_LL_NextNode(&mynode))
	{
		if(mynode.item->time +300 < time(NULL))
		{
			// only delete if not a favorite
			if(mynode.item->favorite == 0)
			{
				mynode.item = mynode.item->prev;
				Qaullib_User_LL_Delete_Item(mynode.item->next);
			}
		}
		else if(mynode.item->changed == 0 && mynode.item->time +10 < time(NULL))
		{
			if(mynode.item->type > QAUL_USERTYPE_UNCHECKED)
				mynode.item->changed = QAUL_USERTYPE_KNOWN;
		}
	}
}


// ------------------------------------------------------------
int Qaullib_User_LL_IpExists (union olsr_ip_addr *ip)
{
	//printf("[LL next] Qaullib_User_LL_IpExists\n");

	struct qaul_user_LL_node mynode;
	Qaullib_User_LL_InitNodeWithIP(&mynode, ip);
	while(Qaullib_User_LL_NextItem(&mynode))
	{
		if(memcmp(&mynode.item->ip, ip, qaul_ip_size) == 0)
		{
			return 1;
		}
	}
	return 0;
}


// ------------------------------------------------------------
int Qaullib_User_LL_IpSearch (union olsr_ip_addr *ip, struct qaul_user_LL_item **item)
{
	//printf("[LL next] Qaullib_User_LL_IpSearch\n");

	struct qaul_user_LL_node mynode;
	Qaullib_User_LL_InitNodeWithIP(&mynode, ip);
	while(Qaullib_User_LL_NextItem(&mynode))
	{
		if(memcmp(&mynode.item->ip, ip, qaul_ip_size) == 0)
		{
			*item = mynode.item;
			return 1;
		}
	}
	return 0;
}


