/*
 * qaul.net is free software
 * licensed under GPL (version 3)
 */

#ifndef _QAULLIB_WEBSERVER
#define _QAULLIB_WEBSERVER

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


/**
 * call back function for web server
 */
void *Qaullib_WwwEvent_handler(enum mg_event event, struct mg_connection *conn, const struct mg_request_info *request_info);


#ifdef __cplusplus
}
#endif // __cplusplus

#endif
