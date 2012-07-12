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

- (void) QaulConfigure 
{
	
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
