/*
 *  KUX security enhance(KSE) header file
 *
 *  This file contains the KSE header files.
 *
 *  Authors:  Zhou Xiong, <zhouxiong@inspur.com>
 *
 *  Copyright (C) 2013,2014 Inspur, inc
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
#include <linux/lsm_audit.h>
#include "flask.h"
#include "permissions.h"

/* mac_level and integrity_level resides in task_struct and
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
struct integrity_level {
	int level_value;
};

struct task_security_struct {
	struct mac_level mlevel;
	struct integrity_level ilevel;
};

struct inode_security_struct {
	struct mac_level mlevel;
	struct integrity_level ilevel;
	int task_sid;		/* SID of creating task */
	int initialized;	/* initialization flag */
	struct mutex lock;
};

struct user_inode_security_struct { /* just for syscall, no mutex */
	struct mac_level mlevel;
	struct integrity_level ilevel;
	int task_sid;		/* SID of creating task */
	int initialized;	/* initialization flag */
};

extern void kse_audit(struct task_security_struct *,
		struct task_security_struct *,
		struct inode_security_struct *,
		int, int, int,int, struct common_audit_data *);
int iss_to_context(int init, struct mac_level *mlevel,
			struct integrity_level *ilevel,
			char **scontext, u32 *scontext_len);
int context_to_iss(struct mac_level *mlevel, struct integrity_level *ilevel,
			const void **scontext, ssize_t *scontext_len);
extern int kse_enabled;
extern int kse_noyaudit;
int task_inode_perm(struct task_struct *tsk1, struct inode *inode,
				struct dentry *dentry, int perm);
int task_task_perm(struct task_struct *tsk1,
				struct task_struct *tsk2, int perm);
#endif
