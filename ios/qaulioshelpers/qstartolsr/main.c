/*
 * qaul.net is free software
 * licensed under GPL (version 3)
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main (int argc, const char * argv[])
{
    // insert code here...
    printf("start olsrd as root\n");
    
    // retrieve current user id
    printf("orig user: %i\n", getuid());
    
    // set root rights
    setuid(0);
    printf("olsrd user: %i\n", getuid());
    //system("/usr/bin/olsrd -i en0 -f /usr/bin/olsrd_ios.conf -d 2");
    system("/usr/bin/olsrd -i en0 -f /usr/bin/olsrd_ios.conf -d 0");
    
    printf("olsrd started");
	return 0;
}

