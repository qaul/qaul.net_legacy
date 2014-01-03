/*
 * qaul.net is free software
 * licensed under GPL (version 3)
 */

/**
 * qaul helper functions running as root user
 *
 * For this executable the suid bit needs to be set, to be
 * callable from userspace. This is done by qaul.net on the first 
 * program start. 
 * To set the suid bit manually for testing, open a Terminal, navigate
 * to the qaulhelper executable binary and type:
 *   sudo chown root qaulhelper
 *   sudo chgrp wheel qaulhelper
 *   sudo chmod 6755 qaulhelper
 *
 *
 * usage:
 *   qaulhelper startolsrd <RESOURCEPATH> <INTERFACE> 
 *   qaulhelper startolsrd /Applications/qaul.app/Contents/Resources en1
 *   qaulhelper stopolsrd
 *   qaulhelper startportforwarding <INTERFACE>
 *   qaulhelper startportforwarding en1
 *   qaulhelper stopportforwarding
 *   qaulhelper enablewifi <OSXVERSION> <INTERFACE>
 *   qaulhelper enablewifi 1038 en1
 *   qaulhelper disablewifi <OSXVERSION> <INTERFACE>
 *   qaulhelper disablewifi 1038 en1
 *   qaulhelper createnetworkprofile <POFILENAME>
 *   qaulhelper createnetworkprofile qaul.net
 *   qaulhelper switchnetworkprofile <POFILENAME>
 *   qaulhelper switchnetworkprofile qaul.net
 *   qaulhelper deletenetworkprofile <POFILENAME>
 *   qaulhelper deletenetworkprofile qaul.net
 *   qaulhelper setip <SERVICENAME> <IP> <SUBNET> <ROUTER>
 *   qaulhelper setip Wi-Fi 10.213.28.55 255.0.0.0 0.0.0.0
 *   qaulhelper setdhcp <SERVICENAME> <INTERFACENAME>
 *   qaulhelper setdhcp Wi-Fi en1
 *   qaulhelper setdns <SERVICENAME>
 *   qaulhelper setdns Wi-Fi
 * 
 * only for OSX <= 10.5
 *   qaulhelper createibss <ESSID> <CHANNEL>
 *   qaulhelper createibss qaul.net 11
 *
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
 * create network profile
 */
int create_networkprofile (int argc, const char * argv[]);

/**
 * switch network profile
 */
int switch_networkprofile (int argc, const char * argv[]);

/**
 * delete network profile
 */
int delete_networkprofile (int argc, const char * argv[]);

/**
 * create or join IBSS
 *
 * only for OSX <= 10.5
 */
int create_ibss (int argc, const char * argv[]);

/**
 * configure static IP for wifi
 */
int set_ip (int argc, const char * argv[]);

/**
 * configure DHCP for wifi
 */
int set_dhcp (int argc, const char * argv[]);

/**
 * set dns servers
 */
int set_dns (int argc, const char * argv[]);

/**
 * validate IP argument
 */
int validate_ip (const char* str);

/**
 * validate interface argument
 */
int validate_interface (const char* str);

/**
 * validate service argument
 */
int validate_service (const char* str);

/**
 * validate essid argument
 */
int validate_essid (const char* str);

/**
 * validate number argument
 */
int validate_number (const char* str);

/**
 * validate path argument
 */
int validate_path (const char* str);

/**
 * check if the char is a number
 */
int validate_char_number (char mychar);

/**
 * check if the char is an ascii letter
 */
int validate_char_letter (char mychar);

/**
 * check char for problematic entities
 */
int validate_char_problematic (char mychar);


int main (int argc, const char * argv[])
{
    if (argc > 1)
    {
        if(strncmp(argv[1], "startolsrd", 10) == 0)
        {
            start_olsrd(argc, argv);
        }
        else if(strncmp(argv[1], "stopolsrd", 9) == 0)
        {
            stop_olsrd(argc, argv);
        }
        else if(strncmp(argv[1], "startportforwarding", 19) == 0)
        {
            start_portforwarding(argc, argv);
        }
        else if(strncmp(argv[1], "stopportforwarding", 18) == 0)
        {
            stop_portforwarding(argc, argv);
        }
        else if(strncmp(argv[1], "enablewifi", 10) == 0)
        {
            enable_wifi(argc, argv);
        }
        else if(strncmp(argv[1], "disablewifi", 11) == 0)
        {
            disable_wifi(argc, argv);
        }
        else if(strncmp(argv[1], "createnetworkprofile", 20) == 0)
        {
            create_networkprofile(argc, argv);
        }
        else if(strncmp(argv[1], "switchnetworkprofile", 20) == 0)
        {
            switch_networkprofile(argc, argv);
        }
        else if(strncmp(argv[1], "deletenetworkprofile", 20) == 0)
        {
            delete_networkprofile(argc, argv);
        }
        else if(strncmp(argv[1], "createibss", 10) == 0)
        {
            create_ibss(argc, argv);
        }
        else if(strncmp(argv[1], "setip", 5) == 0)
        {
            set_ip(argc, argv);
        }
        else if(strncmp(argv[1], "setdhcp", 7) == 0)
        {
            set_dhcp(argc, argv);
        }
        else if(strncmp(argv[1], "setdns", 6) == 0)
        {
            set_dns(argc, argv);
        }
        else
        {
            printf("unknown command ...\n");
            printf("execute qaulhelper without arguments to see help instructions.\n");
        }
    }
    else
    {
        printf("\n");
        printf("qaulhelper executes helper functions for qaul.net\n\n");
        printf("usage:\n");
        printf("  qaulhelper startolsrd <RESOURCEPATH> <INTERFACE>\n");
        printf("  qaulhelper startolsrd /Applications/qaul.app/Contents/Resources en1\n");
        printf("  qaulhelper stopolsrd\n");
        printf("  qaulhelper startportforwarding <INTERFACE>\n");
        printf("  qaulhelper startportforwarding en1\n");
        printf("  qaulhelper stopportforwarding\n");
        printf("  qaulhelper enablewifi <OSXVERSION> <INTERFACE>\n");
        printf("  qaulhelper enablewifi 1038 en1\n");
        printf("  qaulhelper disablewifi <OSXVERSION> <INTERFACE>\n");
        printf("  qaulhelper disablewifi 1038 en1\n");
        printf("  qaulhelper createnetworkprofile <POFILENAME>\n");
        printf("  qaulhelper createnetworkprofile qaul.net\n");
        printf("  qaulhelper switchnetworkprofile <POFILENAME>\n");
        printf("  qaulhelper switchnetworkprofile qaul.net\n");
        printf("  qaulhelper deletenetworkprofile <POFILENAME>\n");
        printf("  qaulhelper deletenetworkprofile qaul.net\n");
        printf("  qaulhelper setip <SERVICENAME> <IP> <SUBNET> <ROUTER>\n");
        printf("  qaulhelper setip Wi-Fi 10.213.28.55 255.0.0.0 0.0.0.0\n");
        printf("  qaulhelper setdhcp <SERVICENAME> <INTERFACE>\n");
        printf("  qaulhelper setdhcp Wi-Fi en1\n");
        printf("  qaulhelper setdns <SERVICENAME>\n");
        printf("  qaulhelper setdns Wi-Fi\n");
        printf("\n");
        printf("only for OSX <= 10.5\n");
        printf("  qaulhelper createibss <ESSID> <CHANNEL>\n");
        printf("  qaulhelper createibss qaul.net 11\n");
        printf("\n");
    }
    
    return 0;
}


int start_olsrd (int argc, const char * argv[])
{
    char s[512];
    printf("start olsrd\n");
    
    if(argc >= 4)
    {
        // validate arguments
        if (validate_path(argv[2]) == 0)
        {
            printf("argument 1 not valid\n");
            return 0;
        }
        if (validate_interface(argv[3]) == 0)
        {
            printf("argument 2 not valid\n");
            return 0;
        }
                
        // become root
        setuid(0);
        
        // start olsrd
        sprintf(s,"/usr/local/qaul/olsrd -f %s/olsrd_osx.conf -i %s -d 0 -nofork &", argv[2], argv[3]);
        printf("%s\n",s);
        system(s);
            
        printf("olsrd started\n");
    }
    else
        printf("missing argument\n");

	return 0;
}

int stop_olsrd (int argc, const char * argv[])
{
    printf("stop olsrd\n");
    
    // become root
    setuid(0);
    
    // kill olsrd
    system("killall olsrd");
    
    printf("olsrd stopped\n");
	return 0;
}

int start_portforwarding (int argc, const char * argv[])
{
    char s[512];
    printf("start portforwarding\n");
    
    if(argc >= 3)
    {
        // validate arguments
        if (validate_interface(argv[2]) == 0)
        {
            printf("argument 1 not valid\n");
            return 0;
        }
        
        // become root
        setuid(0);
        
        // forward port 80 by firewall
        sprintf(s,"/sbin/ipfw add 1053 fwd localhost,8081 tcp from any to any 80 in recv %s", argv[2]);
        printf("%s\n",s);
        system(s);

        // start portforwarding of udp ports
        sprintf(s,"/usr/local/qaul/socat UDP4-RECVFROM:53,fork UDP4-SENDTO:localhost:8053 &");
        printf("%s\n",s);
        system(s);
        sprintf(s,"/usr/local/qaul/socat UDP4-RECVFROM:67,fork UDP4-SENDTO:localhost:8067 &");
        printf("%s\n",s);
        system(s);

        printf("portforwarding started\n");
    }
    else
        printf("missing argument\n");
    
    return 0;
}

int stop_portforwarding (int argc, const char * argv[])
{
    printf("stop port forwarding\n");
    
    // become root
    setuid(0);
    
    // remove firewall rules
    system("/sbin/ipfw delete 1053");
    // stop port forwarding
    system("/usr/bin/killall socat");
    
    printf("port forwarding stopped\n");
	return 0;
}

int enable_wifi (int argc, const char * argv[])
{
    char s[512];
    printf("enable wifi\n");
    
    if(argc >= 4)
    {
        // validate arguments
        if (validate_number(argv[2]) == 0)
        {
            printf("argument 1 not valid\n");
            return 0;
        }
        if (validate_interface(argv[3]) == 0)
        {
            printf("argument 2 not valid\n");
            return 0;
        }
        
        // become root
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
    char s[512];
    printf("disable wifi\n");
    
    if(argc >= 4)
    {
        // validate arguments
        if (validate_number(argv[2]) == 0)
        {
            printf("argument 1 not valid\n");
            return 0;
        }
        if (validate_interface(argv[3]) == 0)
        {
            printf("argument 2 not valid\n");
            return 0;
        }

        // become root
        setuid(0);
        
        // enable wifi
        if(atoi(argv[2]) <= NSAppKitVersionNumber10_5)
            sprintf(s,"/usr/sbin/networksetup -setairportpower off");
        else
            sprintf(s,"/usr/sbin/networksetup -setairportpower \"%s\" off", argv[3]);
        
        printf("%s\n",s);
        system(s);
        
        printf("wifi enabled\n");
    }
    else
        printf("missing argument\n");
    
	return 0;
}

int create_networkprofile (int argc, const char * argv[])
{
    char s[512];
    printf("create network profile\n");
    
    if(argc >= 3)
    {
        // validate arguments
        if (validate_service(argv[2]) == 0)
        {
            printf("argument 1 not valid\n");
            return 0;
        }
        
        // become root
        setuid(0);
        
        // create network profile
        sprintf(s,"/usr/sbin/networksetup -createlocation \"%s\" populate", argv[2]);
        printf("%s\n",s);
        system(s);
        
        printf("network profile created\n");
    }
    else
        printf("missing argument\n");
    
	return 0;
}

int switch_networkprofile (int argc, const char * argv[])
{
    char s[512];
    printf("switch network profile\n");
    
    if(argc >= 3)
    {
        // validate arguments
        if (validate_service(argv[2]) == 0)
        {
            printf("argument 1 not valid\n");
            return 0;
        }
        
        // become root
        setuid(0);
        
        // switche network profile
        sprintf(s,"/usr/sbin/networksetup -switchtolocation \"%s\"", argv[2]);
        printf("%s\n",s);
        system(s);
        
        printf("network profile switched\n");
    }
    else
        printf("missing argument\n");
    
	return 0;
}

int delete_networkprofile (int argc, const char * argv[])
{
    char s[512];
    printf("delete network profile\n");
    
    if(argc >= 3)
    {
        // validate arguments
        if (validate_service(argv[2]) == 0)
        {
            printf("argument 1 not valid\n");
            return 0;
        }
        
        // become root
        setuid(0);
        
        // switche network profile
        sprintf(s,"/usr/sbin/networksetup -deletelocation \"%s\"", argv[2]);
        printf("%s\n",s);
        system(s);
        
        printf("network profile deleted\n");
    }
    else
        printf("missing argument\n");
    
	return 0;
}

int create_ibss (int argc, const char * argv[])
{
    char s[512];
    printf("create or join ibss\n");
    
    if(argc >= 4)
    {
        // validate arguments
        if (validate_interface(argv[2]) == 0)
        {
            printf("argument 1 not valid\n");
            return 0;
        }
        if (validate_essid(argv[3]) == 0)
        {
            printf("argument 2 not valid\n");
            return 0;
        }
        
        // become root
        setuid(0);
        
        // create or join ibss
        sprintf(s,"/System/Library/PrivateFrameworks/Apple80211.framework/Versions/Current/Resources/airport -i%s -c%s", argv[2], argv[3]);
        printf("%s\n",s);
        system(s);
        
        printf("ibss created\n");
    }
    else
        printf("missing argument\n");
    
	return 0;
}

int set_ip (int argc, const char * argv[])
{
    char s[512];    
    printf("configure ip\n");
    
    if(argc >= 6)
    {
        // validate arguments
        if (validate_ip(argv[2]) == 0)
        {
            printf("argument 1 not valid\n");
            return 0;
        }
        if (validate_ip(argv[3]) == 0)
        {
            printf("argument 2 not valid\n");
            return 0;
        }
        if (validate_ip(argv[4]) == 0)
        {
            printf("argument 3 not valid\n");
            return 0;
        }
        if (validate_ip(argv[5]) == 0)
        {
            printf("argument 4 not valid\n");
            return 0;
        }
        
        // become root
        setuid(0);
        
        // set ip manually
        sprintf(s,"/usr/sbin/networksetup -setmanual %s %s %s %s", argv[2], argv[3], argv[4], argv[5]);
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
    char s[512];
    printf("set DHCP\n");
    
    if(argc >= 3)
    {
        // validate arguments
        if (validate_interface(argv[2]) == 0)
        {
            printf("argument 1 not valid\n");
            return 0;
        }
        
        // become root
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

int set_dns (int argc, const char * argv[])
{
    char s[512];
    printf("set DNS\n");
    
    if(argc >= 3)
    {
        // set root rights
        setuid(0);
    
        // set dns
        sprintf(s,"/usr/sbin/networksetup -setdnsservers %s 178.254.31.11 77.67.33.81", argv[2]);
        printf("%s\n",s);
        system(s);
    
        printf("DNS set\n");
    }
    else
        printf("missing argument\n");
    
    return 0;
}

/**
 * validation functions
 */

int validate_ip (const char* str)
{
    int i;
    
    // check length
    if(strlen(str)>15)
    {
        printf("argument too long\n");
        return 0;
    }
    
    // check numbers and dots
    for(i=0; i<strlen(str); i++)
    {
        if(validate_char_number(str[i]) == 0 && strncmp(&str[i], ".", 1) != 0)
        {
            printf("invalid character\n");
            return 0;
        }
    }
    return 1;
}

int validate_interface (const char* str)
{
    int i;
    
    // check length
    if(strlen(str)>20)
    {
        printf("argument too long\n");
        return 0;
    }
    
    // check numbers and dots
    for(i=0; i<strlen(str); i++)
    {
        if(validate_char_number(str[i]) == 0 &&
           validate_char_letter(str[i]) == 0
           )
        {
            printf("invalid character\n");
            return 0;
        }
    }
    return 1;
}

int validate_service (const char* str)
{
    int i;
    
    // check length
    if(strlen(str)>50)
    {
        printf("argument too long\n");
        return 0;
    }
    
    // check numbers and dots
    for(i=0; i<strlen(str); i++)
    {
        if(validate_char_problematic(str[i]) == 0)
        {
            printf("invalid character %d : %c\n", i, str[i]);
            return 0;
        }
    }
    return 1;
}

int validate_number (const char* str)
{
    int i;
    
    // check length
    if(strlen(str)>10)
    {
        printf("argument too long\n");
        return 0;
    }
    
    // check numbers and dots
    for(i=0; i<strlen(str); i++)
    {
        if(validate_char_number(str[i]) == 0)
        {
            printf("invalid character\n");
            return 0;
        }
    }
    return 1;
}

int validate_path (const char* str)
{
    int i;
    
    // check length
    if(strlen(str)>100)
    {
        printf("argument too long\n");
        return 0;
    }
    
    // check numbers and dots
    for(i=0; i<strlen(str); i++)
    {
        if(validate_char_number(str[i]) == 0 &&
           validate_char_letter(str[i]) == 0 &&
           strncmp(&str[i], ".", 1) != 0 &&
           strncmp(&str[i], "-", 1) != 0 &&
           strncmp(&str[i], "_", 1) != 0 &&
           strncmp(&str[i], "/", 1) != 0
           )
        {
            printf("invalid character\n");
            return 0;
        }
    }
    return 1;
}

int validate_essid (const char* str)
{
    int i;
    
    // check length
    if(strlen(str)>50)
    {
        printf("argument too long\n");
        return 0;
    }
    
    // check numbers and dots
    for(i=0; i<strlen(str); i++)
    {
        if(validate_char_number(str[i]) == 0 &&
           validate_char_letter(str[i]) == 0 &&
           strncmp(&str[i], ".", 1) != 0 &&
           strncmp(&str[i], "-", 1) != 0 &&
           strncmp(&str[i], "_", 1) != 0
           )
        {
            printf("invalid character\n");
            return 0;
        }
    }
    return 1;
}

int validate_char_number (char mychar)
{
    if(strncmp(&mychar, "0", 1)==0)
        return 1;
    if(strncmp(&mychar, "1", 1)==0)
        return 1;
    if(strncmp(&mychar, "2", 1)==0)
        return 1;
    if(strncmp(&mychar, "3", 1)==0)
        return 1;
    if(strncmp(&mychar, "4", 1)==0)
        return 1;
    if(strncmp(&mychar, "5", 1)==0)
        return 1;
    if(strncmp(&mychar, "6", 1)==0)
        return 1;
    if(strncmp(&mychar, "7", 1)==0)
        return 1;
    if(strncmp(&mychar, "8", 1)==0)
        return 1;
    if(strncmp(&mychar, "9", 1)==0)
        return 1;
    
    return 0;
}

int validate_char_letter (char mychar)
{
    if(strncmp(&mychar, "a", 1)==0 || strncmp(&mychar, "A", 1)==0)
        return 1;
    if(strncmp(&mychar, "b", 1)==0 || strncmp(&mychar, "B", 1)==0)
        return 1;
    if(strncmp(&mychar, "c", 1)==0 || strncmp(&mychar, "C", 1)==0)
        return 1;
    if(strncmp(&mychar, "d", 1)==0 || strncmp(&mychar, "D", 1)==0)
        return 1;
    if(strncmp(&mychar, "e", 1)==0 || strncmp(&mychar, "E", 1)==0)
        return 1;
    if(strncmp(&mychar, "f", 1)==0 || strncmp(&mychar, "F", 1)==0)
        return 1;
    if(strncmp(&mychar, "g", 1)==0 || strncmp(&mychar, "G", 1)==0)
        return 1;
    if(strncmp(&mychar, "h", 1)==0 || strncmp(&mychar, "H", 1)==0)
        return 1;
    if(strncmp(&mychar, "i", 1)==0 || strncmp(&mychar, "I", 1)==0)
        return 1;
    if(strncmp(&mychar, "j", 1)==0 || strncmp(&mychar, "J", 1)==0)
        return 1;
    if(strncmp(&mychar, "k", 1)==0 || strncmp(&mychar, "K", 1)==0)
        return 1;
    if(strncmp(&mychar, "l", 1)==0 || strncmp(&mychar, "L", 1)==0)
        return 1;
    if(strncmp(&mychar, "m", 1)==0 || strncmp(&mychar, "M", 1)==0)
        return 1;
    if(strncmp(&mychar, "n", 1)==0 || strncmp(&mychar, "N", 1)==0)
        return 1;
    if(strncmp(&mychar, "o", 1)==0 || strncmp(&mychar, "O", 1)==0)
        return 1;
    if(strncmp(&mychar, "p", 1)==0 || strncmp(&mychar, "P", 1)==0)
        return 1;
    if(strncmp(&mychar, "q", 1)==0 || strncmp(&mychar, "Q", 1)==0)
        return 1;
    if(strncmp(&mychar, "r", 1)==0 || strncmp(&mychar, "R", 1)==0)
        return 1;
    if(strncmp(&mychar, "s", 1)==0 || strncmp(&mychar, "S", 1)==0)
        return 1;
    if(strncmp(&mychar, "t", 1)==0 || strncmp(&mychar, "T", 1)==0)
        return 1;
    if(strncmp(&mychar, "u", 1)==0 || strncmp(&mychar, "U", 1)==0)
        return 1;
    if(strncmp(&mychar, "v", 1)==0 || strncmp(&mychar, "V", 1)==0)
        return 1;
    if(strncmp(&mychar, "w", 1)==0 || strncmp(&mychar, "W", 1)==0)
        return 1;
    if(strncmp(&mychar, "x", 1)==0 || strncmp(&mychar, "X", 1)==0)
        return 1;
    if(strncmp(&mychar, "y", 1)==0 || strncmp(&mychar, "Y", 1)==0)
        return 1;
    if(strncmp(&mychar, "z", 1)==0 || strncmp(&mychar, "Z", 1)==0)
        return 1;
    
    return 0;
}

int validate_char_problematic (char mychar)
{
    if(strncmp(&mychar, "\"", 1) == 0)
        return 0;
    if(strncmp(&mychar, "'", 1) == 0)
        return 0;
    if(strncmp(&mychar, "`", 1) == 0)
        return 0;
    if(strncmp(&mychar, ";", 1) == 0)
        return 0;
    if(strncmp(&mychar, "\\", 1) == 0)
        return 0;
    if(strncmp(&mychar, "&", 1) == 0)
        return 0;
    if(strncmp(&mychar, ">", 1) == 0)
        return 0;
    if(strncmp(&mychar, "<", 1) == 0)
        return 0;
    if(strncmp(&mychar, "|", 1) == 0)
        return 0;
    if(strncmp(&mychar, "$", 1) == 0)
        return 0;
    if(strncmp(&mychar, "*", 1) == 0)
        return 0;
    if(strncmp(&mychar, "%", 1) == 0)
        return 0;
    if(strncmp(&mychar, "?", 1) == 0)
        return 0;
    if(strncmp(&mychar, "!", 1) == 0)
        return 0;
    if(strncmp(&mychar, "#", 1) == 0)
        return 0;
    if(strncmp(&mychar, "~", 1) == 0)
        return 0;
    if(strncmp(&mychar, "=", 1) == 0)
        return 0;
    if(strncmp(&mychar, "(", 1) == 0)
        return 0;
    if(strncmp(&mychar, "[", 1) == 0)
        return 0;
    if(strncmp(&mychar, "{", 1) == 0)
        return 0;
    if(strncmp(&mychar, "^", 1) == 0)
        return 0;
    
    return 1;
}
