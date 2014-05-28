/*
 * qaul.net is free software
 * licensed under GPL (version 3)
 */

#define GetCurrentDir getcwd

#include <stdio.h> // defines FILENAME_MAX
#include <stdlib.h>
#include <unistd.h>
#include "../../libqaul/qaullib.h"
#include "../../libqaul/qaullib_private.h"

#include <gtk/gtk.h>
#include <webkit/webkit.h>

// ------------------------------------------------------------

/// GUI
GtkWidget *qaulMainWindow;

static void destroyWindowCb(GtkWidget* widget, GtkWidget* window);
static gboolean closeWebViewCb(WebKitWebView* webView, GtkWidget* window);
GdkPixbuf *create_pixbuf(const gchar * filename);

/// runs on start up after opening the window
void qaul_onstartup(void);

/// executed before quitting the application
void qaul_onquit(void);

/// configure computer for qaul.net
int qaulConfigureCounter;
gint qaulConfigureTimer;
gboolean qaul_configure(gpointer data);

/// configuration tasks
void qaul_olsrdStart(void);
void qaul_olsrdStop(void);
void qaul_startPortForwarding(void);
void qaul_stopPortForwarding(void);
void qaul_configFindWifiInterface(void);
void qaul_configFindInterfaces(void);
void qaul_configWifi(void);
void qaul_configIP(void);

/// timers
gint qaulTimerEvents;
gint qaulTimerSockets;
gint qaulTimerTopology;
gboolean qaul_timerEvent(gpointer data);
gboolean qaul_timerSocket(gpointer data);
gboolean qaul_timerTopology(gpointer data);

// ------------------------------------------------------------

int main(int argc, char *argv[])
{
	char cCurrentPath[FILENAME_MAX];

	qaulConfigureCounter = 0;
	qaulTimerEvents = 0;
	qaulTimerSockets = 0;
	qaulTimerTopology = 0;


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

	// start configuration timer
	qaulConfigureTimer = g_timeout_add(500, qaul_configure, NULL);

	// open window
	gtk_init(&argc,&argv);

    // Create a window that will contain the browser instance
    qaulMainWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(qaulMainWindow), 400, 592);
    gtk_window_set_title(GTK_WINDOW(qaulMainWindow), "qaul.net - قول");
    gtk_window_set_icon(GTK_WINDOW(qaulMainWindow), create_pixbuf("app_icon.png"));

    // Create a browser instance
    WebKitWebView *webView = WEBKIT_WEB_VIEW(webkit_web_view_new());

    // Create a scrollable area, and put the browser instance into it
    GtkWidget *scrolledWindow = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledWindow),
            GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(scrolledWindow), GTK_WIDGET(webView));

    // Set up callbacks so that if either the main window or the browser instance is
    // closed, the program will exit
    g_signal_connect(qaulMainWindow, "destroy", G_CALLBACK(destroyWindowCb), NULL);
    g_signal_connect(webView, "close-web-view", G_CALLBACK(closeWebViewCb), qaulMainWindow);

    // Put the scrollable area into the main window
    gtk_container_add(GTK_CONTAINER(qaulMainWindow), scrolledWindow);

    // Load a web page into the browser instance
    webkit_web_view_load_uri(webView, "http://127.0.0.1:8081/jqm_qaul.html");

    // Make sure that when the browser area becomes visible, it will get mouse
    // and keyboard events
    gtk_widget_grab_focus(GTK_WIDGET(webView));

    // Make sure the main window and all its contents are visible
    gtk_widget_show_all(qaulMainWindow);

    // Run the main GTK+ event loop
    gtk_main();

    return 0;

/*
	printf("----------------------------------------------------\n");
	printf(" config started \n");
	printf("----------------------------------------------------\n");
	// The invoking of Qaullib_GetIP() is mandatory to load the IP.
	printf("IP: %s\n", Qaullib_GetIP());

	// wait until user name is set
	int username_flag = 0;
	while(Qaullib_ExistsUsername() == 0)
	{
		if(username_flag == 0)
		{
			username_flag = 1;
			printf("waiting until user name is set ...\n");
			printf("open web browser with http://localhost:8081/jqm_qaul.html to set it ...\n");
		}
		sleep(1);
	}
	printf("user name successfully set!\n");

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
*/
}

static void destroyWindowCb(GtkWidget* widget, GtkWidget* window)
{
    qaul_onquit();
    gtk_main_quit();
}

static gboolean closeWebViewCb(WebKitWebView* webView, GtkWidget* window)
{
    gtk_widget_destroy(window);
    return TRUE;
}

GdkPixbuf *create_pixbuf(const gchar * filename)
{
   GdkPixbuf *pixbuf;
   GError *error = NULL;
   pixbuf = gdk_pixbuf_new_from_file(filename, &error);
   if(!pixbuf) {
      fprintf(stderr, "%s\n", error->message);
      g_error_free(error);
   }

   return pixbuf;
}

void qaul_onquit(void)
{
	// stop configuration
	if(qaulConfigureCounter < 60)
		g_source_remove(qaulConfigureTimer);
	// stop timers
	if(qaulTimerEvents)
		g_source_remove(qaulTimerEvents);
	if(qaulTimerSockets)
		g_source_remove(qaulTimerSockets);
	if(qaulTimerTopology)
		g_source_remove(qaulTimerTopology);
}

gboolean qaul_configure(gpointer data)
{
    // init
    if(qaulConfigureCounter == 0)
    {
        // everything is fine
        Qaullib_ConfigStart();
        qaulConfigureCounter = 10;
    }

    // check autorization
    if(qaulConfigureCounter == 10)
    {
        qaulConfigureCounter = 20;
    }

    // configure network interface
    if(qaulConfigureCounter == 20)
    {
        // TODO: configure network interface

        //printf("[configure] search wifi interface \n");
        //if(QaulWifiGetInterface())
			//qaulConfigureCounter = 21;
        //else
        //{
			//printf("[configure] no wifi interface found \n");
            //// TODO: display error screen
        //}
        qaulConfigureCounter = 29;
    }

    //// configure interface
    //if(qaulConfigureCounter == 21)
    //{
        ////printf("[configure] configure interface \n");
        ////qaulConfigureCounter = 22;
        ////// open console with sudo
        ////printf("[configure] open console \n");
        ////qaulConfigProcess = new QProcess(this);
        ////connect(qaulConfigProcess, SIGNAL(started()), this, SLOT(QaulWifiConfigure()));
        ////connect(qaulConfigProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(QaulConfigureProcessRead()));
        ////qaulConfigProcess->start("pkexec /bin/bash");
        //qaulConfigureCounter = 29;
    //}

    // check if username is set
    if(qaulConfigureCounter == 30)
    {
        printf("[configure] check username \n");
        if(Qaullib_ExistsUsername())
			qaulConfigureCounter = 40;
        else
        {
            // wait
            qaulConfigureCounter--;
        }
    }

    // start olsrd
    if(qaulConfigureCounter == 40)
    {
        printf("[configure] start olsrd \n");

        // TODO: start olsrd
        qaul_olsrdStart();

        qaulConfigureCounter = 44;
    }

    // connect ipc
    if(qaulConfigureCounter == 45)
    {
        printf("[configure] connect ipc \n");
        Qaullib_IpcConnect();
        qaulConfigureCounter = 46;
    }

    // start captive portal
    if(qaulConfigureCounter == 46)
    {
    	printf("[configure] start captive portal \n");
    	Qaullib_SetConfVoIP();
        Qaullib_UDP_StartServer();
        Qaullib_CaptiveStart();

        // TODO: configure firewall
        //qaul_startPortForwarding();

        qaulConfigureCounter = 50;
    }

    // start timers
    if(qaulConfigureCounter == 50)
    {
        printf("[configure] timers \n");

		// start timers
		qaulTimerEvents = g_timeout_add(10, qaul_timerSocket, NULL);
		qaulTimerSockets = g_timeout_add(100, qaul_timerEvent, NULL);
		qaulTimerTopology = g_timeout_add(5000, qaul_timerTopology, NULL);

        Qaullib_ConfigurationFinished();

        qaulConfigureCounter = 60;
    }

    // end configuration
	if(qaulConfigureCounter == 60)
	{
		printf("[configure] finished \n");
		return FALSE;
	}

	qaulConfigureCounter++;
	return TRUE;
}


void qaul_olsrdStart(void)
{
	system("/usr/share/qaul/qaulhelper startolsrd no wlan0");
}

void qaul_olsrdStop(void)
{
	system("/usr/share/qaul/qaulhelper stopolsrd");
}

void qaul_startPortForwarding(void)
{

}

void qaul_stopPortForwarding(void)
{

}

void qaul_configFindWifiInterface(void)
{

}

void qaul_configFindInterfaces(void)
{

}

void qaul_configWifi(void)
{

}

void qaul_configIP(void)
{

}


gboolean qaul_timerEvent(gpointer data)
{
    int myEvent;
    gchar myFilePath[MAX_URL_LEN +8];
	GError *myError;
	GtkWidget *myDialog;
	char *myFileChoose;

    myEvent = Qaullib_TimedCheckAppEvent();

    if(myEvent > 0)
    {
		printf("qaul_timerEvent: event [%i] found \n", myEvent);

    	if(myEvent == QAUL_EVENT_CHOOSEFILE)
        {
			printf("QAUL_EVENT_CHOOSEFILE \n");
    		myDialog = gtk_file_chooser_dialog_new("Choose File",
								  GTK_WINDOW(qaulMainWindow),
								  GTK_FILE_CHOOSER_ACTION_OPEN,
								  GTK_STOCK_CANCEL,
								  GTK_RESPONSE_CANCEL,
								  GTK_STOCK_OPEN,
								  GTK_RESPONSE_ACCEPT,
								  NULL);
			if (gtk_dialog_run(GTK_DIALOG(myDialog)) == GTK_RESPONSE_ACCEPT)
			{
				myFileChoose = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(myDialog));
				Qaullib_FilePicked(2, myFileChoose);
				g_free(myFileChoose);
			}
			gtk_widget_destroy(myDialog);
        }
        else if(myEvent == QAUL_EVENT_OPENFILE)
        {
			printf("QAUL_EVENT_OPENFILE \n");

        	// open file in default program
            myError = NULL;
            g_snprintf(myFilePath, MAX_URL_LEN +8, "file://%s", Qaullib_GetAppEventOpenPath());
            if(gtk_show_uri(gdk_screen_get_default(), myFilePath, gtk_get_current_event_time(), &myError))
				printf("Open file %s\n", myFilePath);
			else
				printf("Error opening file %s\n", myFilePath);

        	if(myError != NULL)
        	{
        		g_printerr("Error QAUL_EVENT_OPENFILE: %s\n", myError->message);
        		g_error_free(myError);
        	}
        }
        else if(myEvent == QAUL_EVENT_OPENURL)
        {
			printf("QAUL_EVENT_OPENURL \n");

        	// open URL in default browser
        	myError = NULL;
//            if(gtk_show_uri(
//					gdk_screen_get_default(),
//					Qaullib_GetAppEventOpenURL(),
//					gtk_get_current_event_time(),
//					&myError
//				))
            if(gtk_show_uri(
					NULL,
					Qaullib_GetAppEventOpenURL(),
					GDK_CURRENT_TIME,
					&myError
				))
				printf("Open URL %s\n", Qaullib_GetAppEventOpenURL());
			else
				printf("Error opening URL %s\n", Qaullib_GetAppEventOpenURL());

        	if(myError != NULL)
        	{
        		g_printerr("Error QAUL_EVENT_OPENURL: %s\n", myError->message);
        		g_error_free(myError);
        	}
        }
        else if(myEvent == QAUL_EVENT_QUIT)
        {
			printf("QAUL_EVENT_QUIT \n");

            // quit application
			qaul_onquit();
			gtk_main_quit();
        }
        else if(myEvent == QAUL_EVENT_NOTIFY || myEvent == QAUL_EVENT_RING)
        {
			printf("QAUL_EVENT_NOTIFY || QAUL_EVENT_RING \n");

            // play beep
            gdk_beep();
        }
    }

	return TRUE;
}

gboolean qaul_timerSocket(gpointer data)
{
    Qaullib_TimedSocketReceive();
	return TRUE;
}

gboolean qaul_timerTopology(gpointer data)
{
    Qaullib_IpcSendCom(1);
    Qaullib_TimedDownload();
	return TRUE;
}
