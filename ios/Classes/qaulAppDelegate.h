/*
 * qaul.net is free software
 * licensed under GPL (version 3)
 */

#import <UIKit/UIKit.h>

@class qaulViewController;

@interface qaulAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    qaulViewController *viewController;
	
	NSString *qaulResourcePath;
	int qaulStarted;
	
/*
	// wifi config
	QaulConfigWifi *mysudo;
	NSArray * qaulInterfacesAll;
	NSEnumerator *en;
	SCNetworkInterfaceRef qaulWifiInterface;
	BOOL qaulWifiInterfaceSet;
	SCNetworkServiceRef qaulServiceId;
	BOOL qaulServiceFound;
	BOOL qaulServiceConfigured;
	CFStringRef qaulServiceName;
	OSStatus status;
	AuthorizationRef authorizationRef;
	
	// Timer
	NSTimer *qaullibTimer;
	NSTimer *qaullibTimer2;
	NSTimer *qaullibTimer3;
*/
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet qaulViewController *viewController;

/**
 * copy files to the documents directory the first time the application is started
 */
- (void) CopyFilesAtFirstStartup;

/**
 * startup and configure app
 */
- (void) QaulConfigure;

@end

