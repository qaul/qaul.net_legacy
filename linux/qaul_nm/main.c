/*
 * qaul.net is free software
 * licensed under GPL (version 3)
 */

#include <gtk/gtk.h>
#include <webkit/webkit.h>
#include <dbus/dbus-glib.h>

// ------------------------------------------------------------

static void destroyWindowCb(GtkWidget* widget, GtkWidget* window);
GdkPixbuf *create_pixbuf(const gchar * filename);

// ------------------------------------------------------------

int main(int argc, char *argv[])
{
	char cCurrentPath[FILENAME_MAX];
	GtkWidget *window;
    DBusGConnection *connection;
    GError *error;
    DBusGProxy *proxy;
    char **name_list;
    char **name_list_ptr;
	
	if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
	{
		printf ("ERROR: couldn't get directory\n");
		return EXIT_FAILURE;
	}
	cCurrentPath[sizeof(cCurrentPath) - 1] = '\0';
	printf ("The current working directory is %s\n", cCurrentPath);

	// open window
	gtk_init(&argc,&argv);

    // Create a window that will contain the browser instance
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 592);
    gtk_window_set_title(GTK_WINDOW(window), "qaul.net - قول");
    gtk_window_set_icon(GTK_WINDOW(window), create_pixbuf("app_icon.png"));

    // Set up callbacks so that if either the main window or the browser instance is
    // closed, the program will exit
    g_signal_connect(window, "destroy", G_CALLBACK(destroyWindowCb), NULL);
    g_signal_connect(webView, "close-web-view", G_CALLBACK(closeWebViewCb), window);

    // Put the scrollable area into the main window
    gtk_container_add(GTK_CONTAINER(window), scrolledWindow);

    // Load a web page into the browser instance
    webkit_web_view_load_uri(webView, "http://127.0.0.1:8081/jqm_qaul.html");

    // Make sure the main window and all its contents are visible
    gtk_widget_show_all(window);

    // ------------------------------------------------------------
    // ------------------------------------------------------------
    // dbus sample program with proxies
    // http://dbus.freedesktop.org/doc/dbus-tutorial.html#sample-program-1
    
    //Proxy proxy = new Proxy(getBusConnection(), "/remote/object/path");
    //      Object returnValue = proxy.MethodName(arg1, arg2);
    
    error = NULL;
    connection = dbus_g_bus_get(DBUS_BUS_SESSION, &error);
    if(connection == NULL)
    {
        g_printerr ("Failed to open connection to bus: %s\n",
                  error->message);
        g_error_free (error);
    }
    else
    {
        // Create a proxy object for the "bus driver" (name "org.freedesktop.DBus")
        proxy = dbus_g_proxy_new_for_name (connection,
                                         DBUS_SERVICE_DBUS,
                                         DBUS_PATH_DBUS,
                                         DBUS_INTERFACE_DBUS);

        // Call ListNames method, wait for reply
        error = NULL;
        if(!dbus_g_proxy_call (proxy, "ListNames", &error, G_TYPE_INVALID,
                              G_TYPE_STRV, &name_list, G_TYPE_INVALID))
        {
            // Just do demonstrate remote exceptions versus regular GError
            if (error->domain == DBUS_GERROR && error->code == DBUS_GERROR_REMOTE_EXCEPTION)
                g_printerr ("Caught remote method exception %s: %s",
                        dbus_g_error_get_name (error),
                        error->message);
            else
                g_printerr ("Error: %s\n", error->message);

            g_error_free (error);
        }
        else
        {
            // Print the results
            g_print ("Names on the message bus:\n");

            for(name_list_ptr = name_list; *name_list_ptr; name_list_ptr++)
            {
              g_print("  %s\n", *name_list_ptr);
            }
        }

        // release connection    
        g_strfreev(name_list);
        g_object_unref(proxy);
    }

    // ------------------------------------------------------------
    // ------------------------------------------------------------

    // Run the main GTK+ event loop
    gtk_main();

    return 0;
}

static void destroyWindowCb(GtkWidget* widget, GtkWidget* window)
{
    gtk_main_quit();
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

