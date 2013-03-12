//
//  QaulConfigWifi.m
//  wificonfig
//
//  Created by foton on 3/5/11.
//  GPL software by qaul.net
//

#import "QaulConfigWifi.h"

// ----------------------------------------------
// definitions
#ifndef NSAppKitVersionNumber10_4
#define NSAppKitVersionNumber10_4 824
#else
#define IS_OSX_10_5_OR_HIGHER 1
#endif

#ifndef NSAppKitVersionNumber10_5
#define NSAppKitVersionNumber10_5 949
#else
#define IS_OSX_10_6_OR_HIGHER 1
#endif

#ifndef NSAppKitVersionNumber10_6
#define NSAppKitVersionNumber10_6 1038
#else
#define IS_OSX_10_7_OR_HIGHER 1
#endif
// ----------------------------------------------

#ifdef IS_OSX_10_6_OR_HIGHER
#import <CoreWLAN/CoreWLAN.h>
#endif

@implementation QaulConfigWifi

- (id) init 
{ 
	if( self = [super init] ) 
	{ 
		resourcePath = [[NSBundle mainBundle] resourcePath];
		olsrdPath = [NSString stringWithFormat:@"%@/olsrd",resourcePath];

		if(floor(NSAppKitVersionNumber) > NSAppKitVersionNumber10_4) 
			networksetupPath=[NSString stringWithFormat:@"/usr/sbin/networksetup"];
		else 
			networksetupPath=[NSString stringWithFormat:@"/System/Library/CoreServices/RemoteManagement/ARDAgent.app/Contents/Support/networksetup"];	
		
		airportPath = [NSString stringWithFormat:@"/System/Library/PrivateFrameworks/Apple80211.framework/Versions/Current/Resources/airport"];
	} 
	return self; 
} 

- (void)setPaths
{
	resourcePath = [[NSBundle mainBundle] resourcePath];
	olsrdPath = [NSString stringWithFormat:@"%@/olsrd",resourcePath];
	
	if(floor(NSAppKitVersionNumber) > NSAppKitVersionNumber10_4) 
		networksetupPath=[NSString stringWithFormat:@"/usr/sbin/networksetup"];
	else 
		networksetupPath=[NSString stringWithFormat:@"/System/Library/CoreServices/RemoteManagement/ARDAgent.app/Contents/Support/networksetup"];	
	
	airportPath = [NSString stringWithFormat:@"/System/Library/PrivateFrameworks/Apple80211.framework/Versions/Current/Resources/airport"];	
}

- (BOOL)syscall:(AuthorizationRef)authRef command:(NSString*)strCmd arguments:(NSArray*)args
{
	OSStatus err;
    FILE *fd = NULL;
    int argc = [args count];
    char **argv = (char **)malloc(sizeof(char *) * (argc + 1));
    int i;
	
	NSLog([NSString stringWithFormat:@"command: %@", strCmd]);
    for(i=0; i<argc; i++)
	{
        argv[i] = (char *) [[[args objectAtIndex:i] description] UTF8String];
		NSLog([NSString stringWithFormat:@"argument %i: %@", i, [[args objectAtIndex:i] description]]);
	}
	argv[i] = NULL;
	
	if( (err = AuthorizationExecuteWithPrivileges(
											 authRef,
											 [strCmd UTF8String], 
											 kAuthorizationFlagDefaults, 
											 argv, &fd)) != noErr )
	{
		NSLog([NSString stringWithFormat:@"error: %d", err]);
	}
	else
	{
		//// wait for process to terminate
		//int child;
		//wait(&child);
		//close(fileno(fd));
		//NSLog(@"no error");
	}
	
	// this function crashes... how can the memory be released
	//free(*argv);
	
	return (err ==0) ? true : false;
}

- (BOOL)testAuthorization:(AuthorizationRef)authRef
{
	[self setPaths];
	NSString *strAuth = @"/sbin/dmesg";
	return [self syscall:authRef command:strAuth arguments:[NSArray arrayWithObjects:nil]];
}

- (BOOL)startAirport:(AuthorizationRef)authRef interface:(SCNetworkInterfaceRef)interface
{
	[self setPaths];
	NSArray *arguments;
	if(floor(NSAppKitVersionNumber) > NSAppKitVersionNumber10_5)
	{
		arguments = [NSArray arrayWithObjects:@"-setairportpower",SCNetworkInterfaceGetBSDName(interface),@"on",nil];
	}
	else 
	{
		arguments = [NSArray arrayWithObjects:@"-setairportpower",@"on",nil];
	}
	return [self syscall:authRef command:networksetupPath arguments:arguments];
}

- (BOOL)stopAirport:(AuthorizationRef)authRef interface:(SCNetworkInterfaceRef)interface
{
	[self setPaths];
	NSArray *arguments;
	if(floor(NSAppKitVersionNumber) > NSAppKitVersionNumber10_5)
	{
		arguments = [NSArray arrayWithObjects:@"-setairportpower",SCNetworkInterfaceGetBSDName(interface),@"off",nil];
	}
	else 
	{
		arguments = [NSArray arrayWithObjects:@"-setairportpower",@"off",nil];
	}
	return [self syscall:authRef command:networksetupPath arguments:arguments];
}

- (BOOL)setAddress:(AuthorizationRef)authRef address:(NSString*)address service:(SCNetworkServiceRef)service
{
	[self setPaths];
	return [self syscall:authRef command:networksetupPath arguments:[NSArray arrayWithObjects:@"-setmanual",SCNetworkServiceGetName(service),address,@"255.0.0.0",@"0.0.0.0",nil]];
}

- (BOOL)setDhcp:(AuthorizationRef)authRef service:(SCNetworkServiceRef)service interface:(SCNetworkInterfaceRef)interface
{
	[self setPaths];
	NSArray *arguments = [NSArray arrayWithObjects:@"-setdhcp",SCNetworkServiceGetName(service),nil];
	if(![self syscall:authRef command:networksetupPath arguments:arguments]) 
		NSLog(@"Wifi DHCP not set");
	usleep(200000);
	NSArray *arguments2 = [NSArray arrayWithObjects:@"set",SCNetworkInterfaceGetBSDName(interface),@"DHCP",nil];
	return [self syscall:authRef command:[NSString stringWithFormat:@"/usr/sbin/ipconfig"] arguments:arguments2];
}


- (BOOL)connect2network:(AuthorizationRef)authRef name:(NSString*)name channel:(int)channel interface:(SCNetworkInterfaceRef)interface service:(SCNetworkServiceRef)service
{
	NSLog(@"connect 2 network\n");
	
	[self setPaths];
	if(floor(NSAppKitVersionNumber) < NSAppKitVersionNumber10_6)
	{
		NSLog(@"OSX 10.5 or lower");
		return [self syscall:authRef command:airportPath arguments:[NSArray arrayWithObjects:[NSString stringWithFormat:@"-i%@",name],[NSString stringWithFormat:@"-c%i",channel],nil]];
	}
#ifdef IS_OSX_10_6_OR_HIGHER
	else 
	{
		NSLog(@"OSX 10.6 or higher");
		
		// use CoreWLAN framework for OSX >= 10.6
		// ----------------------------------------------
		// create ibss network
		NSMutableDictionary *ibssParams = [NSMutableDictionary dictionaryWithCapacity:0];
		[ibssParams setValue:name forKey:kCWIBSSKeySSID];
		[ibssParams setValue:[NSNumber numberWithInt:channel] forKey:kCWIBSSKeyChannel];
		//[ibssParams setValue:@"" forKey:kCWIBSSKeyPassphrase];
		NSError *error = nil;
		CWInterface* wifiInterface = [CWInterface interfaceWithName:[NSString stringWithFormat:@"%@",SCNetworkInterfaceGetBSDName(interface)]];
		//if (wifiInterface) NSLog(@"CWInterface wifi interface created: %@", wifiInterface);
		//else NSLog([NSString stringWithFormat:@"%@",SCNetworkInterfaceGetBSDName(interface)]);
		BOOL created = [wifiInterface enableIBSSWithParameters:[NSDictionary dictionaryWithDictionary:ibssParams] error:&error];
		
		// if creation failed try to join the existing qaul.net network
		if(!created)
		{
			//NSLog(@"Error: %@", error);
			NSLog(@"join network");
			
			CW8021XProfile *user8021XProfile = [CW8021XProfile profile];
			user8021XProfile.ssid = name;
			user8021XProfile.userDefinedName = name;			
			user8021XProfile.username = nil;
			user8021XProfile.password = nil;
			
			NSMutableDictionary *params = [NSMutableDictionary dictionaryWithCapacity:0];
			[params setValue:user8021XProfile forKey:kCWAssocKey8021XProfile];
			// scan for network:
			error = nil;
			NSMutableArray* scan = [NSMutableArray arrayWithArray:[wifiInterface scanForNetworksWithParameters:params error:&error]];
			if(error) 
				NSLog(@"scanning error: %@", error);
			else 
				NSLog(@"objects in array: %i",[scan count]);
			// loop through networks and search for qaul.net
			CWNetwork *selectedNetwork;
			for(selectedNetwork in scan)
			{
				if ([name isEqualToString:selectedNetwork.ssid]) 
					break;
			}
			NSLog(@"network Name: %@", selectedNetwork.ssid);
			if(selectedNetwork)
			{
				error = nil;
				params = nil;
				[params setValue:nil forKey:kCWAssocKeyPassphrase];
				created = [wifiInterface associateToNetwork:selectedNetwork parameters:[NSDictionary dictionaryWithDictionary:params] error:&error];
				if(created) 
				{
					NSLog(@"qaul.net joined");
				}
				else 
					NSLog(@"joining qaul.net failed: %@", error);				
			}
			else 
			{
				NSLog(@"Network qaul.net not found!");
				created = false;
			}

		}
		
		// set dns servers for internet gateway
		// TODO: WLAN adapter names with spaces
		if([self syscall:authRef command:networksetupPath arguments:[NSArray arrayWithObjects:@"-setdnsservers",SCNetworkServiceGetName(service),@"88.84.130.20",@"176.10.111.206",nil]])
			NSLog(@"DNS servers set");
			
		return created;
	}
#endif
	return false;
}

- (BOOL)startOlsrd:(AuthorizationRef)authRef interface:(SCNetworkInterfaceRef)interface
{
	[self setPaths];
	
	// ------------------------------------------------
	// uses script & destroys network connection
	// destroys network connection
	NSString *startScript = [NSString stringWithFormat:@"%@/olsrd_start.sh",resourcePath];
	return [self syscall:authRef command:startScript arguments:[NSArray arrayWithObjects:resourcePath,SCNetworkInterfaceGetBSDName(interface),nil]];
	
	// ------------------------------------------------
	// invoke the olsrd directly
	// needs to copy the shared olsrd qaul library into the library folder
	// invoke program
	//NSString *startScript = [NSString stringWithFormat:@"%@/olsrd",resourcePath];
	//return [self syscall:authRef command:startScript arguments:[NSArray arrayWithObjects:@"-i",SCNetworkInterfaceGetBSDName(interface),@"-f",[NSString stringWithFormat:@"%@/olsrd_osx.conf",resourcePath],@"-d",@"0",nil]];
}

- (BOOL)stopOlsrd:(AuthorizationRef)authRef
{
	[self setPaths];
	NSString *stopScript = [NSString stringWithFormat:@"%@/olsrd_stop.sh",resourcePath];
	return [self syscall:authRef command:stopScript arguments:[NSArray arrayWithObjects:nil]];
}

- (BOOL)startPortForwarding:(AuthorizationRef)authRef interface:(SCNetworkInterfaceRef)interface
{
	NSLog(@"start port forwarding");
	
	// forward port 80 by firewall
	[self setPaths];
	[self syscall:authRef command:@"/sbin/ipfw" arguments:[NSArray arrayWithObjects:@"add",@"1053",@"fwd",@"localhost,8081",@"tcp",@"from",@"any",@"to",@"any",@"80",@"in",@"recv",SCNetworkInterfaceGetBSDName(interface),nil]];

	NSLog(@"start udp forwarding");
	
	// start portforwarding of udp ports
	[self syscall:authRef command:[NSString stringWithFormat:@"%@/socat_start.sh",resourcePath] arguments:[NSArray arrayWithObjects:resourcePath,nil]];
	//[self syscall:authRef command:[NSString stringWithFormat:@"%@/socat",resourcePath] arguments:[NSArray arrayWithObjects:@"UDP4-RECVFROM:67,fork",@"UDP4-SENDTO:localhost:8067","&",nil]];
	//[self syscall:authRef command:[NSString stringWithFormat:@"%@/socat",resourcePath] arguments:[NSArray arrayWithObjects:@"UDP4-RECVFROM:53,fork",@"UDP4-SENDTO:localhost:8053","&",nil]];
	NSLog(@"udp forwarding started");
	
	return true;
}

- (BOOL)stopPortForwarding:(AuthorizationRef)authRef
{
	// remove firewall rules
	[self syscall:authRef command:@"/sbin/ipfw" arguments:[NSArray arrayWithObjects:@"delete",@"1053",nil]];
	
	// stop port forwarding
	[self syscall:authRef command:@"/usr/bin/killall" arguments:[NSArray arrayWithObjects:@"socat",nil]];
	
	return true;
}

- (BOOL)runTask:(NSString*)path arguments:(NSArray*)arguments
{
	NSTask *task;
	task = [[NSTask alloc] init];
	[task setLaunchPath:path];
	
	[task setArguments: arguments];
	
	NSPipe *pipe;
	pipe = [NSPipe pipe];
	[task setStandardOutput: pipe];
	
	NSFileHandle *file;
	file = [pipe fileHandleForReading];
	
	[task launch];
	
	NSData *data;
	data = [file readDataToEndOfFile];
	
	NSString *myString;
	myString = [[NSString alloc] initWithData: data encoding: NSUTF8StringEncoding];
	
	[task release];
	[myString release];
	
	return true;
}

- (BOOL)createNetworkProfile:(AuthorizationRef)authRef
{
	NSLog(@"createNetworkProfile");
	
	NSTask *task;
	task = [[NSTask alloc] init];
	[task setLaunchPath: @"/usr/sbin/networksetup"];
	
	NSArray *arguments;
	arguments = [NSArray arrayWithObjects: @"-getcurrentlocation", nil];
	[task setArguments: arguments];
	
	NSPipe *pipe;
	pipe = [NSPipe pipe];
	[task setStandardOutput: pipe];
	
	NSFileHandle *file;
	file = [pipe fileHandleForReading];
	
	[task launch];
	
	NSData *data;
	data = [file readDataToEndOfFile];
	
	NSString *allFile;	
	allFile = [[NSString alloc] initWithData: data encoding: NSUTF8StringEncoding];
	arguments = [allFile componentsSeparatedByString:@"\n"];
	networkProfile = [[arguments objectAtIndex:0] description];
	
	NSLog (@"current networkProfile: '%@'", networkProfile);
	[self runTask:@"/usr/sbin/networksetup" arguments:[NSArray arrayWithObjects:@"-createlocation",@"new",@"populate",nil]];
/*
	if([networkProfile isEqualToString:@"qaul.net"])
	{
		networkProfile = @"new";
		[self runTask:@"/usr/sbin/networksetup" arguments:[NSArray arrayWithObjects:@"-createlocation",networkProfile,@"populate",nil]];
		NSLog(@"new networkProfile name '%@'", networkProfile);
	}
*/
	[task release];
	
	// ----------------------------------------------------------------

	[self runTask:@"/usr/sbin/networksetup" arguments:[NSArray arrayWithObjects:@"-createlocation",@"qaul.net",@"populate",nil]];
	[self runTask:@"/usr/sbin/networksetup" arguments:[NSArray arrayWithObjects:@"-switchtolocation",@"qaul.net",nil]];

	NSLog(@"createNetworkProfile created");
	return true;	
}

- (BOOL)deleteNetworkProfile:(AuthorizationRef)authRef
{
	NSLog(@"deleteNetworkProfile");
	
	[self runTask:@"/usr/sbin/networksetup" arguments:[NSArray arrayWithObjects:@"-switchtolocation",@"new",nil]];
	//[self runTask:@"/usr/sbin/networksetup" arguments:[NSArray arrayWithObjects:@"-deletelocation",@"qaul.net",nil]];

	NSLog(@"deleteNetworkProfile deleted");
	
	return true;		
}

@end
