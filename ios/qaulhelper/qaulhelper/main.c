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
 *   qaulhelper startolsrd
 *   qaulhelper stopolsrd
 *   qaulhelper setip <IP>
 */


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


/**
 * start olsrd
 */
int start_olsrd (int argc, const char * argv[]);

/**
 * stop olsrd
 */
int stop_olsrd (int argc, const char * argv[]);

/**
 * configure static IP for wifi
 */
int set_ip (int argc, const char * argv[]);

/**
 * check IP argument
 */
int check_ip (const char* ip);

/**
 * check each char of the IP
 */
int check_ip_char (char mychar);



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
        else
            printf("unknown command\n");
    }
    else
    {
        printf("\n");
        printf("qaulhelper contains helper functions for qaul.net\n\n");
        printf("usage:\n");
        printf("  qaulhelper startolsrd\n");
        printf("  qaulhelper stopolsrd\n");
        printf("  qaulhelper setip <IP>\n");
        printf("\n");
    }
    
    return 0;
}


int start_olsrd (int argc, const char * argv[])
{
    printf("start olsrd as root\n");
    
    // retrieve current user id
    printf("orig user: %i\n", getuid());
    
    // set root rights
    setuid(0);
    printf("olsrd user: %i\n", getuid());
    system("/usr/bin/olsrd -i en0 -f /usr/bin/olsrd_ios.conf -d 0");
    
    printf("olsrd started");
	return 0;    
}

int stop_olsrd (int argc, const char * argv[])
{
    printf("stop olsrd\n");
    
    // set root rights
    setuid(0);
    
    // kill olsrd
    system("/usr/bin/killall olsrd");
    
    printf("olsrd stopped");
	return 0;
}

int set_ip (int argc, const char * argv[])
{
    char *s;

    printf("configure ip\n");
    
    if(argc == 3)
    {
        // check argument
        if (check_ip(argv[2]) == 0) {
            printf("argument 1 not valid\n");
            return 0;
        }
        
        // set root rights
        setuid(0);
        
        // set ip manually
        sprintf(s,"/usr/sbin/ipconfig set en0 MANUAL %s 255.0.0.0", argv[2]);
        system(s);
        
        printf("ip configured\n");
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
