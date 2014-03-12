/*
 * qaul.net is free software
 * licensed under GPL (version 3)
 */

#import <Cocoa/Cocoa.h>
#import <WebKit/WebView.h>
#import "QaulConfigWifi.h"


@interface WindowController : NSObject /* Specify a superclass (eg: NSObject or NSView) */ {
    IBOutlet id start_name;
    IBOutlet id qaulTabView;
	IBOutlet WebView *qaulWebView;
	NSString *username;
	NSString *qaulResourcePath;
	int qaulStarted;
	
	// wifi config
	QaulConfigWifi *qaulConfigWifi;
//	NSArray * qaulInterfacesAll;
//	NSEnumerator *en;
    BOOL qaulInterfaceManual;
    NSString *qaulInterfaceName;
	SCNetworkInterfaceRef qaulWifiInterface;
	BOOL qaulWifiInterfaceSet;
    BOOL qaulWifiInterfaceConfigurable;
	SCNetworkServiceRef qaulServiceId;
	BOOL qaulServiceFound;
	BOOL qaulServiceConfigured;
	CFStringRef qaulServiceName;
	OSStatus status;
//	AuthorizationRef authorizationRef;
	
	// Timer
	NSTimer *qaullibTimer;
	NSTimer *qaullibTimer2;
	NSTimer *qaullibTimer3;
}

/**
 * copy files to the home directory the first time the application is started
 */
- (void)copyFilesAtFirstStartup;
- (void)createDirectory:(NSString *)directoryName atFilePath:(NSString *)filePath;

/**
 * window loaded
 */
- (void)awakeFromNib;
- (void)applicationDidFinishLaunching:(id)sender;

/**
 * window is closing
 */
- (BOOL)windowShouldClose:(id)sender;
- (BOOL)applicationShouldTerminate:(id)app;

/** 
 * support methods
 */
- (void)endAlert:(id)sheet returnCode:(int)returnCode contextInfo:(void *)contextInfo;

- (void)init_app;

/**
 * create json of all interfaces for qaullib
 */
- (void)createInterfaceJson;

/**
 * timers
 */
- (void)startTimer;
- (void)stopTimer;
- (void)checkIpcMessage:(NSTimer *)theTimer;
- (void)checkIpcTopology:(NSTimer *)theTimer;
- (void)checkNames;

/**
 * wait timer
 */
- (void)startDelay:(NSTimeInterval)secs;
- (void)fireDelay;

@end
