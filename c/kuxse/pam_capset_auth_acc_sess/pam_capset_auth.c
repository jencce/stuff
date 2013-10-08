/* pam_capset_auth module 
 * writen by levysong <songlw@inspur.com> 2013/09/26
 */

#define DEFAULT_USER "nobody"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/capability.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <syslog.h>


#define PAM_SM_AUTH
#define PAM_SM_ACCOUNT
#define PAM_SM_SESSION
#define PAM_SM_PASSWORD

#include <security/pam_modules.h>
#include <security/_pam_macros.h>
#include <security/pam_ext.h>
#include <security/pam_modutil.h>

/* --- authentication management functions --- */


#define MAC_CAT_MAX 256
#define __NR_mac_task_ctl 349

/* Mandatory access controll struct  */
struct mac_level {
	int level_type;    /* type */
	int level_value;
	int level_flag;
	int level_catsum;
	int level_category[MAC_CAT_MAX];
};

/* Intigrety level struct */
struct intigrety_level {
	int level_value;
};

struct task_security_struct {
	struct mac_level mlevel;
	struct intigrety_level ilevel;
};

void init_tss(struct task_security_struct *tss)
{
	int i;
	tss->mlevel.level_type = 0;
	tss->mlevel.level_value = 0;
	tss->mlevel.level_catsum = 0;
	tss->mlevel.level_flag = 0;
	for (i = 0; i < MAC_CAT_MAX; i++)
		tss->mlevel.level_category[i] = 0;

	tss->ilevel.level_value = 0;
}

void alter_mil(pam_handle_t *pamh)
{
	int i = 0;
	struct task_security_struct tss1;
	struct task_security_struct tss2;

	init_tss(&tss1);
	init_tss(&tss2);

	i = syscall(__NR_mac_task_ctl, 0, &tss2);
	if (i < 0) {
		pam_syslog(pamh, LOG_INFO, "get task sec1 fail\n");
	} else {
		pam_syslog(pamh, LOG_INFO, "old tsk sec: mv %d, iv %d\n", tss2.mlevel.level_value,
			tss2.ilevel.level_value);
	}

	tss1.mlevel.level_type = 0;
	tss1.mlevel.level_value = 4;
	tss1.mlevel.level_catsum = 0;
	tss1.mlevel.level_flag = 1;
	for (i = 0; i < MAC_CAT_MAX; i++)
		tss1.mlevel.level_category[i] = 0;

	tss1.ilevel.level_value = 3;
	
	syscall(__NR_mac_task_ctl, 1, &tss1);

	init_tss(&tss2);
	i = syscall(__NR_mac_task_ctl, 0, &tss2);
	if (i < 0) {
		pam_syslog(pamh, LOG_INFO, "get task sec2 fail\n");
	} else {
		pam_syslog(pamh, LOG_INFO, "new tsk sec: mv %d, iv %d\n", tss2.mlevel.level_value,
			tss2.ilevel.level_value);
	}
}

PAM_EXTERN int
pam_sm_authenticate(pam_handle_t *pamh, int flags ,
		    int argc , const char **argv )
{
	int retval;
	const char *user=NULL;

	/*
	 * authentication requires we know who the user wants to be
	 */
	retval = pam_get_item(pamh, PAM_USER, (void*)&user);
	if (user == NULL || *user == '\0' || retval != PAM_SUCCESS) {
		pam_syslog(pamh, LOG_INFO, "username not known\n");
		return PAM_AUTH_ERR;
	}
	pam_syslog(pamh, LOG_INFO, "user: %s\n", user);

	if (strcmp(user, "zx") == 0)
		alter_mil(pamh);

	return PAM_SUCCESS;
}

PAM_EXTERN int
pam_sm_setcred(pam_handle_t *pamh , int flags ,
	       int argc , const char **argv )
{
     return PAM_SUCCESS;
}

/* --- account management functions --- */

PAM_EXTERN int
pam_sm_acct_mgmt(pam_handle_t *pamh , int flags ,
		 int argc , const char **argv )
{
     return PAM_SUCCESS;
}

/* --- password management --- */

PAM_EXTERN int
pam_sm_chauthtok(pam_handle_t *pamh , int flags ,
		 int argc , const char **argv )
{
     return PAM_SUCCESS;
}

/* --- session management --- */

PAM_EXTERN int
pam_sm_open_session(pam_handle_t *pamh , int flags ,
		    int argc , const char **argv )
{
    return PAM_SUCCESS;
}

PAM_EXTERN int
pam_sm_close_session(pam_handle_t *pamh , int flags ,
		     int argc, const char **argv )
{
     return PAM_SUCCESS;
}

/* end of module definition */

#ifdef PAM_STATIC

/* static module data */

struct pam_module _pam_permit_modstruct = {
    "pam_capset_auth",
    pam_sm_authenticate,
    pam_sm_setcred,
    pam_sm_acct_mgmt,
    pam_sm_open_session,
    pam_sm_close_session,
    pam_sm_chauthtok
};

#endif
