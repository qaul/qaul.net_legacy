/*
 * qaul.net is free software
 * licensed under GPL (version 3)
 */

#import "AppDelegate.h"
#import "ViewController.h"
#import "../../libqaul/qaullib.h"

@implementation AppDelegate


- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
	// copy files at first startup
	[self CopyFilesAtFirstStartup];
	
	// initialize app
	NSLog(@"initialize app");
	Qaullib_Init([qaulResourcePath UTF8String]);
	
	// set configuration
	Qaullib_SetConf(QAUL_CONF_QUIT);
	Qaullib_SetConf(QAUL_CONF_IOS);
	
	// start web server
	NSLog(@"start web server");
	if(Qaullib_WebserverStart())
		NSLog(@"successful web server start");
	else
		NSLog(@"web server start up error");
   
    
    // start configuration
    [self QaulSetGlobals];
    [self QaulConfigure];
    
    
    // open window
    self.window = [[[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]] autorelease];
    // Override point for customization after application launch.
    self.viewController = [[[ViewController alloc] initWithNibName:@"ViewController" bundle:nil] autorelease];
    self.window.rootViewController = self.viewController;
    [self.window makeKeyAndVisible];
    
    return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
    // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
    // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
    // Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
}


#pragma mark -
#pragma mark Qaul Configuration

- (void) CopyFilesAtFirstStartup
{
	NSFileManager *filemgr;
	NSError *dError;
		
    // check if files have been copied
    filemgr =[NSFileManager defaultManager];
    
    qaulResourcePath = [NSString stringWithFormat:@"%@/qaul.net", [NSSearchPathForDirectoriesInDomains(NSLibraryDirectory, NSUserDomainMask, YES) objectAtIndex:0]];
    
    qaulDocumentPath = [NSString stringWithFormat:@"%@/qaul.net", [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0]];

    NSLog(@"CopyFilesAtFirstStartup");
    NSLog(@"qaulResourcePath: %@, qaulDocumentPath: %@", qaulResourcePath, qaulDocumentPath);

    

	// check if data base exists
	if(![filemgr fileExistsAtPath:[NSString stringWithFormat:@"%@/qaullib.db", qaulResourcePath]])
	{
        NSLog(@"First startup: copy files to document directory");
 
        // create resource folder
        if (![filemgr createDirectoryAtPath:qaulResourcePath withIntermediateDirectories:NO attributes:nil error:&dError])
            NSLog(@"Create qaulResourcePath directory error: %@", dError);
 
        // create document path for downloaded files
        if (![filemgr createDirectoryAtPath:qaulResourcePath withIntermediateDirectories:NO attributes:nil error:&dError])
            NSLog(@"Create documents directory error: %@", dError);
 	
		if(![filemgr copyItemAtPath:[NSString stringWithFormat:@"/Applications/qaul.net.app/www"] toPath:[NSString stringWithFormat:@"%@/www", qaulResourcePath] error:&dError])
			NSLog(@"Error: %@", dError);
		else
			NSLog(@"Sucessfully copied");
	}
	else
		NSLog(@"Not first startup");
	
	[filemgr release];
}

- (void) CreateDirectory:(NSString *)directoryName atFilePath:(NSString *)filePath
{
    NSString *filePathAndDirectory = [filePath stringByAppendingPathComponent:directoryName];
    NSError *error;
    
    if (![[NSFileManager defaultManager] createDirectoryAtPath:filePathAndDirectory
                                   withIntermediateDirectories:NO
                                                    attributes:nil
                                                         error:&error])
    {
        NSLog(@"Create directory error: %@", error);
    }
}



- (void) QaulSetGlobals
{
	NSLog(@"QaulSetGlobals");
	qaulStarted = 0;
}

- (void) QaulConfigure
{
	NSLog(@"configure app");
	
	// init and start web server
	if(qaulStarted == 0)
	{
		// already initialized in didFinishLaunchingWithOptions
		Qaullib_ConfigStart();
		
		qaulStarted = 10;
	}
    
	
	// check authorization
	if(qaulStarted == 10)
	{
        // nothing to do here for the moment
        
		qaulStarted = 20;
	}
	
	
	// wait for wifi configuration
	if(qaulStarted == 20)
	{
/*
		if(Qaullib_CheckConf(QAUL_CHECK_WIFI_SET))
		{
			NSLog(@"wifi configured");
			qaulStarted = 25;
		}
		else
		{
			NSLog(@"wait for wifi configuration");
			qaulStarted = 19;
			[self startDelay: 1.0f];
		}
 */
        qaulStarted = 25;
	}
	
	// set static ip
	if(qaulStarted == 25)
	{
		// set static ip as root
		NSString *myip = [NSString stringWithFormat:@"/usr/bin/qaulhelper setip %s", Qaullib_GetIP()];
		system([myip UTF8String]);
        NSLog(@"* * * ip configured * * *");
        
		qaulStarted = 40;
	}
	
    
	// start olsrd
	if(qaulStarted == 40)
	{
		NSLog(@"* * * start olsrd * * *");
		system("/usr/bin/qaulhelper startolsrd");
		NSLog(@"* * * olsrd started * * *");
		
		qaulStarted = 44;
	}
	
	if(qaulStarted == 44)
		[self startDelay: 2.0f];    
	
	// connect ipc
	if(qaulStarted == 45)
	{
		Qaullib_IpcConnect();
		
		qaulStarted = 46;
	}
	
	// connect captive
	if(qaulStarted == 46)
	{
		// start VoIP
		Qaullib_SetConfVoIP();
		
		// start
		Qaullib_UDP_StartServer();
		
		// start captive portal
		Qaullib_CaptiveStart();
		
		// TODO: start port forwarding
		//success = [mysudo startPortForwarding:authorizationRef interface:qaulWifiInterface];
		
		// TODO: configure firewall
		
		qaulStarted = 50;
	}
	
	// finished
	if(qaulStarted == 50)
	{
		// start timer
		[self startTimer];
		
		// set configuration to finished
		Qaullib_ConfigurationFinished();
		
		qaulStarted = 60;
	}
}



// -------------------------------------------------------------------------
// Timer
// -------------------------------------------------------------------------
- (void)startTimer
{
	[qaullibTimer invalidate];
    qaullibTimer = [NSTimer scheduledTimerWithTimeInterval: 0.1 target:self selector:
					@selector(checkIpcMessage:) userInfo:nil repeats:YES];
    [qaullibTimer fire];
	
	[qaullibTimer2 invalidate];
    qaullibTimer2 = [NSTimer scheduledTimerWithTimeInterval: 3.0 target:self selector:
					 @selector(checkIpcTopology:) userInfo:nil repeats:YES];
    [qaullibTimer2 fire];
	
	[qaullibTimer3 invalidate];
    qaullibTimer3 = [NSTimer scheduledTimerWithTimeInterval: 0.01 target:self selector:
					 @selector(checkAppEvents:) userInfo:nil repeats:YES];
    [qaullibTimer3 fire];
}

- (void)stopTimer
{
	[qaullibTimer invalidate];
    qaullibTimer = nil;
	
	[qaullibTimer2 invalidate];
    qaullibTimer2 = nil;
	
	[qaullibTimer3 invalidate];
    qaullibTimer3 = nil;
}

- (void)checkIpcMessage:(NSTimer *)theTimer
{
	Qaullib_TimedSocketReceive();
}

- (void)checkIpcTopology:(NSTimer *)theTimer
{
	Qaullib_IpcSendCom(1);
	Qaullib_TimedDownload();
}

- (void)checkAppEvents:(NSTimer *)theTimer
{
	int appEvent = Qaullib_TimedCheckAppEvent();
    
    if (appEvent > 0)
    {
        NSLog([NSString stringWithFormat:@"appEvent found: %i\n", appEvent]);

        if(appEvent == QAUL_EVENT_CHOOSEFILE)
        {
/*
            // show file picker
            int i; // Loop counter.

            // Create the File Open Dialog class.
            NSOpenPanel* openDlg = [NSOpenPanel openPanel];

            // Enable the selection of files in the dialog.
            [openDlg setCanChooseFiles:YES];

            // Disable the selection of multiple files in the dialog.
            [openDlg setAllowsMultipleSelection:NO];

            // Enable the selection of directories in the dialog.
            //[openDlg setCanChooseDirectories:YES];

            // Display the dialog.  If the OK button was pressed,
            // process the files.
            if ( [openDlg runModalForDirectory:nil file:nil] == NSOKButton )
            {
                // Get an array containing the full filenames of all
                // files and directories selected.
                NSArray* files = [openDlg filenames];

                // Loop through all the files and process them.
                for( i = 0; i < [files count]; i++ )
                {
                    NSString* fileName = [files objectAtIndex:i];

                    // Do something with the filename.
                    NSLog(@"filename: %@\n", fileName);
                    Qaullib_FilePicked(2, [fileName UTF8String]);
                }
            }
 */
        }
        else if(appEvent == QAUL_EVENT_OPENFILE)
        {
/*
            // open file
            [[NSWorkspace sharedWorkspace] openFile:[NSString stringWithFormat:@"%s", Qaullib_GetAppEventOpenPath()]];
 */
        }
        else if(appEvent == QAUL_EVENT_OPENURL)
        {
            // open url
            if (![[UIApplication sharedApplication] openURL:[NSURL URLWithString:[NSString stringWithFormat:@"%s", Qaullib_GetAppEventOpenURL()]]])
                NSLog(@"%@%@",@"Failed to open url:",[NSString stringWithFormat:@"%s", Qaullib_GetAppEventOpenURL()]);
        }
        else if(appEvent == QAUL_EVENT_QUIT)
        {
            NSLog(@"QAUL_EVENT_QUIT\n");
            system("/usr/bin/qaulhelper stopolsrd");            
            exit(0);
        }
        else if(appEvent == QAUL_EVENT_NOTIFY || appEvent == QAUL_EVENT_RING)
        {
/*
            NSLog(@"QAUL_EVENT_NOTIFY or QAUL_EVENT_RING received\n");
            NSBeep();
 */
        }
    }
}


// -------------------------------------------------------------------------
// Wait Timer
// -------------------------------------------------------------------------
- (void)startDelay:(NSTimeInterval)secs
{
	NSLog(@"start Delay %f s\n",secs);
	[self performSelector:@selector(fireDelay) withObject:nil afterDelay:secs];
}

- (void)fireDelay
{
	NSLog(@"Delay fired\n");
	qaulStarted++;
	[self QaulConfigure];
}


#pragma mark -
#pragma mark Memory management

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application {
    //Free up as much memory as possible by purging cached data objects that can be recreated (or reloaded from disk) later.
}

- (void)dealloc
{
    [_window release];
    [_viewController release];
    [super dealloc];
}

 
@end
