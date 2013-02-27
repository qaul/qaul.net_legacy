//
//  QaulConfigWifi.h
//  wificonfig
//
//  Created by foton on 3/5/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <Foundation/Foundation.h>
#import <SystemConfiguration/SystemConfiguration.h>
#import <CoreFoundation/CFArray.h>
#import <AppKit/AppKit.h>
#import <SecurityFoundation/SFAuthorization.h>


@interface QaulConfigWifi : NSObject {
	NSString* olsrdPath;
	NSString* resourcePath;
	NSString* networksetupPath; 
	NSString* airportPath;
	NSString* networkProfile;
}

- (void)setPaths;
- (BOOL)syscall:(AuthorizationRef)authRef command:(NSString*)strCmd arguments:(NSArray*)args;
- (BOOL)testAuthorization:(AuthorizationRef)authRef;
- (BOOL)startAirport:(AuthorizationRef)authRef interface:(SCNetworkInterfaceRef)interface;
- (BOOL)stopAirport:(AuthorizationRef)authRef interface:(SCNetworkInterfaceRef)interface;
- (BOOL)setAddress:(AuthorizationRef)authRef address:(NSString*)address service:(SCNetworkServiceRef)service;
- (BOOL)setDhcp:(AuthorizationRef)authRef service:(SCNetworkServiceRef)service interface:(SCNetworkInterfaceRef)interface;
- (BOOL)connect2network:(AuthorizationRef)authRef name:(NSString*)name channel:(int)channel interface:(SCNetworkInterfaceRef)interface service:(SCNetworkServiceRef)service;
- (BOOL)startOlsrd:(AuthorizationRef)authRef interface:(SCNetworkInterfaceRef)interface;
- (BOOL)stopOlsrd:(AuthorizationRef)authRef;
- (BOOL)startPortForwarding:(AuthorizationRef)authRef interface:(SCNetworkInterfaceRef)interface;
- (BOOL)stopPortForwarding:(AuthorizationRef)authRef;
- (BOOL)runTask:(NSString*)path arguments:(NSArray*)arguments;
- (BOOL)createNetworkProfile:(AuthorizationRef)authRef;
- (BOOL)deleteNetworkProfile:(AuthorizationRef)authRef;

@end
