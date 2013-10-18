/*
 *  NSA Security-Enhanced Linux (SELinux) security module
 *
 *  This file contains the SELinux security data structures for kernel objects.
 *
 *  Author(s):  Stephen Smalley, <sds@epoch.ncsc.mil>
 *		Chris Vance, <cvance@nai.com>
 *		Wayne Salamon, <wsalamon@nai.com>
 *		James Morris <jmorris@redhat.com>
 *
 *  Copyright (C) 2001,2002 Networks Associates Technology, Inc.
 *  Copyright (C) 2003 Red Hat, Inc., James Morris <jmorris@redhat.com>
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License version 2,
 *	as published by the Free Software Foundation.
 */
#ifndef _KUXSE_OBJSEC_H_
#define _KUXSE_OBJSEC_H_

#include <linux/list.h>
#include <linux/sched.h>
#include <linux/fs.h>
#include <linux/binfmts.h>
#include <linux/in.h>
#include <linux/spinlock.h>
#include "flask.h"

/* mac_level and intigrety_level resides in task_struct and
 * inode as annotations for access controll */

#define MAC_CAT_MAX 256

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

struct inode_security_struct {
	struct mac_level mlevel;
	struct intigrety_level ilevel;
	int task_sid;		/* SID of creating task */
	int initialized;	/* initialization flag */
	struct mutex lock;
};

struct user_inode_security_struct { /* just for syscall, no mutex */
	struct mac_level mlevel;
	struct intigrety_level ilevel;
	int task_sid;		/* SID of creating task */
	int initialized;	/* initialization flag */
};

#endif
