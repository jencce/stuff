#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <signal.h>
#include <security/pam_appl.h>
#include <security/pam_misc.h>

struct sigaction sa;
struct sigaction acs[8];
const char *sigs[10] = {"SIGALRM", "SIGINT", "SIGHUP", "SIGQUIT", 
			"SIGTERM", "SIGTSTP", "SIGTTIN", "SIGTTOU" };

void voidhandler()
{
	printf("sig reeved\n");
}

void disig()
{
	memset(&sa, 0, sizeof(sa));
	sigemptyset( &(sa.sa_mask));
	sa.sa_flags = SA_INTERRUPT; /* don't restart system calls */
	sa.sa_handler = voidhandler;
	int i = 0;
	for( ; i < 8; i++ )
		sigaction( sigs[i], &sa, &acs[i] );
}

void absig()
{
	int i = 0;
	for( ; i < 8; i++ )
		sigaction( sigs[i], &acs[i], NULL );
}

int * pconv( int nm, const struct pam_message **msg, struct pam_response **resp, void *appdata_ptr )
{
	struct pam_response *pr;	
	const struct pam_message *pm;
	int n;
	char pass[256];

	*resp = malloc( nm * sizeof(struct pam_response) );
	if ( resp == NULL )
		return -1;
	memset( *resp, 0, nm * sizeof(struct pam_response) );
	memset( pass, 0, 256 );

	for( pr = *resp, pm = *msg, n = nm; n--; pr++, pm++ )
	{
		switch( pm->msg_style )
		{
			case PAM_PROMPT_ECHO_ON:
			case PAM_PROMPT_ECHO_OFF:
				printf("Password:");
				//disig();
				struct termios oflag, nflag;
				tcgetattr( fileno(stdin), &oflag );
				nflag = oflag;
				nflag.c_lflag &= ~ECHO;
				nflag.c_lflag |= ECHONL;

				if( tcsetattr( fileno(stdin), TCSANOW, &nflag ) != 0 )
				{
					perror("set term nflag failed\n");
					return -1;
				}

				scanf( "%s", pass );

				if( tcsetattr( fileno(stdin), TCSANOW,  &oflag ) != 0 )
				{
					perror("set term oflag failed\n");
					return -1;
				}
				//absig();
				pr->resp = strdup( pass );
				//printf( "%s\n", pass );
				memset( pass, 0, 256 );
				break;
			case PAM_TEXT_INFO:
				printf("app conv\n");
				break;
			default:
				break;
		}
	}

	return 0;
}

int main()
{
	int ret = 0;
	struct pam_handle_t *pamh = NULL;
	struct pam_conv pamc = {
		pconv,
		NULL
	};

	ret = pam_start( "zxpam", "zx", &pamc, &pamh );

	if ( ret != PAM_SUCCESS )
	{
		perror("start failed");
		return -1;
	}
	
	(void) pam_set_item( pamh, PAM_USER, "zx" );
	//(void) pam_set_item( pamh, PAM_USER, "zxx" );
	
	ret = pam_authenticate( pamh, 0 );
	printf("ret %d\n", ret);
	if ( ret != PAM_SUCCESS )
	{
		//perror("auth failed");
		printf("pamerr:  %s\n", pam_strerror(pamh, ret));
		return -1;
	}
	
	ret = pam_acct_mgmt( pamh, 0 );
	if ( ret != PAM_SUCCESS )
	{
		perror("denied");
		return -1;
	} else 
	{
		printf("pass\n");
	}

	ret = pam_end( pamh, PAM_SUCCESS );

	return ret == PAM_SUCCESS ? 0 : -1;
}
