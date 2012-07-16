/*
 * qaul.net is free software
 * licensed under GPL (version 3)
 */

#include "qaullib_private.h"
#include "../pjproject-2.0.1/pjsip/include/pjsua-lib/pjsua.h"

#define THIS_FILE	"qaullib_voip.c"

//#define SIP_DOMAIN	"192.168.6.140"
#define SIP_USER	"qaul"
#define SIP_PASSWD	"secret"


// currently active call id
static pjsua_call_id qaul_voip_callid;
static pjsua_acc_id qaul_voip_acc_id;
static pjsua_transport_id qaul_voip_trans_id;


/**
 *  Callback called by the library upon receiving incoming call
 */
static void on_incoming_call(pjsua_acc_id acc_id, pjsua_call_id call_id, pjsip_rx_data *rdata)
{
    pjsua_call_info ci;

    PJ_UNUSED_ARG(acc_id);
    PJ_UNUSED_ARG(rdata);

    pjsua_call_get_info(call_id, &ci);

    PJ_LOG(3,(THIS_FILE, "Incoming call from %.*s!!",
			 (int)ci.remote_info.slen,
			 ci.remote_info.ptr));

    // check if any calls are in progress
    if(pjsua_call_get_count() == 0)
    {
    	qaul_voip_callid = call_id;
    	// send ringing notice
    	pjsua_call_answer(call_id, 180, NULL, NULL);
    	// set qaul_voip_event
    	qaul_voip_event = 2;
    }
    else
    	// send user busy notice
    	pjsua_call_answer(call_id, 486, NULL, NULL);
}

/**
 * Callback called by the library when call's state has changed
 */
static void on_call_state(pjsua_call_id call_id, pjsip_event *e)
{
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

	// check if another call is in progress
	if(pjsua_call_get_count() == 0)
	{
		// create uri
		sprintf(stmt, "sip:%s@%s", SIP_USER, ip);
		pj_str_t uri = pj_str(stmt);

		status = pjsua_call_make_call(qaul_voip_acc_id, &uri, 0, NULL, NULL, &qaul_voip_callid);
		if (status != PJ_SUCCESS)
		{
			pjsua_perror(THIS_FILE, "Error making call", status);
			qaul_voip_event = 5;
		}
	}
}

void Qaullib_VoipCallAccept(void)
{
	pjsua_call_answer(qaul_voip_callid, 200, NULL, NULL);
}

void Qaullib_VoipCallEnd(void)
{
	pjsua_call_hangup_all();
}

int Qaullib_VoipStart(void)
{
    pj_status_t status;
	char buffer[256];
	char* stmt = buffer;

	qaul_voip_event = 0;
	qaul_voip_callid = 0;

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
