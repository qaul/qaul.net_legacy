/*
 * qaul.net is free software
 * licensed under GPL (version 3)
 */

#ifdef WIN32
#include <windows.h>
#else
#endif

#include <stdio.h> // defines FILENAME_MAX
#include <stdlib.h>
#ifdef WINDOWS
    #include <direct.h>
    #define GetCurrentDir _getcwd
#else
    #include <unistd.h>
    #define GetCurrentDir getcwd
 #endif

#include "../../libqaul/qaullib.h"
#include "../../libqaul/qaullib_private.h"
#include "config.h"

// ------------------------------------------------------------
int main(int argc, char *argv[])
{
	char cCurrentPath[FILENAME_MAX];

	if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
	{
		printf ("ERROR: couldn't get directory\n");
		return EXIT_FAILURE;
	}
	cCurrentPath[sizeof(cCurrentPath) - 1] = '\0';
	printf ("The current working directory is %s\n", cCurrentPath);

	Qaullib_Init(cCurrentPath);
	// enable debug menu
	qaul_conf_debug = 1;

	if(!Qaullib_WebserverStart())
		printf("Webserver startup failed\n");
	Qaullib_ConfigStart();

	printf("----------------------------------------------------\n");
	printf(" config started\n");
	printf("----------------------------------------------------\n");
	
	// check if IP has been set in DB
	if (!Qaullib_DbGetConfigValue("ip", qaul_ip_str))
	{
		// set IP
		Qaullib_SetIP(QaulServerIP);
	}
	// load the IP otherwise
	printf("IP: %s\n", Qaullib_GetIP());

	// set user name
	int username_flag = 0;
	if(Qaullib_ExistsUsername() == 0)
	{
		Qaullib_SetUsername(QaulServerUsername);
		printf("user name set!\n");
	}

	if(!Qaullib_IpcConnect())
		printf("Ipc connection failed\n");
	Qaullib_SetConfVoIP();
	if(!Qaullib_UDP_StartServer())
		printf("UDP server failed\n");
	if(!Qaullib_CaptiveStart())
		printf("Captive portal failed\n");
	Qaullib_ConfigurationFinished();

	// test config
	printf("IP: %s\n", Qaullib_GetIP());
	printf("Qaul started\n");

	// loop variables
	int socketCounter = 0;
	int ipcCounter = 0;

	printf("kill app to exit!\n");

	// main loop
	while (1) {
		usleep(10000);

		// get event
		int event = Qaullib_TimedCheckAppEvent();
		if(event == QAUL_EVENT_QUIT)
			printf("quit app\n");
		else if(event == QAUL_EVENT_CHOOSEFILE)
			printf("open file chooser\n");
		else if(event == QAUL_EVENT_OPENFILE)
			printf("open file\n");

		// check sockets
		if(socketCounter >= 10)
		{
			Qaullib_TimedSocketReceive();
			socketCounter = 0;
		}
		else
			socketCounter++;

		// get network node IPs
		// schedule downloads
		if(ipcCounter >= 500)
		{
			Qaullib_IpcSendCom(1);
			Qaullib_TimedDownload();
			ipcCounter = 0;
		}
		else
			ipcCounter++;
	}
}
