/*
 * qaul.net is free software
 * licensed under GPL (version 3)
 */

#include "qaullib_private.h"
#include "../pjproject-2.0.1/pjsip/include/pjsua-lib/pjsua.h"

#define THIS_FILE	"qaullib_voip.c"
#define SIP_USER	"qaul"


// currently active call id
static pjsua_call_id qaul_voip_callid;
static pjsua_acc_id qaul_voip_acc_id;
static pjsua_transport_id qaul_voip_trans_id;

// thread registration list
static pj_status_t rc;

/**
 * thread linked list declarations
 */
struct qaul_voip_thread_LL_item {
	struct qaul_voip_thread_LL_item *next;	// next node
	struct qaul_voip_thread_LL_item *prev;  // previous node
	pj_thread_desc 		desc;
	pj_thread_t 		*this_thread;
};
static struct qaul_voip_thread_LL_item qaul_voip_threadlist;
int qaul_voip_LL_count;
struct qaul_voip_thread_LL_item* Qaullib_Voip_LL_Add();
// todo: clean references when a call ended
void Qaullib_Voip_LL_Delete_Item(struct qaul_voip_thread_LL_item *item);
void Qaullib_Voip_LL_Clean (void);


/**
 * register this thread in pjsip
 */
static void Qaullib_VoipRegisterThread(void)
{
	// get new reference
	struct qaul_voip_thread_LL_item* item = Qaullib_Voip_LL_Add();
	// register
	pj_bzero(item->desc, sizeof(item->desc));
	rc = pj_thread_register("thread", item->desc, &item->this_thread);
	if (rc != PJ_SUCCESS) {
		pjsua_perror(THIS_FILE, "...error in pj_thread_register", rc);
	}
}

/**
 *  Callback called by the library upon receiving incoming call
 */
static void on_incoming_call(pjsua_acc_id acc_id, pjsua_call_id call_id, pjsip_rx_data *rdata)
{
    pjsua_call_info ci;

    PJ_UNUSED_ARG(acc_id);
    PJ_UNUSED_ARG(rdata);

    pjsua_call_get_info(call_id, &ci);

    printf("on_incoming_call: acc_id %i, call_id %i, qaul_voip_callid %i\n", (int)acc_id, (int)call_id, (int)qaul_voip_callid);

    PJ_LOG(3,(THIS_FILE, "Incoming call from %.*s!!",
			 (int)ci.remote_info.slen,
			 ci.remote_info.ptr));

    // check if any calls are in progress
    if(pjsua_call_get_count() == 0 && qaul_voip_callid == call_id)
    {
    	PJ_LOG(3,(THIS_FILE, "Call %d state=%.*s", call_id, "back invite message received"));

    	// this is the invite answer for our call
    	// we will answer with an accept message
    	pjsua_call_answer(call_id, 200, NULL, NULL);
    }
    else if(pjsua_call_get_count() == 0)
    {
    	printf("on_incoming_call if(pjsua_call_get_count() == 0) answer with 180\n");

    	qaul_voip_callid = call_id;
    	// send ringing notice
    	pjsua_call_answer(call_id, 180, NULL, NULL);
    	// set qaul_voip_event
    	qaul_voip_event = 2;
    	qaul_voip_new_call = 1;
    	// get caller name
    	if(ci.local_contact.slen <= MAX_USER_LEN)
    	{
    		strncpy(qaul_voip_caller_name, ci.local_contact.ptr, (int)ci.local_contact.slen);
    		qaul_voip_caller_name[(int)ci.local_contact.slen] = '\0';
    	}
    	else
    	{
    		strncpy(qaul_voip_caller_name, ci.local_contact.ptr, MAX_USER_LEN);
    		qaul_voip_caller_name[MAX_USER_LEN] = '\0';
    	}
    }
    else
    {
    	printf("on_incoming_call - else - pjsua_call_get_count() %i\n", (int)pjsua_call_get_count());

    	// send user busy notice
    	pjsua_call_answer(call_id, 486, NULL, NULL);
    }
}

/**
 * Callback called by the library when call's state has changed
 */
static void on_call_state(pjsua_call_id call_id, pjsip_event *e)
{
	printf("on_call_state\n");

	pjsua_call_info ci;

    PJ_UNUSED_ARG(e);

    pjsua_call_get_info(call_id, &ci);
    PJ_LOG(3,(THIS_FILE, "Call %d state=%.*s", call_id,
			 (int)ci.state_text.slen,
			 ci.state_text.ptr));
}

/**
 * Callback called by the library when call's media state has changed
 */
static void on_call_media_state(pjsua_call_id call_id)
{
	printf("on_call_media_state\n");

	pjsua_call_info ci;

    pjsua_call_get_info(call_id, &ci);

    if (ci.media_status == PJSUA_CALL_MEDIA_ACTIVE)
    {
		// When media is active, connect call to sound device.
		pjsua_conf_connect(ci.conf_slot, 0);
		pjsua_conf_connect(0, ci.conf_slot);
    }
}

void Qaullib_VoipCallStart(char* ip)
{
	pj_status_t status;
	char buffer[256];
	char* stmt = buffer;

	// register this thread
	Qaullib_VoipRegisterThread();

	// check if another call is in progress
	if(pjsua_call_get_count() == 0)
	{
		// create uri
		sprintf(stmt, "sip:%s@%s:%i", SIP_USER, ip, VOIP_PORT);
		pj_str_t uri = pj_str(stmt);

		status = pjsua_call_make_call(qaul_voip_acc_id, &uri, 0, NULL, NULL, &qaul_voip_callid);
		if (status != PJ_SUCCESS)
		{
			pjsua_perror(THIS_FILE, "Error making call", status);
			qaul_voip_event = 5;
		}

		printf("Qaullib_VoipCallStart qaul_voip_callid %i\n", (int)qaul_voip_callid);
	}
}

void Qaullib_VoipCallAccept(void)
{
	Qaullib_VoipRegisterThread();
	pjsua_call_answer(qaul_voip_callid, 200, NULL, NULL);
}

void Qaullib_VoipCallEnd(void)
{
	Qaullib_VoipRegisterThread();
	pjsua_call_hangup_all();
}

int Qaullib_VoipStart(void)
{
    pj_status_t status;
	char buffer[256];
	char* stmt = buffer;

	qaul_voip_event = 0;
	qaul_voip_event_code = 400;
	qaul_voip_callid = 0;
	qaul_voip_new_call = 0;

	// init thread list
	qaul_voip_LL_count = 0;
	qaul_voip_threadlist.next = &qaul_voip_threadlist;
	qaul_voip_threadlist.prev = &qaul_voip_threadlist;

    // Create pjsua first!
    status = pjsua_create();
    if (status != PJ_SUCCESS)
    {
    	pjsua_perror(THIS_FILE, "Error in pjsua_create()", status);
    	return 0;
    }

    // Init pjsua
    {
		pjsua_config cfg;
		pjsua_logging_config log_cfg;

		pjsua_config_default(&cfg);
		cfg.cb.on_incoming_call = &on_incoming_call;
		cfg.cb.on_call_media_state = &on_call_media_state;
		cfg.cb.on_call_state = &on_call_state;

		pjsua_logging_config_default(&log_cfg);
		log_cfg.console_level = 4;

		status = pjsua_init(&cfg, &log_cfg, NULL);
		if (status != PJ_SUCCESS)
		{
			pjsua_perror(THIS_FILE, "Error in pjsua_init()", status);
			return 0;
		}
    }

    // Add UDP transport.
    {
		pjsua_transport_config cfg;

		pjsua_transport_config_default(&cfg);
		cfg.port = VOIP_PORT;
		status = pjsua_transport_create(PJSIP_TRANSPORT_UDP, &cfg, &qaul_voip_trans_id);
		if (status != PJ_SUCCESS)
		{
			pjsua_perror(THIS_FILE, "Error creating transport", status);
			return 0;
		}
    }

    // Initialization is done, now start pjsua
    status = pjsua_start();
    if (status != PJ_SUCCESS)
    {
    	pjsua_perror(THIS_FILE, "Error starting pjsua", status);
    	return 0;
    }

	// create local account
	status = pjsua_acc_add_local (qaul_voip_trans_id, PJ_TRUE, &qaul_voip_acc_id);
	if (status != PJ_SUCCESS)
		pjsua_perror(THIS_FILE, "account creation error", status);

    return 1;
}

int Qaullib_VoipStop(void)
{
	// Destroy pjsua
	pjsua_destroy();
}


/********************************************//**
 * thread list functions
 ***********************************************/

struct qaul_voip_thread_LL_item* Qaullib_Voip_LL_Add ()
{
	struct qaul_voip_thread_LL_item *new_item;
	new_item = (struct qaul_voip_thread_LL_item *)malloc(sizeof(struct qaul_voip_thread_LL_item));

	// create links
	new_item->prev = &qaul_voip_threadlist;
	new_item->next = qaul_voip_threadlist.next;
	qaul_voip_threadlist.next = new_item;
	new_item->next->prev = new_item;

	qaul_voip_LL_count++;
	return new_item;
}

// todo: clean references when a call ended
void Qaullib_Voip_LL_Delete_Item (struct qaul_voip_thread_LL_item *item)
{
	item->prev->next = item->next;
	item->next->prev = item->prev;

	qaul_voip_LL_count--;
	free(item);
}

void Qaullib_Voip_LL_Clean (void)
{
	// clean everything up to 5 references
	while(qaul_voip_LL_count > 5)
	{
		Qaullib_Voip_LL_Delete_Item(qaul_voip_threadlist.prev);
	}
}
