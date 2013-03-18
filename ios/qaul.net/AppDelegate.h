/*
 * qaul.net is free software
 * licensed under GPL (version 3)
 */

#import <UIKit/UIKit.h>

@class ViewController;

@interface AppDelegate : UIResponder <UIApplicationDelegate> {
    UIWindow *_window;
    ViewController *_viewController;
	
	NSString *qaulResourcePath;
	NSString *qaulDocumentPath;
	int qaulStarted;
	OSStatus status;
    //	AuthorizationRef authorizationRef;
	
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
     */
	// Timer
	NSTimer *qaullibTimer;
	NSTimer *qaullibTimer2;
	NSTimer *qaullibTimer3;
    
}

@property (strong, nonatomic) UIWindow *window;
@property (strong, nonatomic) ViewController *viewController;

/**
 * copy files to the documents directory the first time the application is started
 */
- (void) CopyFilesAtFirstStartup;
- (void) CreateDirectory:(NSString *)directoryName atFilePath:(NSString *)filePath;

/**
 * set global variables
 */
- (void) QaulSetGlobals;

/**
 * startup and configure app
 */
- (void) QaulConfigure;

/**
 * wait for secs until running QaulConfigure again
 */
- (void)startDelay:(NSTimeInterval)secs;
- (void)fireDelay;

/**
 * timers
 */
- (void)startTimer;
- (void)stopTimer;
- (void)checkIpcMessage:(NSTimer *)theTimer;
- (void)checkIpcTopology:(NSTimer *)theTimer;

@end
