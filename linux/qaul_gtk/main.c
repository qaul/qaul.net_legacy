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
	GtkWidget *window;
	
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
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 592);
    gtk_window_set_title(GTK_WINDOW(window), "qaul.net - قول");
    gtk_window_set_icon(GTK_WINDOW(window), create_pixbuf("app_icon.png"));

    // Create a browser instance
    WebKitWebView *webView = WEBKIT_WEB_VIEW(webkit_web_view_new());

    // Create a scrollable area, and put the browser instance into it
    GtkWidget *scrolledWindow = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledWindow),
            GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(scrolledWindow), GTK_WIDGET(webView));

    // Set up callbacks so that if either the main window or the browser instance is
    // closed, the program will exit
    g_signal_connect(window, "destroy", G_CALLBACK(destroyWindowCb), NULL);
    g_signal_connect(webView, "close-web-view", G_CALLBACK(closeWebViewCb), window);

    // Put the scrollable area into the main window
    gtk_container_add(GTK_CONTAINER(window), scrolledWindow);

    // Load a web page into the browser instance
    webkit_web_view_load_uri(webView, "http://127.0.0.1:8081/jqm_qaul.html");

    // Make sure that when the browser area becomes visible, it will get mouse
    // and keyboard events
    gtk_widget_grab_focus(GTK_WIDGET(webView));

    // Make sure the main window and all its contents are visible
    gtk_widget_show_all(window);

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
        //qaulConfigureCounter = 41;
        //QaulOlsrdStart();
        
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
        Qaullib_SetConfVoIP();
        Qaullib_UDP_StartServer();
        Qaullib_CaptiveStart();
        
        // TODO: configure firewall
        //QaulConfigureFirewall();
        
        qaulConfigureCounter = 50;
    }

    // start timers
    if(qaulConfigureCounter == 50)
    {
        printf("[configure] timers \n");
        
		// start timers
		qaulTimerEvents = g_timeout_add(10, qaul_timerSocket, NULL);
		qaulTimerSockets = g_timeout_add(100, qaul_timerSocket, NULL);
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

gboolean qaul_timerEvent(gpointer data)
{
    int myEvent = Qaullib_TimedCheckAppEvent();
/*
    if(myEvent > 0)
    {
        if(myEvent == QAUL_EVENT_CHOOSEFILE)
        {
            // open file selection
            QString fileName = QFileDialog::getOpenFileName(
                        this,
                        tr("Add File"),
                        "/home",
                        tr("Files (*.*)"));
            // file was selected
            if(fileName != NULL)
            {
                qDebug() << "file selected " << fileName;
                Qaullib_FilePicked(2, fileName.toLocal8Bit().data());
            }
            else
            {
                qDebug() << "file selection canceld ";
            }

        }
        else if(myEvent == QAUL_EVENT_OPENFILE)
        {
            // open file in default application
            QString filepath = "file://";
            filepath += Qaullib_GetAppEventOpenPath();
            qDebug() << filepath;
            QDesktopServices::openUrl(QUrl(filepath));
        }
        else if(myEvent == QAUL_EVENT_OPENURL)
        {
            // open URL in browser
            QString urlpath = Qaullib_GetAppEventOpenURL();
            qDebug() << urlpath;
            QDesktopServices::openUrl(QUrl(urlpath));
        }
        else if(myEvent == QAUL_EVENT_QUIT)
        {
            // quit application
            QApplication::quit();
        }
        else if(myEvent == QAUL_EVENT_NOTIFY && myEvent == QAUL_EVENT_RING)
        {
            // play beep
            // does not work under linux
            QApplication::beep();
        }
    }
*/
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
