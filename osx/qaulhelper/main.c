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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


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
    pid_t pid1;
    int status;
    char s[256];
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
        pid1 = fork();
        if (pid1 < 0)
            printf("fork for pid1 failed\n");
        else if(pid1 == 0)
        {
            sprintf(s,"%s/olsrd_osx.conf", argv[2]);
            execl("/usr/local/qaul/olsrd", "olsrd", "-f", s, "-i", argv[3], "-d", "0", (char*)0);
        }
        else
            waitpid(pid1, &status, 0);
        
        printf("olsrd started\n");
    }
    else
        printf("missing argument\n");

	return 0;
}

int stop_olsrd (int argc, const char * argv[])
{
    pid_t pid1;
    int status;
    printf("stop olsrd\n");
    
    // become root
    setuid(0);

    // kill olsrd
    pid1 = fork();
    if (pid1 < 0)
        printf("fork for pid1 failed\n");
    else if(pid1 == 0)
        execl("/usr/bin/killall", "killall", "olsrd", (char*)0);
    else
        waitpid(pid1, &status, 0);
    
    printf("olsrd stopped\n");
	return 0;
}

int start_portforwarding (int argc, const char * argv[])
{
    pid_t pid1, pid2, pid3;
    int fd;
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
        pid1 = fork();
        if (pid1 < 0)
            printf("fork for pid1 failed\n");
        else if(pid1 == 0)
        {
            // redirect standart output and error to /dev/null
            // the program otherwise often didn't return correctly
            fd = open("/dev/null", O_WRONLY | O_CREAT | O_APPEND);
            dup2(fd, STDOUT_FILENO);
            dup2(fd, STDERR_FILENO);
            close(fd);
            // execute program
            execl("/sbin/ipfw", "ipfw", "add", "1053", "fwd", "localhost,8081", "tcp", "from", "any", "to", "any", "80", "in", "recv", argv[2], (char*)0);
        }
        else
            printf("tcp port 80 forwarded\n");

        // forward udp ports by socat
        pid2 = fork();
        if (pid2 < 0)
            printf("fork for pid1 failed\n");
        else if(pid2 == 0)
        {
            // redirect standart output and error to /dev/null
            // the program otherwise often didn't return correctly
            fd = open("/dev/null", O_WRONLY | O_CREAT | O_APPEND);
            dup2(fd, STDOUT_FILENO);
            dup2(fd, STDERR_FILENO);
            close(fd);
            // execute program
            execl("/usr/local/qaul/socat", "socat", "UDP4-RECVFROM:53,fork", "UDP4-SENDTO:localhost:8053", (char*)0);
        }
        else
            printf("udp port 53 forwarded\n");
        
        pid3 = fork();
        if (pid3 < 0)
            printf("fork for pid1 failed\n");
        else if(pid3 == 0)
        {
            // redirect standart output and error to /dev/null
            // the program otherwise often didn't return correctly
            fd = open("/dev/null", O_WRONLY | O_CREAT | O_APPEND);
            dup2(fd, STDOUT_FILENO);
            dup2(fd, STDERR_FILENO);
            close(fd);
            // execute program
            execl("/usr/local/qaul/socat", "socat", "UDP4-RECVFROM:67,fork", "UDP4-SENDTO:localhost:8067", (char*)0);
        }
        else
            printf("udp port 67 forwarded\n");

        printf("portforwarding started\n");
    }
    else
        printf("missing argument\n");
    
    return 0;
}

int stop_portforwarding (int argc, const char * argv[])
{
    pid_t pid1, pid2;
    int status;
    printf("stop port forwarding\n");
    
    // become root
    setuid(0);

    // remove firewall rules
    pid1 = fork();
    if (pid1 < 0)
        printf("fork for pid1 failed\n");
    else if(pid1 == 0)
        execl("/sbin/ipfw", "ipfw", "delete", "1053", (char*)0);
    else
        printf("tcp port 80 forwarding stopped\n");
    
    // stop port forwarding
    pid2 = fork();
    if (pid2 < 0)
        printf("fork for pid2 failed\n");
    else if(pid2 == 0)
        execl("/usr/bin/killall", "killall", "socat", (char*)0);
    else
        waitpid(pid2, &status, 0);

    printf("port forwarding stopped\n");
	return 0;
}

int enable_wifi (int argc, const char * argv[])
{
    pid_t pid1;
    int status;
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
        pid1 = fork();
        if (pid1 < 0)
            printf("fork for pid1 failed\n");
        else if(pid1 == 0)
        {
            if(atoi(argv[2]) <= NSAppKitVersionNumber10_5)
                execl("/usr/sbin/networksetup", "networksetup", "-setairportpower", "on", (char*)0);
            else
                execl("/usr/sbin/networksetup", "networksetup", "-setairportpower", argv[3], "on", (char*)0);
        }
        else
            waitpid(pid1, &status, 0);
        
        printf("wifi enabled\n");
    }
    else
        printf("missing argument\n");
    
	return 0;
}

int disable_wifi (int argc, const char * argv[])
{
    pid_t pid1;
    int status;
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

        // disable wifi
        pid1 = fork();
        if (pid1 < 0)
            printf("fork for pid1 failed\n");
        else if(pid1 == 0)
        {
            if(atoi(argv[2]) <= NSAppKitVersionNumber10_5)
                execl("/usr/sbin/networksetup", "networksetup", "-setairportpower", "off", (char*)0);
            else
                execl("/usr/sbin/networksetup", "networksetup", "-setairportpower", argv[3], "off", (char*)0);        
        }
        else
            waitpid(pid1, &status, 0);

        printf("wifi disabled\n");
    }
    else
        printf("missing argument\n");
    
	return 0;
}

int create_networkprofile (int argc, const char * argv[])
{
    pid_t pid1;
    int status;
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
        pid1 = fork();
        if (pid1 < 0)
            printf("fork for pid1 failed\n");
        else if(pid1 == 0)
            execl("/usr/sbin/networksetup", "networksetup", "-createlocation", argv[2], "populate", (char*)0);
        else
            waitpid(pid1, &status, 0);
        
        printf("network profile created\n");
    }
    else
        printf("missing argument\n");
    
	return 0;
}

int switch_networkprofile (int argc, const char * argv[])
{
    pid_t pid1;
    int status;
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

        // switch network profile
        pid1 = fork();
        if (pid1 < 0)
            printf("fork for pid1 failed\n");
        else if(pid1 == 0)
            execl("/usr/sbin/networksetup", "networksetup", "-switchtolocation", argv[2], (char*)0);
        else
            waitpid(pid1, &status, 0);
        
        printf("network profile switched\n");
    }
    else
        printf("missing argument\n");
    
	return 0;
}

int delete_networkprofile (int argc, const char * argv[])
{
    pid_t pid1;
    int status;
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

        // switch network profile
        pid1 = fork();
        if (pid1 < 0)
            printf("fork for pid1 failed\n");
        else if(pid1 == 0)
            execl("/usr/sbin/networksetup", "networksetup", "-deletelocation", argv[2], (char*)0);
        else
            waitpid(pid1, &status, 0);

        printf("network profile deleted\n");
    }
    else
        printf("missing argument\n");
    
	return 0;
}

int create_ibss (int argc, const char * argv[])
{
    pid_t pid1;
    int status;
    char s1[256];
    char s2[256];
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
        pid1 = fork();
        if (pid1 < 0)
            printf("fork for pid1 failed\n");
        else if(pid1 == 0)
        {
            sprintf(s1,"-i%s", argv[2]);
            sprintf(s2,"-c%s", argv[3]);
            execl("/System/Library/PrivateFrameworks/Apple80211.framework/Versions/Current/Resources/airport", "airport", s1, s2, (char*)0);
        }
        else
            waitpid(pid1, &status, 0);

        printf("ibss created\n");
    }
    else
        printf("missing argument\n");
    
	return 0;
}

int set_ip (int argc, const char * argv[])
{
    pid_t pid1;
    int status;
    printf("configure ip\n");
    
    if(argc >= 6)
    {
        // validate arguments
        if (validate_service(argv[2]) == 0)
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
        pid1 = fork();
        if (pid1 < 0)
            printf("fork for pid1 failed\n");
        else if(pid1 == 0)
            execl("/usr/sbin/networksetup", "networksetup", "-setmanual", argv[2], argv[3], argv[4], argv[5], (char*)0);
        else
            waitpid(pid1, &status, 0);

        printf("ip configured\n");
    }
    else
        printf("missing argument\n");
    
	return 0;
}

int set_dhcp (int argc, const char * argv[])
{
    pid_t pid1, pid2;
    int status;
    printf("set DHCP\n");
    
    if(argc >= 3)
    {
        // validate arguments
        if (validate_service(argv[2]) == 0)
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

        // set dhcp
        pid1 = fork();
        if (pid1 < 0)
            printf("fork for pid1 failed\n");
        else if(pid1 == 0)
            execl("/usr/sbin/networksetup", "networksetup", "-setdhcp", argv[2], (char*)0);
        else
            waitpid(pid1, &status, 0);
        
        usleep(200000);
        
        pid2 = fork();
        if (pid2 < 0)
            printf("fork for pid2 failed\n");
        else if(pid2 == 0)
            execl("/usr/sbin/ipconfig", "ipconfig", "set", argv[3], "DHCP", (char*)0);
        else
            waitpid(pid2, &status, 0);
        
        printf("DHCP set\n");
    }
    else
        printf("missing argument\n");
    
	return 0;
}

int set_dns (int argc, const char * argv[])
{
    pid_t pid1;
    int status;
    printf("set DNS\n");
    
    if(argc >= 3)
    {
        // validate arguments
        if (validate_service(argv[2]) == 0)
        {
            printf("argument 1 not valid\n");
            return 0;
        }

        // set root rights
        setuid(0);

        // set dns
        pid1 = fork();
        if (pid1 < 0)
            printf("fork for pid1 failed\n");
        else if(pid1 == 0)
            execl("/usr/sbin/networksetup", "networksetup", "-setdnsservers", argv[2], "178.254.31.11", "77.67.33.81", (char*)0);
        else
            waitpid(pid1, &status, 0);
        
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
    if(strlen(str)>200)
    {
        printf("argument too long\n");
        return 0;
    }
    
    // check if it is a valid path
    if(strncmp(&str[0], "/", 1) != 0)
    {
        printf("not an absolute path\n");
        return 0;
    }
    
    for(i=0; i<strlen(str); i++)
    {
        if(
           strncmp(&str[i], ":", 1) == 0
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
