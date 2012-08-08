/*
 * qaul.net is free software
 * licensed under GPL (version 3)
 */

#include "qaullib_private.h"
#include "olsrd/hashing.h"
#include "qaullib_threads.h"
#include "qaullib_file_LL.h"


int qaul_file_LL_count;
struct qaul_file_LL_item Qaul_file_LL_table[HASHSIZE];


/**
 * Hashing function. Creates a key based on an @a filehash
 *
 * @retval the hash(a value in the (0 to HASHMASK-1) range)
 */
static uint32_t Qaullib_File_LL_Hashing(unsigned char* filehash);

// ------------------------------------------------------------
void Qaullib_File_LL_Init (void)
{
  int i;

  qaul_file_LL_count = 0;

  for (i = 0; i < HASHSIZE; i++)
  {
	  Qaul_file_LL_table[i].next = &Qaul_file_LL_table[i];
	  Qaul_file_LL_table[i].prev = &Qaul_file_LL_table[i];
  }
}


// ------------------------------------------------------------
void Qaullib_File_LL_InitNode(struct qaul_file_LL_node *node)
{
	node->index = 0;
	node->item = &Qaul_file_LL_table[0];
}

// ------------------------------------------------------------
void Qaullib_File_LL_InitNodeWithHash(struct qaul_file_LL_node *node, char *filehash)
{
	node->index = Qaullib_File_LL_Hashing(filehash);
	node->item = &Qaul_file_LL_table[node->index];
}

// ------------------------------------------------------------
int Qaullib_File_LL_NextNode (struct qaul_file_LL_node *node)
{
	for(; node->index < HASHSIZE;)
	{
		if(node->item->next != &Qaul_file_LL_table[node->index])
		{
			node->item = node->item->next;
			return 1;
		}
		node->index++;
		node->item =  &Qaul_file_LL_table[node->index];
	}
	return 0;
}

// ------------------------------------------------------------
int Qaullib_File_LL_NextNodePub (struct qaul_file_LL_node *node)
{
	while(Qaullib_File_LL_NextNode(node))
	{
		if(node->item->type == QAUL_FILETYPE_FILE && node->item->status >= QAUL_FILESTATUS_DOWNLOADED)
			return 1;
	}
	return 0;
}

// ------------------------------------------------------------
int Qaullib_File_LL_NextNodePubBinaries (struct qaul_file_LL_node *node)
{
	while(Qaullib_File_LL_NextNode(node))
	{
		if(node->item->type == QAUL_FILETYPE_EXECUTABLE && node->item->status >= QAUL_FILESTATUS_DOWNLOADED)
			return 1;
	}
	return 0;
}

// ------------------------------------------------------------
int Qaullib_File_LL_NextNodeGuiPriv (struct qaul_file_LL_node *node)
{
	while(Qaullib_File_LL_NextNode(node))
	{
		if(node->item->gui_notify == 1)
		{
			if(node->item->type == 1)
				return 1;
			else
				node->item->gui_notify = 0;
		}
	}
	return 0;
}


/**
 * Checks whether there is a next item within hashed linked list.
 * The function links the item into the @a node
 *
 * @retval 1 there is a next item
 * @retval 0 there is no next item
 */
int Qaullib_File_LL_NextItem (struct qaul_file_LL_node *node)
{
	if(node->item->next != &Qaul_file_LL_table[node->index])
	{
		node->item = node->item->next;
		return 1;
	}
	return 0;
}

// ------------------------------------------------------------
struct qaul_file_LL_item* Qaullib_File_LL_Add (struct qaul_file_LL_item *item)

{
	struct qaul_file_LL_item *new_item;
	new_item = (struct qaul_file_LL_item *)malloc(sizeof(struct qaul_file_LL_item));

//	// create file hash from hash string
//	Qaullib_StringToHash(filehashstr, new_item->hash);

	printf("Qaullib_File_LL_Add hash: %s\n", item->hashstr);

	// get index
	uint32_t LLhash = Qaullib_File_LL_Hashing(item->hash);

	// fill in content
	memcpy(&new_item->hash, item->hash, MAX_HASH_LEN);
	memcpy(&new_item->hashstr, item->hashstr, MAX_HASHSTR_LEN);
	memcpy(&new_item->hashstr[MAX_HASHSTR_LEN], "\0", 1);
	strncpy(new_item->suffix, item->suffix, MAX_SUFFIX_LEN);
	memcpy(&new_item->suffix[MAX_SUFFIX_LEN], "\0", 1);
	strncpy(new_item->description, item->description, MAX_DESCRIPTION_LEN);
	memcpy(&new_item->description[MAX_DESCRIPTION_LEN], "\0", 1);
	strncpy(new_item->created_at, item->created_at, MAX_TIME_LEN);
	memcpy(&new_item->created_at[MAX_TIME_LEN], "\0", 1);

	new_item->id = item->id;
	new_item->type = item->type;
	new_item->status = item->status;
	new_item->size = item->size;
	new_item->downloaded = item->downloaded;

	strncpy(new_item->adv_name, item->adv_name, MAX_USER_LEN);
	memcpy(&new_item->adv_name[MAX_USER_LEN], "\0", 1);
	memcpy(&new_item->adv_ip, &item->adv_ip, sizeof(union olsr_ip_addr));
	new_item->adv_validip = item->adv_validip;

	new_item->gui_notify = 1;

	// lock
	pthread_mutex_lock( &qaullib_mutex_fileLL );
	// create links
	new_item->prev = &Qaul_file_LL_table[LLhash];
	new_item->next = Qaul_file_LL_table[LLhash].next;

	Qaul_file_LL_table[LLhash].next = new_item;
	new_item->next->prev = new_item;
	qaul_file_LL_count++;

	// unlock
	pthread_mutex_unlock( &qaullib_mutex_fileLL );

	// check if file exists
	if(Qaullib_File_LL_HashExists(new_item->hash))
		printf("file found in LL\n");
	else
		printf("file not found in LL\n");

	return new_item;
}


// ------------------------------------------------------------
void Qaullib_File_LL_Delete_Item (struct qaul_file_LL_item *item)
{
	printf("Delete item\n");

	// lock
	pthread_mutex_lock( &qaullib_mutex_fileLL );

	item->prev->next = item->next;
	item->next->prev = item->prev;
	qaul_file_LL_count--;

	// unlock
	pthread_mutex_unlock( &qaullib_mutex_fileLL );

	free(item);
}


// ------------------------------------------------------------
int Qaullib_File_LL_HashSearch (char *filehash, struct qaul_file_LL_item **item)
{
	struct qaul_file_LL_node mynode;
	Qaullib_File_LL_InitNodeWithHash(&mynode, filehash);

	printf("Qaullib_File_LL_HashSearch\n");

	while(Qaullib_File_LL_NextItem(&mynode))
	{
		printf("item found: %s\n", mynode.item->hashstr);

		if(memcmp(&mynode.item->hash, filehash, MAX_HASH_LEN) == 0)
		{
			*item = mynode.item;
			return 1;
		}
	}

	return 0;
}

// ------------------------------------------------------------
int Qaullib_File_LL_HashExists (char *filehash)
{
	struct qaul_file_LL_node mynode;
	Qaullib_File_LL_InitNodeWithHash(&mynode, filehash);
	while(Qaullib_File_LL_NextItem(&mynode))
	{
		if(memcmp(&mynode.item->hash, filehash, MAX_HASH_LEN) == 0)
		{
			return 1;
		}
	}

	return 0;
}

// ------------------------------------------------------------
static uint32_t Qaullib_File_LL_Hashing(unsigned char *filehash)
{
	// FIXME: make that work!
/*
	uint32_t hash;
	hash = jenkins_hash(filehash, MAX_HASH_LEN);
	printf("Qaullib_File_LL_Hashing mask: %u u: %u\n", HASHMASK, hash & HASHMASK);
	return hash & HASHMASK;
*/

	/*
	uint32_t hash;

	memcpy(&hash, filehash, sizeof(uint32_t));
	printf("Qaullib_File_LL_Hashing mask: %u u: %u\n", HASHMASK, hash & HASHMASK);

	return hash & HASHMASK;
	*/

	return 1;
}

