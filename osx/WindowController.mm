#import "WindowController.h"
#import "qaullib.h"

#import <Foundation/Foundation.h> // Add Security.framework to the Xcode project
#import <CoreFoundation/CoreFoundation.h>
#import <SystemConfiguration/SystemConfiguration.h>

// ----------------------------------------------
// definitions
#ifndef NSAppKitVersionNumber10_4
#define NSAppKitVersionNumber10_4 824
#endif

#ifndef NSAppKitVersionNumber10_5
#define NSAppKitVersionNumber10_5 949
#endif

#ifndef NSAppKitVersionNumber10_6
#define NSAppKitVersionNumber10_6 1038
#endif
// ----------------------------------------------

@implementation WindowController
// -------------------------------------------------------------------------
// initialize
// -------------------------------------------------------------------------
- (id)init
{
	if(self)
	{
		qaulStarted = 0;
		//qaulResourcePath =(NSString *)[[NSBundle mainBundle] resourcePath];
		mysudo = [[QaulConfigWifi alloc] init];
		qaulWifiInterface = nil;
		qaulWifiInterfaceSet = FALSE;
		qaulServiceConfigured = FALSE;
	}
	return self;
}


// -------------------------------------------------------------------------
// window is opened
// -------------------------------------------------------------------------
- (void)awakeFromNib
{
	//status = AuthorizationCreate(NULL, kAuthorizationEmptyEnvironment, kAuthorizationFlagDefaults, &authorizationRef);
	//NSLog(@"awake from nib!");
	//[self init_app];
}

- (void)applicationDidFinishLaunching:(id)sender
{
	NSLog(@"applicationDidFinishLaunching");
	[self init_app];
}

// -------------------------------------------------------------------------
// quit application when window is closed
// -------------------------------------------------------------------------
- (BOOL)windowShouldClose:(id)sender 
{
	// quit application
	[NSApp terminate:nil];
	
	return TRUE;
	
/*
	// open alert message
	NSAlert *alert = [[[NSAlert alloc] init] autorelease];
	[alert addButtonWithTitle:@"OK"];
	[alert addButtonWithTitle:@"Cancel"];
	[alert setMessageText:@"Quit qaul.net?"];
	[alert setAlertStyle:NSInformationalAlertStyle];
	[alert 
					beginSheetModalForWindow:sender
					modalDelegate:self 
					didEndSelector:@selector(endAlert:returnCode:contextInfo:) 
					contextInfo:nil];	
	
	return FALSE;
*/
}

- (void)endAlert:(id)sheet returnCode:(int)returnCode contextInfo:(void *)contextInfo
{
	if (returnCode == NSAlertFirstButtonReturn)
	{
		// quit application
		[NSApp terminate:nil];
    }
}

- (BOOL) applicationShouldTerminate:(id)app
{
	NSLog(@"Application should Terminate Event");
	
	// TODO: stop timers
	
	// check for Authorization
	if([mysudo testAuthorization:authorizationRef]) 
		NSLog(@"Authorization tested success");
	else 
		NSLog(@"Authorization tested error");
	
	// stop library
	Qaullib_Exit();
	
	// stop olsrd 
	if(![mysudo stopOlsrd:authorizationRef]) 
		NSLog(@"olsrd not killed");
	// stop portforwarding 
	if(![mysudo stopPortForwarding:authorizationRef]) 
		NSLog(@"portforwarding not removed");
	
	usleep(50000);
	// stop wifi
	if(![mysudo stopAirport:authorizationRef interface:qaulWifiInterface]) 
		NSLog(@"airport not stopped");
	
	// change location
	[mysudo deleteNetworkProfile:authorizationRef];
	usleep(50000);
	
	return TRUE;
}

// -------------------------------------------------------------------------
// start library, wifi, routing
// -------------------------------------------------------------------------
- (void)init_app 
{
	NSLog(@"init_app");
	BOOL success;

	// check if it was initialized
	if(qaulStarted == 0)
	{
		[qaulTabView selectTabViewItemWithIdentifier:@"start"];		
		
		// init qaullib
		NSLog(@"initialize app");		
		qaulResourcePath = [[NSBundle mainBundle] resourcePath];
		Qaullib_Init([qaulResourcePath UTF8String]);
		
		// set Download path
		NSString *downloadPath = [NSHomeDirectory() stringByAppendingPathComponent:@"/Downloads"];
		NSLog(@"path to download folder: %@", downloadPath);		
		Qaullib_SetConfDownloadFolder([downloadPath UTF8String]);
		
		qaulStarted = 1;
	}
	
	// start webserver
	// set tab to web view
	if(qaulStarted == 1)
	{
		// start webserver
		Qaullib_WebserverStart();
		
		// change to chat view	
		NSString *myUrl = @"http://127.0.0.1:8081/jqm_qaul.html";
		[qaulWebView setMainFrameURL:myUrl];
		[qaulTabView selectTabViewItemWithIdentifier:@"chat"];
		
		// start user configuration
		Qaullib_ConfigStart();
		
		qaulStarted = 10;
	}

	// check authorization 
	if(qaulStarted == 10)
	{
		// Create authorization reference
		// authorization
		NSLog(@"authorize");
		
		const char *path0 = "/usr/bin/killall";
		const char *path1 = [[[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:@"olsrd_start.sh"] cString];
		const char *path2 = [[[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:@"olsrd_stop.sh"] cString];
		const char *path3 = [[[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:@"socat_start.sh"] cString];
		const char *path4 = "/usr/sbin/networksetup";
		const char *path5 = "/usr/sbin/ipconfig";		
		const char *path6 = "/sbin/dmesg";
		const char *path7 = "/sbin/ipfw";
		const char *path8 = "/System/Library/CoreServices/RemoteManagement/ARDAgent.app/Contents/Support/networksetup";
		
		AuthorizationItem authItem0		= { kAuthorizationRightExecute, 1, &path0, 0 };
		AuthorizationItem authItem1		= { kAuthorizationRightExecute, 1, &path1, 0 };
		AuthorizationItem authItem2		= { kAuthorizationRightExecute, 1, &path2, 0 };
		AuthorizationItem authItem3		= { kAuthorizationRightExecute, 1, &path3, 0 };
		AuthorizationItem authItem4		= { kAuthorizationRightExecute, 1, &path4, 0 };
		AuthorizationItem authItem5		= { kAuthorizationRightExecute, 1, &path5, 0 };
		AuthorizationItem authItem6		= { kAuthorizationRightExecute, 1, &path6, 0 };
		AuthorizationItem authItem7		= { kAuthorizationRightExecute, 1, &path7, 0 };
		AuthorizationItem authItem8		= { kAuthorizationRightExecute, 1, &path8, 0 };
		
		AuthorizationItem items[9];
		
		items[0] =  authItem0;
		items[1] =  authItem1;
		items[2] =  authItem2;
		items[3] =  authItem3;
		items[4] =  authItem4;
		items[5] =  authItem5;
		items[6] =  authItem6;
		items[7] =  authItem7;
		items[8] =  authItem8;
		
		AuthorizationRights authRights	= { 8, items};
		AuthorizationFlags flags = kAuthorizationFlagDefaults | kAuthorizationFlagInteractionAllowed | kAuthorizationFlagPreAuthorize | kAuthorizationFlagExtendRights;		
		
		status = AuthorizationCreate(&authRights, kAuthorizationEmptyEnvironment, flags, &authorizationRef);
		if (status == errAuthorizationSuccess) 
			NSLog(@"Authorization success");
		else 
			NSLog(@"Authorization failed");
		
		// use the authorization
		if([mysudo testAuthorization:authorizationRef]) 
			NSLog(@"Authorization tested success");
		else 
			NSLog(@"Authorization tested error");
		
		qaulStarted = 20;
	}

	// create new network profile
	// we need to create a new network profile that we can 
	// trash afterwards, because the routing table gets messed up
	// otherwise.
	if(qaulStarted == 20)
	{
		success = [mysudo createNetworkProfile:authorizationRef];		
		qaulStarted = 21;
	}
	
	// search Wifi interface
	if(qaulStarted == 21)
	{
		NSLog(@"loop through interfaces");
		// -----------------------------------
		// configure & start up wifi
		qaulInterfacesAll = (NSArray *) SCNetworkInterfaceCopyAll ();
		en = [qaulInterfacesAll objectEnumerator];
		SCNetworkInterfaceRef inter;
		SCPreferencesRef prefRef;
		prefRef = SCPreferencesCreate(kCFAllocatorSystemDefault, (CFStringRef)@"XXX", NULL);
		NSEnumerator *e = [(NSArray *)SCNetworkSetCopyServices(SCNetworkSetCopyCurrent(prefRef)) objectEnumerator];
		SCNetworkServiceRef service;

		// loop through interfaces
		while ((inter = (SCNetworkInterfaceRef) [en nextObject])) 
		{
			//CFStringRef name = SCNetworkInterfaceGetBSDName (inter);
			//NSLog(@"%@", name);
			
			// check if it is a Wifi
			CFStringRef typesMy = SCNetworkInterfaceGetInterfaceType(inter);
			if (typesMy == kSCNetworkInterfaceTypeIEEE80211)
			{
				//NSLog(@"Its a Wifi!");
				qaulWifiInterface = inter;
				qaulWifiInterfaceSet = TRUE;
				// TODO:
				// put connection into array
				//[qaulInterfacesWifi addObject:[qaulInterfacesAll objectAtIndex:i]];
				//[qaulInterfacesWifi addObject:inter];
			}
		}
		
		// loop through Services
		while (service = (SCNetworkServiceRef)[e nextObject]) 
		{
			inter = SCNetworkServiceGetInterface(service);
			
			// check if interface matches
			if(CFEqual(qaulWifiInterface, inter))
			{
				qaulServiceId = service;
				//CFStringRef name = SCNetworkInterfaceGetBSDName (inter);
				//NSLog(@"Wifi Service found!");
				//NSLog(@"%@", name);
				
				// enable it if disabled
				if(!SCNetworkServiceGetEnabled(qaulServiceId))
				{				
					NSLog(@"enable Service");
					if(!SCNetworkServiceSetEnabled(qaulServiceId, TRUE)) 
						NSLog(@"service couldn't be enabled");
					else qaulServiceConfigured = true;
				}
				else NSLog(@"SCNetworkServiceEnabled");
					
				// get Service Name
				qaulServiceName = SCNetworkServiceGetName(qaulServiceId);
				NSLog(@"service name: %@", qaulServiceName);
				qaulServiceFound = TRUE;
				break;
			}
		}
		
		//success = [mysudo startAirport:authorizationRef interface:qaulWifiInterface];
		//if(success) NSLog(@"startAirport success!!");
		//else NSLog(@"startAirport no success");
		
		//if(qaulServiceFound) ;
		//else 
		qaulStarted = 22;
	}
	
	// enable Service
	if(qaulStarted == 22)
	{
		
		// check if the Service is enabled
		if(!qaulServiceConfigured)
		{
			NSLog(@"Service not activated!");
			
			// TODO: create deleted Services
			// create it
			//SCNetworkSetAddService();
			//service = SCNetworkServiceCreate (prefRef, inter);
			// configure it
			//[service release];
			
		}
		
		qaulStarted = 23;
	}
	
	// switch airport on
	if(qaulStarted == 23)
	{
		NSLog(@"switch airport on");
		// switch on airport via cli 
		success = [mysudo startAirport:authorizationRef interface:qaulWifiInterface];
		if(success) 
			NSLog(@"startAirport success!!");
		else 
			NSLog(@"startAirport no success");
		
		// configure Address taken away from this place...
		
		qaulStarted = 24;
	}
	
	// set IP
	if(qaulStarted == 24)
	{
		// set IP
		NSString *myip = [NSString stringWithFormat:@"%s",Qaullib_GetIP()];
		success = [mysudo setAddress:authorizationRef address:myip service:qaulServiceId];
		//success = [mysudo setAddress:authorizationRef address:@"10.202.0.40" service:qaulServiceId];
		if(success) 
			NSLog(@"setAddress success!!");
		else 
			NSLog(@"setAddress no success");		
		
		qaulStarted = 25;
	}
	
	if (qaulStarted == 25)
	{
		if(floor(NSAppKitVersionNumber) > NSAppKitVersionNumber10_5)  
			[self startDelay: 15.0f];
		else 
			[self startDelay: 3.0f];
	}
	
	// configure airport
	if(qaulStarted == 26)
	{
		success = [mysudo connect2network:authorizationRef name:@"qaul.net" channel:11 interface:qaulWifiInterface]; // channel selection is buggy on many devices, channel 1 usually works
		if(success) 
			NSLog(@"connect2network success!!");
		else 
			NSLog(@"connect2network no success");
		
		qaulStarted = 29;
	}
	
	//39 usleep(7000000);
	if(qaulStarted == 29) 
		[self startDelay: 3.0f];

	// check username
	if(qaulStarted == 30)
	{
		// wait until username is set
		NSLog(@"check Username");
		if(Qaullib_ExistsUsername()) 
			qaulStarted = 40;
		else 
		{
			qaulStarted = 29;
			[self startDelay: 0.5f];
		}
	}
	
	// start olsrd
	if(qaulStarted == 40)
	{
		success = [mysudo startOlsrd:authorizationRef interface:qaulWifiInterface];
		if(success) 
			NSLog(@"olsrd start success!!");
		else 
			NSLog(@"olsrd start no success");
		
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
		
		// start port forwarding
		success = [mysudo startPortForwarding:authorizationRef interface:qaulWifiInterface];
		
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
	[self performSelector:@selector(checkNames) withObject:nil afterDelay:2];	
}

- (void)checkAppEvents:(NSTimer *)theTimer
{
	int appEvent = Qaullib_TimedCheckAppEvent();
	if (appEvent > 0)
	{
		NSLog([NSString stringWithFormat:@"appEvent found: %i\n", appEvent]);

		if(appEvent == QAUL_EVENT_CHOOSEFILE)
		{
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
		}
		else if(appEvent == QAUL_EVENT_OPENFILE)
		{
			// open file
			[[NSWorkspace sharedWorkspace] openFile:[NSString stringWithFormat:@"%s", Qaullib_GetAppEventOpenPath()]];
		}
		else if(appEvent == QAUL_EVENT_OPENURL)
		{
			// open url
			[[NSWorkspace sharedWorkspace] openURL:[NSURL URLWithString:[NSString stringWithFormat:@"%s", Qaullib_GetAppEventOpenURL()]]];
		}
		else if(appEvent == QAUL_EVENT_QUIT)
		{
			[NSApp terminate:nil];
		}
		else if(appEvent == QAUL_EVENT_NOTIFY || appEvent == QAUL_EVENT_RING)
		{
			NSLog(@"QAUL_EVENT_NOTIFY or QAUL_EVENT_RING received\n");
			NSBeep();
		}
	}
}

- (void)checkNames
{
	NSLog(@"check usernames by tcp");
	Qaullib_TimedDownload();	
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
	[self init_app];
}


@end
