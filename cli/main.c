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

#include "../libqaul/qaullib.h"

// ------------------------------------------------------------
static int exit_flag;

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
	if(!Qaullib_WebserverStart()) printf("Webserver startup failed\n");
	if(!Qaullib_IpcConnect()) printf("Ipc connection failed\n");
	// TODO: start captive portal
	Qaullib_ConfigurationFinished();

	// test config
	printf("username: %s\n",Qaullib_GetUsername());
	printf("IP: %s\n",Qaullib_GetIP());
	printf("Qaul started\n");

	// loop variables
	int socketCounter = 0;
	int ipcCounter = 0;

	// main loop
	while (exit_flag == 0) {
		usleep(10000);
		int event = Qaullib_TimedCheckAppEvent();
		if(event == 100)
			printf("open file chooser\n");
		else if(event == 101)
			printf("open file\n");

		if(socketCounter >= 10)
		{
			Qaullib_TimedSocketReceive();
			socketCounter = 0;
		}
		else
			socketCounter++;

		if(ipcCounter >= 1500)
		{
			Qaullib_IpcSendCom(1);
			Qaullib_TimedDownload();
		}
		else
			ipcCounter++;

		if(getchar()) exit_flag = 1;
	}
	printf("Exiting on signal %d, waiting for all threads to finish...",exit_flag);

	// shutdown olsrd
	printf("send Qaullib_Exit()\n");
	Qaullib_Exit();

	return EXIT_SUCCESS;
}
