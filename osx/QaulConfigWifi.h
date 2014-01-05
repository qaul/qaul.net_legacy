//
//  QaulConfigWifi.h
//  wificonfig
//
//  GPL software by qaul.net
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
    NSString* qaulhelperPath;
	NSString* networkProfile;
}

- (void)setPaths;
- (BOOL)runTask:(NSString*)path arguments:(NSArray*)arguments;
- (BOOL)startAirport:(SCNetworkInterfaceRef)interface;
- (BOOL)stopAirport:(SCNetworkInterfaceRef)interface;
- (BOOL)setAddress:(NSString*)address service:(SCNetworkServiceRef)service;
- (BOOL)setDhcp:(SCNetworkServiceRef)service interface:(SCNetworkInterfaceRef)interface;
- (BOOL)connect2network:(NSString*)name channel:(int)channel interface:(SCNetworkInterfaceRef)interface service:(SCNetworkServiceRef)service;
- (BOOL)startOlsrd:(SCNetworkInterfaceRef)interface;
- (BOOL)stopOlsrd;
- (BOOL)startPortForwarding:(SCNetworkInterfaceRef)interface;
- (BOOL)stopPortForwarding;
- (BOOL)createNetworkProfile;
- (BOOL)deleteNetworkProfile;

@end
