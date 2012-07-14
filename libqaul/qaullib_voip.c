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

    /* Automatically answer incoming calls with 200/OK */
    pjsua_call_answer(call_id, 200, NULL, NULL);
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

/**
 * Display error and exit application
 */
static void error_exit(const char *title, pj_status_t status)
{
    pjsua_perror(THIS_FILE, title, status);
    //pjsua_destroy();
    //exit(1);
}

int Qaullib_VoipStart()
{
    pjsua_acc_id acc_id;
    pj_status_t status;
	char buffer[256];
	char* stmt = buffer;

    // Create pjsua first!
    status = pjsua_create();
    if (status != PJ_SUCCESS)
    {
    	pjsua_perror(THIS_FILE, "Error in pjsua_create()", status);
    	return 0;
    }

    // check url
    //status = pjsua_verify_url(argv[1]);

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
		status = pjsua_transport_create(PJSIP_TRANSPORT_UDP, &cfg, NULL);
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

    // Register to SIP server by creating SIP account.
    // Todo: find out how to start it successfully without registering
    {
		pjsua_acc_config cfg;

		pjsua_acc_config_default(&cfg);
		sprintf(stmt,"sit:%s@%s",SIP_USER,qaul_ip_str);
		//cfg.id = pj_str("sip:" SIP_USER "@" +qaul_ip_str);
		cfg.id = pj_str(stmt);
		sprintf(stmt, "sip:%s", qaul_ip_str);
		//cfg.reg_uri = pj_str("sip:" qaul_ip_str);
		cfg.reg_uri = pj_str(stmt);
		cfg.cred_count = 1;
		cfg.cred_info[0].realm = pj_str(qaul_ip_str);
		cfg.cred_info[0].scheme = pj_str("digest");
		cfg.cred_info[0].username = pj_str(SIP_USER);
		cfg.cred_info[0].data_type = PJSIP_CRED_DATA_PLAIN_PASSWD;
		cfg.cred_info[0].data = pj_str(SIP_PASSWD);

		status = pjsua_acc_add(&cfg, PJ_TRUE, &acc_id);
		if (status != PJ_SUCCESS)
		{
            printf("server register error: %i", status);
		}
	}

/*
	// If URL is specified, make call to the URL.
	if (argc > 1)
	{
		pj_str_t uri = pj_str(argv[1]);
		status = pjsua_call_make_call(acc_id, &uri, 0, NULL, NULL, NULL);
		if (status != PJ_SUCCESS)
			error_exit("Error making call", status);
    }


    // Wait until user press "q" to quit.
    for (;;)
    {
		char option[10];

		puts("Press 'h' to hangup all calls, 'q' to quit");
		if (fgets(option, sizeof(option), stdin) == NULL)
		{
			puts("EOF while reading stdin, will quit now..");
			break;
		}

		if (option[0] == 'q')
			break;

		if (option[0] == 'h')
			pjsua_call_hangup_all();
    }
*/
    return 1;
}

int Qaullib_VoipStop(void)
{
	// Destroy pjsua
	pjsua_destroy();
}
