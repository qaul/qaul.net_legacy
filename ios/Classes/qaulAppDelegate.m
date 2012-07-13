/*
 * qaul.net is free software
 * licensed under GPL (version 3)
 */

#import "qaulAppDelegate.h"
#import "qaulViewController.h"
#import "../../libqaul/qaullib.h"

@implementation qaulAppDelegate

@synthesize window;
@synthesize viewController;


#pragma mark -
#pragma mark Application lifecycle

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {    
    
    // Override point for customization after application launch.
	// copy files at first startup
	[self CopyFilesAtFirstStartup];
	
	
	// initialize app
	NSLog(@"initialize app");		
	Qaullib_Init([qaulResourcePath UTF8String]);
	
	// start web server
	NSLog(@"start web server");
	if(Qaullib_WebserverStart())
		NSLog(@"successful web server start");
	else		
		NSLog(@"web server start up error");
	
	// Set the view controller as the window's root view controller and display.
    self.window.rootViewController = self.viewController;
    [self.window makeKeyAndVisible];
	
	// start app configuration
	[self QaulConfigure];
	
    return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application {
    /*
     Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
     Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
     */
}


- (void)applicationDidEnterBackground:(UIApplication *)application {
    /*
     Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
     If your application supports background execution, called instead of applicationWillTerminate: when the user quits.
     */
}


- (void)applicationWillEnterForeground:(UIApplication *)application {
    /*
     Called as part of  transition from the background to the inactive state: here you can undo many of the changes made on entering the background.
     */
}


- (void)applicationDidBecomeActive:(UIApplication *)application {
    /*
     Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
     */
}


- (void)applicationWillTerminate:(UIApplication *)application {
    /*
     Called when the application is about to terminate.
     See also applicationDidEnterBackground:.
     */
	
	Qaullib_Exit();
}


#pragma mark -
#pragma mark Qaul Configuration

- (void) CopyFilesAtFirstStartup 
{
	NSFileManager *filemgr;
	NSArray *documentsPaths;
	NSError *dError;
	
	filemgr =[NSFileManager defaultManager];
	documentsPaths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
	qaulResourcePath = [documentsPaths objectAtIndex:0];
	
	// check if data base exists
	if(![filemgr fileExistsAtPath:[NSString stringWithFormat:@"%@/qaullib.db", qaulResourcePath]])
	{
		NSLog(@"First startup: copy files to document directory");
		
		if(![filemgr copyItemAtPath:[NSString stringWithFormat:@"%@/../qaul.app/www", qaulResourcePath] toPath:[NSString stringWithFormat:@"%@/www", qaulResourcePath] error:&dError])
			NSLog(@"Error: %@", dError);
		else
			NSLog(@"Sucessfully copied");
	}
	else 
		NSLog(@"Not first startup");
	
	[filemgr release];
}

- (void) QaulSetGlobals
{
	qaulStarted = 0;
}

- (void) QaulConfigure 
{
	NSLog(@"configure app");
	BOOL success;
	
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
		// todo: how to get root previleges
/*
		// Create authorization reference
		// authorization
		NSLog(@"authorize");
		status = AuthorizationCreate(NULL, kAuthorizationEmptyEnvironment, kAuthorizationFlagDefaults, &authorizationRef);	
		if (status == errAuthorizationSuccess) NSLog(@"Authorization success");
		else NSLog(@"Authorization failed");
*/		
		// use the authorization
//		if([mysudo testAuthorization:authorizationRef]) NSLog(@"Authorization tested success");
//		else NSLog(@"Authorization tested error");
		
		qaulStarted = 20;
	}
/*	
	// search Wifi interface
	if(qaulStarted == 20)
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
		qaulStarted = 21;
	}
	
	// enable Service
	if(qaulStarted == 21)
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
		
		qaulStarted = 22;
	}
	
	// switch airport on
	if(qaulStarted == 22)
	{
		NSLog(@"switch airport on");
		// switch on airport via cli 
		success = [mysudo startAirport:authorizationRef interface:qaulWifiInterface];
		if(success) NSLog(@"startAirport success!!");
		else NSLog(@"startAirport no success");
		
		// set IP
		NSString *myip = [NSString stringWithFormat:@"%s",Qaullib_GetIP()];
		success = [mysudo setAddress:authorizationRef address:myip service:qaulServiceId];
		//success = [mysudo setAddress:authorizationRef address:@"10.202.0.40" service:qaulServiceId];
		if(success) NSLog(@"setAddress success!!");
		else NSLog(@"setAddress no success");
		
		qaulStarted = 23;
	}
	
	//23 usleep(1000000);
	if (qaulStarted == 23)
	{
		if(floor(NSAppKitVersionNumber) > NSAppKitVersionNumber10_5)  [self startDelay: 15.0f];
		else [self startDelay: 3.0f];
	}
	
	// configure airport
	if(qaulStarted == 24)
	{
		success = [mysudo connect2network:authorizationRef name:@"qaul.net" channel:11 interface:qaulWifiInterface];
		if(success) NSLog(@"connect2network success!!");
		else NSLog(@"connect2network no success");
		
		qaulStarted = 29;
	}
	
	//39 usleep(7000000);
	if(qaulStarted == 29) [self startDelay: 3.0f];
	
	// check username
	if(qaulStarted == 30)
	{
		// wait until username is set
		NSLog(@"check Username");
		if(Qaullib_ExistsUsername()) qaulStarted = 40;
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
		if(success) NSLog(@"olsrd start success!!");
		else NSLog(@"olsrd start no success");
		
		qaulStarted = 44;
	}	
	
	//if(qaulStarted == 49) usleep(2000000);
	if(qaulStarted == 44) [self startDelay: 2.0f];
	
	// connect ipc
	if(qaulStarted == 45)
	{
		Qaullib_IpcConnect();
		
		qaulStarted = 46;
	}
	
	// connect captive
	if(qaulStarted == 46)
	{
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
*/
}

#pragma mark -
#pragma mark Memory management

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application {
    /*
     Free up as much memory as possible by purging cached data objects that can be recreated (or reloaded from disk) later.
     */
}


- (void)dealloc {
    [viewController release];
    [window release];
    [super dealloc];
}


@end
