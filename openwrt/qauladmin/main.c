/*
 * qaul.net is free software
 * licensed under GPL (version 3)
 */

/**
 * qaul admin executes administrative tasks from
 * the web interface.
 * 
 * The path of the installed qauladmin binary is:
 *   /qaul/bin/qauladmin
 * 
 * usage:
 *   qauladmin login <password> <shadowhash>
 *   qauladmin login mypassword \$1\$IxLAgyK8\$k9p3I2IDnER48L8H49lPq/
 *   qauladmin newpasswd <password> <salt>
 *   qauladmin newpasswd newpassword \$1\$IxLAgyK8
 */

#define _XOPEN_SOURCE

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <crypt.h>


/**
 * print usage instruction to stdout.
 */
void print_help ();


/**
 * login
 * 
 * checks authentication for root user.
 * The function prints "success" or "error" to stdout.
 */
void login (int argc, const char * argv[]);

/**
 * encrypt new password
 * 
 * Function new password with salt.
 * The function prints the encrypted password string for the 
 * shadow file to standard out.
 */
void newpasswd (int argc, const char * argv[]);


int main (int argc, const char * argv[])
{
    if (argc > 1)
    {
        if(strncmp(argv[1], "login", 5) == 0)
        {
            login(argc, argv);
        }
        else if(strncmp(argv[1], "newpasswd", 5) == 0)
        {
            newpasswd(argc, argv);
        }
        else
        {
        	printf("unknown argument \n");
        	print_help();
        }
    }
    else
    {
        print_help();
    }

    return 0;
}


void print_help ()
{
	printf("\n");
	printf("qauladmin executes qaul.net web admin tasks\n\n");
	printf("usage:\n");
	printf("  qauladmin login <password> <shadowhash>\n");
	printf("  qauladmin login mypassword \\$1\\$IxLAgyK8\\$k9p3I2IDnER48L8H49lPq/\n");
	printf("  qauladmin newpasswd <newpassword> <salt>\n");
	printf("  qauladmin newpasswd mynewpassword \\$1\\$IxLAgyK8\n");
	printf("\n");
}

void login (int argc, const char * argv[])
{
	char *crypt_hash;
	const char *password, *hash;
	
    if(argc >= 3)
    {
		password = argv[2];
		hash = argv[3];
		
		crypt_hash = crypt(password, hash);
		
		if(strncmp(crypt_hash, hash, strlen(hash)) == 0)
			printf("success");
		else
			printf("error");
	}
    else
        printf("missing argument\n");
}

void newpasswd (int argc, const char * argv[])
{
	char *crypt_hash;
	const char *password, *salt;
	
    if(argc >= 3)
    {
		password = argv[2];
		salt = argv[3];
		
		crypt_hash = crypt(password, salt);
		
		printf("%s", crypt_hash);
	}
    else
        printf("missing argument\n");
}
