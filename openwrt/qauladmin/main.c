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
 *   qauladmin login <username>
 *
 */


#include <security/pam_appl.h>
#include <security/pam_misc.h>
#include <stdio.h>
#include <unistd.h>


/**
 * print usage instruction to stdout.
 */
void print_help ();


/**
 * login
 * 
 * checks authentication for root user.
 * The function prints 1 on success and 0 on failure to stdout.
 */
void login (int argc, const char * argv[]);


int main (int argc, const char * argv[])
{
    if (argc > 1)
    {
        if(strncmp(argv[1], "login", 5) == 0)
        {
            login(argc, argv);
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
	printf("  qauladmin login <username>\n");
	printf("\n");
}


void login (int argc, const char * argv[])
{
    if(argc >= 2)
    {
		const char*            user = argv[2];
		static struct pam_conv pam_conversation = { misc_conv, NULL };
		pam_handle_t*          pamh;

		int res = pam_start(argv[0], user, &pam_conversation, &pamh);
		
		if (res == PAM_SUCCESS)
		  res = pam_authenticate(pamh, 0);

		if (res == PAM_SUCCESS)
		  res = pam_acct_mgmt(pamh, 0);

		if (res == PAM_SUCCESS)
		  fprintf(stdout, "success");
		else
		  fprintf(stdout, "error");

		pam_end(pamh, 0);		
    }
    else
        printf("missing argument\n");
}

