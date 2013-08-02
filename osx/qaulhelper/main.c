/*
 * qaul.net is free software
 * licensed under GPL (version 3)
 */

/**
 * qaul helper functions running as root user
 *
 * For this executable the suid bit needs to be set, to be
 * callable from userspace.
 *
 * usage:
 *   qaulhelper startolsrd <INTERFACE> <RESOURCEPATH>
 *   qaulhelper stopolsrd
 *   qaulhelper startportforwarding <INTERFACE>
 *   qaulhelper stopportforwarding
 *   qaulhelper enablewifi <OSXVERSION> <SERVICENAME>
 *   qaulhelper disablewifi <OSXVERSION> <SERVICENAME>
 *   qaulhelper setip <IP> <SERVICENAME>
 *   qaulhelper setdhcp <SERVICENAME> <INTERFACENAME>
 *   qaulhelper setdns
 *
 * NOTE: 
 *   networksetup path ( /usr/sbin/networksetup ) valid on OSX >= 10.5
 *   path on OSX < 10.5 was :
 *   /System/Library/CoreServices/RemoteManagement/ARDAgent.app/Contents/Support/networksetup
 */


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


#define NSAppKitVersionNumber10_4 824
#define NSAppKitVersionNumber10_5 949


/**
 * start olsrd
 */
int start_olsrd (int argc, const char * argv[]);

/**
 * stop olsrd
 */
int stop_olsrd (int argc, const char * argv[]);

/**
 * start port forwarding
 */
int start_portforwarding (int argc, const char * argv[]);

/**
 * stop port forwarding
 */
int stop_portforwarding (int argc, const char * argv[]);

/**
 * enable wifi
 */
int enable_wifi (int argc, const char * argv[]);

/**
 * disable wifi
 */
int disable_wifi (int argc, const char * argv[]);

/**
 * configure static IP for wifi
 */
int set_ip (int argc, const char * argv[]);

/**
 * configure DHCP for wifi
 */
int set_dhcp (int argc, const char * argv[]);

/**
 * check IP argument
 */
int check_ip (const char* ip);

/**
 * check each char of the IP
 */
int check_ip_char (char mychar);

/**
 * set dns servers
 */
int set_dns (int argc, const char * argv[]);



int main (int argc, const char * argv[])
{
    if (argc > 1)
    {
        if(strncmp(argv[1], "startolsrd", 10) == 0)
        {
            start_olsrd (argc, argv);
        }
        else if(strncmp(argv[1], "stopolsrd", 10) == 0)
        {
            stop_olsrd (argc, argv);
        }
        else if(strncmp(argv[1], "setip", 10) == 0)
        {
            set_ip (argc, argv);
        }
        else if(strncmp(argv[1], "setdhcp", 10) == 0)
        {
            set_dhcp (argc, argv);
        }
        else
            printf("unknown command\n");
    }
    else
    {
        printf("\n");
        printf("qaulhelper contains helper functions for qaul.net\n\n");
        printf("usage:\n");
        printf("  qaulhelper startolsrd <INTERFACE> <RESOURCEPATH>\n");
        printf("  qaulhelper stopolsrd\n");
        printf("  qaulhelper startportforwarding <INTERFACE>\n");
        printf("  qaulhelper stopportforwarding\n");
        printf("  qaulhelper enablewifi <OSXVERSION> <SERVICENAME>\n");
        printf("  qaulhelper disablewifi <OSXVERSION> <SERVICENAME>\n");
        printf("  qaulhelper setip <IP>\n");
        printf("  qaulhelper setdhcp <SERVICENAME> <INTERFACENAME>\n");
        printf("  qaulhelper setdns\n");
        printf("\n");
    }
    
    return 0;
}


int start_olsrd (int argc, const char * argv[])
{
    char s[255];
    printf("start olsrd as root\n");
    
    // retrieve current user id
    printf("orig user: %i\n", getuid());
    
    if(argc >= 4)
    {
        // set root rights
        setuid(0);
        
        // start olsrd
        sprintf(s,"%s/olsrd_start.sh \"%s\" %s", argv[3], argv[3], argv[2]);
        printf("%s\n",s);
        system(s);
            
        printf("olsrd started");
    }
    else
        printf("missing argument\n");

	return 0;
}

int stop_olsrd (int argc, const char * argv[])
{
    printf("stop olsrd\n");
    
    // kill olsrd
    system("killall olsrd");
    
    printf("olsrd stopped");
	return 0;
}

int start_portforwarding (int argc, const char * argv[])
{
    char s[255];
    printf("start olsrd as root\n");
    
    if(argc >= 3)
    {
        // set root rights
        setuid(0);
        
        // forward port 80 by firewall
        sprintf(s,"/sbin/ipfw add 1053 fwd localhost 8081 tcp from any to any 80 in recv %s", argv[2]);
        printf("%s\n",s);
        system(s);
        
        // start portforwarding of udp ports
        sprintf(s,"%s/socat_start.sh", argv[3]);
        printf("%s\n",s);
        system(s);
        
        printf("olsrd started");
    }
    else
        printf("missing argument\n");
    
    return 0;
}

int stop_portforwarding (int argc, const char * argv[])
{
    printf("stop port forwarding\n");
    
    // set root rights
    setuid(0);
    
    // remove firewall rules
    system("/sbin/ipfw delete 1053");
    // stop port forwarding
    system("/usr/bin/killall socat");
    
    printf("port forwarding stopped");
	return 0;
}

int enable_wifi (int argc, const char * argv[])
{
    char s[255];
    printf("enable wifi\n");
    
    if(argc >= 4)
    {
        // set root rights
        setuid(0);
        
        // enable wifi
        if(atoi(argv[2]) <= NSAppKitVersionNumber10_5)
            sprintf(s,"/usr/sbin/networksetup -setairportpower on");
        else
            sprintf(s,"/usr/sbin/networksetup -setairportpower %s on", argv[3]);
        
        printf("%s\n",s);
        system(s);

        printf("wifi enabled\n");
    }
    else
        printf("missing argument\n");
    
	return 0;
}

int disable_wifi (int argc, const char * argv[])
{
    char s[255];
    printf("disable wifi\n");
    
    // set root rights
    setuid(0);
    
    if(argc >= 4)
    {
        // set root rights
        setuid(0);
        
        // enable wifi
        if(atoi(argv[2]) <= NSAppKitVersionNumber10_5)
            sprintf(s,"/usr/sbin/networksetup -setairportpower off");
        else
            sprintf(s,"/usr/sbin/networksetup -setairportpower %s off", argv[3]);
        
        printf("%s\n",s);
        system(s);
        
        printf("wifi enabled\n");
    }
    else
        printf("missing argument\n");
    
	return 0;
}

int set_ip (int argc, const char * argv[])
{
    char s[255];    
    printf("configure ip\n");
    
    if(argc >= 4)
    {
        // check argument
        if (check_ip(argv[2]) == 0) {
            printf("argument 1 not valid\n");
            return 0;
        }
        
        // set root rights
        setuid(0);
        
        // set ip manually
        sprintf(s,"/usr/sbin/networksetup -setmanual %s %s 255.0.0.0 0.0.0.0", argv[3], argv[2]);
        printf("%s\n",s);
        system(s);
        
        printf("ip configured\n");
    }
    else
        printf("missing argument\n");
    
	return 0;
}

int set_dhcp (int argc, const char * argv[])
{
    char s[255];
    printf("set DHCP\n");
    
    if(argc >= 3)
    {
        // check argument
        if (check_ip(argv[2]) == 0) {
            printf("argument 1 not valid\n");
            return 0;
        }
        
        // set root rights
        setuid(0);
        
        // set dhcp
        sprintf(s,"/usr/sbin/networksetup -setdhcp %s", argv[2]);
        printf("%s\n",s);
        system(s);
        
        usleep(200000);
        
        sprintf(s,"/usr/sbin/ipconfig set %s DHCP", argv[3]);
        printf("%s\n",s);
        system(s);
        
        printf("DHCP set\n");
    }
    else
        printf("missing argument\n");
    
	return 0;
}

int check_ip (const char* ip)
{
    int i;
    
    // check length
    if(strlen(ip)>15)
    {
        printf("argument too big\n");
        return 0;
    }
    
    // check numbers and dots
    for(i=0; i<strlen(ip); i++)
    {
        if(check_ip_char(ip[i]) == 0)
        {
            printf("invalid character\n");
            return 0;
        }
    }
    return 1;
}

int check_ip_char (char mychar)
{
    if(strncmp(&mychar, "0", 1))
        return 1;
    else if(strncmp(&mychar, "1", 1))
        return 1;
    else if(strncmp(&mychar, "2", 1))
        return 1;
    else if(strncmp(&mychar, "3", 1))
        return 1;
    else if(strncmp(&mychar, "4", 1))
        return 1;
    else if(strncmp(&mychar, "5", 1))
        return 1;
    else if(strncmp(&mychar, "6", 1))
        return 1;
    else if(strncmp(&mychar, "7", 1))
        return 1;
    else if(strncmp(&mychar, "8", 1))
        return 1;
    else if(strncmp(&mychar, "9", 1))
        return 1;
    else if(strncmp(&mychar, ".", 1))
        return 1;
    
    return 0;
}

int set_dns (int argc, const char * argv[])
{
    printf("set DNS\n");
    
    // set root rights
    setuid(0);
    
    // set dns
    system("/usr/sbin/networksetup -setdnsservers 178.254.31.11 176.10.111.206");
    
    printf("DNS set\n");
	return 0;
}

