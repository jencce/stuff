/*
 *  KUX security enhance(KSE) module
 *
 *  This file contains the KSE hook function implementations.
 *
 *  Authors:  Zhou Xiong, <zhouxiong@inspur.com>
 *
 *  Copyright (C) 2013,2014 Inspur, inc
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License version 2,
 *	as published by the Free Software Foundation.
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/tracehook.h>
#include <linux/errno.h>
#include <linux/sched.h>
#include <linux/security.h>
#include <linux/xattr.h>
#include <linux/capability.h>
#include <linux/unistd.h>
#include <linux/mm.h>
#include <linux/mman.h>
#include <linux/slab.h>
#include <linux/pagemap.h>
#include <linux/swap.h>
#include <linux/spinlock.h>
#include <linux/syscalls.h>
#include <linux/file.h>
#include <linux/fdtable.h>
#include <linux/namei.h>
#include <linux/mount.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/bitops.h>
#include <linux/interrupt.h>
#include <linux/parser.h>
#include <linux/nfs_mount.h>
#include <linux/personality.h>
#include <linux/sysctl.h>
#include <linux/audit.h>
#include <linux/string.h>
#include <linux/mutex.h>
#include <linux/posix-timers.h>
#include <linux/lsm_audit.h>
#include <linux/cred.h>
#include <linux/ctype.h>

#include "kseobj.h"
#include "flask.h"
#include "permissions.h"
//#include "security.h"

#define XATTR_KSE_SUFFIX "kse"
#define XATTR_NAME_KSE XATTR_SECURITY_PREFIX XATTR_KSE_SUFFIX

#ifdef CONFIG_SECURITY_KSE_BOOTPARAM
int kse_enabled = CONFIG_SECURITY_KSE_BOOTPARAM_VALUE;
#else
int kse_enabled = 1;
#endif

static int __init kse_enabled_setup(char *str)
{
	unsigned long enabled;
	if (!strict_strtoul(str, 0, &enabled))
		kse_enabled = enabled ? 1 : 0;
	return 1;
}
__setup("kse=", kse_enabled_setup);

int kse_noyaudit = 1;

static int __init kse_noyaudit_setup(char *str)
{
	unsigned long noyaudit;
	if (!strict_strtoul(str, 0, &noyaudit))
		kse_noyaudit = noyaudit ? 1 : 0;
	return 1;
}
__setup("ksenoyaudit=", kse_noyaudit_setup);

/*
 * category_contain: compare two categories, return results
 * cate1: category array 1
 * cate2: category array 2
 * return value:
 * 1: cate1 contains cate2
 * 0: cate1 does not contain cate2
 */
static int category_contain(int *cate1, int *cate2)
{
	int i;
	for (i = 0; i < MAC_CAT_MAX; i++) {
		if (cate2[i] == 1 && cate1[i] == 0)
			return 0;
	}
	return 1;
}

/*
 * kse_mac_compare: compare two mac_level, return results.
 * sml: source mac_level
 * tml: target mac_level
 * return value:
 * -1: target dominate source
 *  0: no mac check
 *  1: source dominate target
 * -2: invalid 
 */
static int kse_mac_compare(struct mac_level *sml, struct mac_level *tml)
{
	if (sml->level_type == MAC_TYPE_UNDEF ||
		tml->level_type == MAC_TYPE_UNDEF)
		return 0;

	if (sml->level_type == MAC_TYPE_HIGH ||
		tml->level_type == MAC_TYPE_LOW)
		return 1;

#if 0
	if (tml->level_type == MAC_TYPE_HIGH ||
		sml->level_type == MAC_TYPE_LOW)
		return -1;
#endif

	if (sml->level_type == MAC_TYPE_PUBLIC ||
		tml->level_type == MAC_TYPE_PUBLIC)
		return 1;

	if (sml->level_type == MAC_TYPE_GEN &&
		tml->level_type == MAC_TYPE_GEN &&
		sml->level_value >= tml->level_value &&
		//sml->level_catsum == tml->level_catsum &&
		category_contain(sml->level_category, tml->level_category))
		return 1;

	return -2;
}

/* 
 * kse_integrity_compare: compare two integrity level
 * sil: source integrity level
 * til: target integrity level
 * return value:
 * -1: target dominate source
 *  0: dominate vice versa
 *  1: source dominate target
 * -2: invalid
 */
static int kse_integrity_compare(struct integrity_level *sil,
					struct integrity_level *til)
{
	if (sil->level_value == til->level_value)
		return 0;
	if (sil->level_value > til->level_value)
		return 1;
	if (sil->level_value < til->level_value)
		return -1;
	
	return -2;
}

/*
 * kse_mac_may_read: compare two mac_level, return result indicate wheather
 * this read operation can be granted.
 * sml: source mac_level
 * tml: target mac_level
 * return value:
 * -1: can not read
 *  0: can read
 */
static int kse_mac_may_read(struct mac_level *sml, struct mac_level *tml)
{
	if (kse_mac_compare(sml, tml) == 0 || kse_mac_compare(sml, tml) == 1)
		return 0;
	else
		return -1;
}

/*
 * kse_mac_may_write: compare two mac_level, return result indicate wheather
 * this write operation can be granted.
 * sml: source mac_level
 * tml: target mac_level
 * return value:
 * -1: can not write
 *  0: can write
 */
static int kse_mac_may_write(struct mac_level *sml, struct mac_level *tml)
{
	if (kse_mac_compare(sml, tml) == 0 || kse_mac_compare(tml, sml) == 1)
		return 0;
	else
		return -1;
}

/*
 * kse_integrity_may_read: compare two integrity_level, return result
 * indicates wheather this read operation can be granted.
 * sil: source integrity_level
 * til: target integrity_level
 * return value:
 * -1: can not read
 *  0: can read
 */
static int kse_integrity_may_read(struct integrity_level *sil,
					struct integrity_level *til)
{
	if (kse_integrity_compare(sil, til) == -1 ||
		kse_integrity_compare(sil, til) == 0)
		return 0;
	else
		return -1;
}

/*
 * kse_integrity_may_write: compare two integrity_level, return result
 * indicate wheather this write operation can be granted.
 * sil: source integrity_level
 * til: target integrity_level
 * return value:
 * -1: can not write
 *  0: can write
 */
static int kse_integrity_may_write(struct integrity_level *sil,
					struct integrity_level *til)
{
	if (kse_integrity_compare(sil, til) == 1 ||
		kse_integrity_compare(sil, til) == 0)
		return 0;
	else
		return -1;
}

/*
 * kse_may_read: compare two task/inode or task/task level, return result
 * indicates wheather this read operation can be granted. Both mac and
 * integrity level are compared.
 * tsk1: source task
 * tsk2: target task
 * inode: target inode
 * flag: 0 tsk2 valid, 1 inode valid
 * return value:
 * -1: can not read
 *  0: can read
 */
static int kse_may_read(struct task_security_struct *tsp1,
			struct task_security_struct *tsp2,
			struct inode_security_struct *isp, int flag)
{
	if (tsp1 == NULL)
		return -1;

	if (flag == 0) { /* task to task */
		if (tsp2 == NULL)
			return -1;

		/* UNDEF no check  */
		if (tsp1->mlevel.level_type == MAC_TYPE_UNDEF ||
			tsp2->mlevel.level_type == MAC_TYPE_UNDEF)
			return 0;

		if (kse_mac_may_read(&tsp1->mlevel, &tsp2->mlevel) == 0 &&
		    kse_integrity_may_read(&tsp1->ilevel, &tsp2->ilevel) == 0)
			return 0;
		else
			return -1;
	}

	if (flag == 1) { /* task to inode */
		if (isp == NULL)
			return -1;

		/* UNDEF no check  */
		if (tsp1->mlevel.level_type == MAC_TYPE_UNDEF ||
			isp->mlevel.level_type == MAC_TYPE_UNDEF)
			return 0;

		if (kse_mac_may_read(&tsp1->mlevel, &isp->mlevel) == 0 &&
		    kse_integrity_may_read(&tsp1->ilevel, &isp->ilevel) == 0)
			return 0;
		else
			return -1;
	}
	return -1;
}

/* same like above */
static int kse_may_write(struct task_security_struct *tsp1,
			struct task_security_struct *tsp2,
			struct inode_security_struct *isp, int flag)
{
	if (tsp1 == NULL)
		return -1;

	if (flag == 0) { /* task to task */
		if (tsp2 == NULL)
			return -1;

		/* UNDEF no check  */
		if (tsp1->mlevel.level_type == MAC_TYPE_UNDEF ||
			tsp2->mlevel.level_type == MAC_TYPE_UNDEF)
			return 0;

		if (kse_mac_may_write(&tsp1->mlevel, &tsp2->mlevel) == 0 &&
		    kse_integrity_may_write(&tsp1->ilevel, &tsp2->ilevel) == 0)
			return 0;
		else
			return -1;
	}

	if (flag == 1) { /* task to inode */
		if (isp == NULL)
			return -1;

		/* UNDEF no check  */
		if (tsp1->mlevel.level_type == MAC_TYPE_UNDEF ||
			isp->mlevel.level_type == MAC_TYPE_UNDEF)
			return 0;

		if (kse_mac_may_write(&tsp1->mlevel, &isp->mlevel) == 0 &&
		    kse_integrity_may_write(&tsp1->ilevel, &isp->ilevel) == 0)
			return 0;
		else
			return -1;
	}
	return -1;
}

/* 
 * task_task_perm: check permission when task operate on task
 * tsk1: source task
 * tsk2: target task
 * perm: permission to be checked
 * return value:
 *  0: granted
 * -1: denied
 * */
int task_task_perm(struct task_struct *tsk1,
				struct task_struct *tsk2, int perm)
{
	struct task_security_struct *tsp1;
	struct task_security_struct *tsp2;
	struct common_audit_data ad;
	int ret;
	int request = 0;

	if (kse_enabled == 0)
		return 0;
	if (tsk1 == NULL || tsk2 == NULL)
		return -1;

	tsp1 = (struct task_security_struct *)task_cred_xxx(tsk1, security);
	tsp2 = (struct task_security_struct *)task_cred_xxx(tsk2, security);

	if (tsp1 == NULL || tsp2 == NULL)
		return -1;

	COMMON_AUDIT_DATA_INIT(&ad, TASK);

	switch (perm) {
	case PROCESS__GETPGID:
		ret = kse_may_read(tsp1, tsp2, NULL, 0);
		break;
	case PROCESS__SETPGID:
		ret = kse_may_write(tsp1, tsp2, NULL, 0);
		break;
	case PROCESS__GETSESSION:
		ret = kse_may_read(tsp1, tsp2, NULL, 0);
		break;
	case PROCESS__GETATTR:
		ret = kse_may_read(tsp1, tsp2, NULL, 0);
		break;
	case PROCESS__GETSCHED:
		ret = kse_may_read(tsp1, tsp2, NULL, 0);
		break;
	case PROCESS__SETSCHED:
		ret = kse_may_write(tsp1, tsp2, NULL, 0);
		break;
	case PROCESS__SETRLIMIT:
		ret = kse_may_write(tsp1, tsp2, NULL, 0);
		break;
	case PROCESS__SIGNAL:
		ret = kse_may_write(tsp1, tsp2, NULL, 0);
		break;
	case PROCESS__SIGCHLD:
		ret = kse_may_write(tsp1, tsp2, NULL, 0);
		break;
	default:
		ret = -1;
		break;
	}
	request = perm;

	if (kse_noyaudit == 1 && ret == 0)
		return ret;

	kse_audit(tsp1, tsp2, NULL, 0, SECCLASS_PROCESS, request, ret, &ad);
	return ret;
}

/* 
 * task_inode_perm: check permission when task operate on inode
 * tsk1: source task
 * inode: target inode
 * perm: permission to be checked
 * return value:
 *  0: granted
 * -1: denied
 * */
int task_inode_perm(struct task_struct *tsk1, struct inode *inode,
				struct dentry *dentry, int perm)
{
	struct task_security_struct *tsp1;
	struct inode_security_struct *iss;
	struct common_audit_data ad;
	int ret;
	int request = 0;

	if (kse_enabled == 0)
		return 0;

	tsp1 = (struct task_security_struct *)task_cred_xxx(tsk1, security);
	iss = inode->i_security;

	COMMON_AUDIT_DATA_INIT(&ad, FS);
	ad.u.fs.path.dentry = dentry;
	ad.u.fs.inode = inode;

	switch (perm) {
	case DIR__ADD_NAME:
		ret = kse_may_write(tsp1, NULL, iss, 1);
		break;
	case DIR__REMOVE_NAME:
		ret = kse_may_write(tsp1, NULL, iss, 1);
		break;
	case FILE__CREATE:
		ret = kse_may_write(tsp1, NULL, iss, 1);
		break;
	case FILE__LINK:
		ret = kse_may_write(tsp1, NULL, iss, 1);
		break;
	case FILE__UNLINK:
		ret = kse_may_write(tsp1, NULL, iss, 1);
		break;
	case FILE__READ:
		ret = kse_may_read(tsp1, NULL, iss, 1);
		break;
	case FILE__WRITE:
		ret = kse_may_write(tsp1, NULL, iss, 1);
		break;
	case FILE__APPEND:
		ret = kse_may_write(tsp1, NULL, iss, 1);
		break;
	case FILE__GETATTR:
		ret = kse_may_read(tsp1, NULL, iss, 1);
		break;
	case FILE__SETATTR:
		ret = kse_may_write(tsp1, NULL, iss, 1);
		break;
	case FILE__EXECUTE:
		ret = kse_may_read(tsp1, NULL, iss, 1);
		break;
	default:
		ret = -1;
		break;
	}
	request |= perm;

	if (kse_noyaudit == 1 && ret == 0)
		return ret;

	kse_audit(tsp1, NULL, iss, 1, SECCLASS_FILE, request, ret, &ad);
	return ret;
}

static int init_mlevel(struct mac_level *mlevel)
{
	mlevel->level_type = MAC_TYPE_UNDEF;
	mlevel->level_value = 0;
	mlevel->level_flag = 0;
	mlevel->level_catsum = 0;

	return 0;
}

static void copy_mlevel(struct mac_level *mldst, const struct mac_level *mlsrc)
{
	int i;
	mldst->level_type = mlsrc->level_type;
	mldst->level_value = mlsrc->level_value;
	mldst->level_flag = mlsrc->level_flag;
	mldst->level_catsum = mlsrc->level_catsum;

	for (i = 0; i < MAC_CAT_MAX; i++)
		mldst->level_category[i] = mlsrc->level_category[i];
}

/*
 * initialise the security for the init task
 */
static void cred_init_security(void)
{
	struct cred *cred = (struct cred *) current->real_cred;
	struct task_security_struct *tsec;
	int ret = 0;

	tsec = kzalloc(sizeof(struct task_security_struct), GFP_KERNEL);
	if (!tsec)
		panic("KUXSE:  Failed to initialize initial task.\n");

	ret = init_mlevel(&tsec->mlevel);
	if (ret)
		panic("KUXSE:  Failed to initialize initial task.\n");

	tsec->ilevel.level_value = 0;
	cred->security = tsec;
}

static struct kmem_cache *kse_inode_cache;

static int inode_alloc_security(struct inode *inode)
{
	struct inode_security_struct *isec;
	u32 sid = current_uid();
	int ret = 0;

	isec = kmem_cache_zalloc(kse_inode_cache, GFP_NOFS);
	if (!isec)
		return -ENOMEM;

	mutex_init(&isec->lock);
	isec->task_sid = sid;
	ret = init_mlevel(&isec->mlevel);
	if (ret)
		return -ENOMEM;

	isec->ilevel.level_value = 0;

	inode->i_security = isec;

	return 0;
}

static void inode_free_security(struct inode *inode)
{
	struct inode_security_struct *isec = inode->i_security;

	inode->i_security = NULL;
	kmem_cache_free(kse_inode_cache, isec);
}

/* inode security operations */

static int kse_inode_alloc_security(struct inode *inode)
{
	return inode_alloc_security(inode);
}

static void kse_inode_free_security(struct inode *inode)
{
	inode_free_security(inode);
}

/* init inode security attrs and context string,
 * according to attrs of current task
 * i_security to xattr string
 */
int iss_to_context(int init, struct mac_level *mlevel,
			struct integrity_level *ilevel,
			char **scontext, u32 *scontext_len)
{
	const struct task_security_struct *tss = current_security();
	int i = 0;
	char *cate = NULL;
	char *cp = NULL;
	char temp[5];

	if (mlevel == NULL || ilevel == NULL)
		return -EINVAL;

	*scontext = NULL;
	*scontext_len  = 0;

#if 1
	if (strncmp(current->comm, "vim", 3) == 0) {
		pr_debug("itcs pid %d\n", current->pid);
		dump_stack();
		pr_debug("itce pid %d\n", current->pid);
	}
#endif

	if (init && tss) {
		copy_mlevel(mlevel, &tss->mlevel);
		ilevel->level_value = tss->ilevel.level_value;

		pr_debug("itc,csum %d, ml %d, il %d, pid %d\n",
			tss->mlevel.level_catsum, tss->mlevel.level_value,
			tss->ilevel.level_value, current->pid);
		//iss->initialized = 1;
	}

	pr_debug("itc,csum %d, ml %d, il %d, p %d\n", mlevel->level_catsum,
		mlevel->level_value, ilevel->level_value, current->pid);

	if (mlevel->level_catsum) 
		cp = kmalloc(mlevel->level_catsum*3 - 1 + 8, GFP_KERNEL);
	else
		cp = kmalloc(MAC_CAT_MAX*3 - 1 + 8, GFP_KERNEL);

	if (cp == NULL) {
		pr_debug("itc, cp NOMEM csum %d\n", mlevel->level_catsum);
		dump_stack();
		return -ENOMEM;
	}

	if (mlevel->level_catsum) {
		cate = kmalloc(mlevel->level_catsum*3-1, GFP_KERNEL);
		if (cate == NULL) {
			pr_debug("itc, cate NOMEM csum\n");
			dump_stack();
			return -ENOMEM;
		}
		memset(cate, 0, mlevel->level_catsum*3-1);
	}

	for (i = 0; cate && i < MAC_CAT_MAX; i++) {
		if (mlevel->level_category[i] == 1) {
			if (strlen(cate) > 0)
				strcat(cate, ",");
			memset(temp, 0, 5);
			sprintf(temp, "c%d", i);
			strcat(cate, temp);
		}
	}

	/* empty category */
	if (cate == NULL)
		//cate = kstrdup("c0", GFP_KERNEL);
		cate = kstrdup("", GFP_KERNEL);

	sprintf(cp, "%d:%d:%s:%d", mlevel->level_type, mlevel->level_value,
		cate, ilevel->level_value);

	pr_debug("itc %d:%d:%s:%d, pid %d\n", mlevel->level_type,
		mlevel->level_value, cate, ilevel->level_value, current->pid);

	if (cp == NULL)
		cp = kstrdup("2:2:c0:2\0", GFP_KERNEL);

	if (cate) 
		kfree(cate);
	
	*scontext = cp;
	*scontext_len = strlen(*scontext);

	return 0;
}

/* context string to inode->i_security  */
int context_to_iss(struct mac_level *mlevel, struct integrity_level *ilevel,
				const void **scontext, ssize_t *scontext_len)
{
	char *cate = NULL;
	char *p = NULL;
	char *cp = NULL;
	char *tp = NULL;
	char temp[5];
	int i, l, ucnt, ctemp;

	if (mlevel == NULL || ilevel == NULL)
		return -EINVAL;

	if (*scontext_len < 5 || *scontext == NULL)
		return -EINVAL;

#if 1
	if (strncmp(current->comm, "vim", 3) == 0) {
		pr_debug("ctis pid %d\n", current->pid);
		dump_stack();
		pr_debug("ctie pid %d\n", current->pid);
	}
#endif

	p = (char *)*scontext;
	ucnt = 0;
	for (i = 0; p[i] && i < *scontext_len; i++) {
		if (p[i] == ':')
			ucnt++;
	}
	/* number of ':' == 3 */
	if (ucnt != 3)
		return -EINVAL;

	/* first letter, mlevel->level_type */
	ucnt = 1; //letters' counter
	cp = (char *)*scontext;
	if ( cp[0] >= '0' && cp[0] <= '9' )
		mlevel->level_type = cp[0] - '0';
	else
		mlevel->level_type = 0;

	while (*cp != ':') {
		cp++;
		ucnt++;
	}
	cp++;
	ucnt++;

	/* second letter, mlevel->level_value */
	if ( *cp >= '0' && *cp <= '9' )
		mlevel->level_value = cp[0] - '0';
	else
		mlevel->level_value = 0;

	while (*cp != ':') {
		cp++;
		ucnt++;
	}
	cp++;
	ucnt++;

	/* third field, mlevel categories, like c0,c1 */
	cate = kmalloc(*scontext_len, GFP_KERNEL);
	if (cate == NULL)
		return -ENOMEM;
	memset(cate, 0, (int)*scontext_len);
	memset(temp, 0, 5);

	p = cate;
	while (*cp != ':') {
		*p++ = *cp++;
		ucnt++;
	}
	cp++;
	ucnt++;

	if (ucnt > *scontext_len)
		return -EINVAL;

	/* fourth field, ilevel value */
	ctemp = 0;
	if (ucnt == *scontext_len) {
		if (isdigit(cp[0]))
			ctemp = cp[0] - '0';
		else
			return -EINVAL;
	} else if ((ucnt + 1) == *scontext_len) {
		if (isdigit(cp[0]) && isdigit(cp[1]))
			ctemp = (cp[0] - '0')*10 + cp[1] - '0';
		else
			return -EINVAL;
	} else
		return -EINVAL;

	if (ctemp >=0 && ctemp <=15)
		ilevel->level_value = ctemp;
	else
		return -EINVAL;

	pr_debug("cti scon:%s, %d:%d:%s:%d, pid %d\n", (char *)*scontext,
			mlevel->level_type, mlevel->level_value,
			cate, ilevel->level_value, current->pid);

	for (i = 0; i < MAC_CAT_MAX; i++)
		mlevel->level_category[i] = 0;

	p = cate;
	tp = cate;
	l = strlen(cate);
	ctemp = 0;
	ucnt = 0;
	
	for (i = 0; i < l; i++) {
		if (cate[i] == ',') {
			tp = &cate[i-1];
			ucnt = tp - p + 1;
			memset(temp, 0, 5);
			strncpy(temp, p, ucnt);
			sscanf(temp, "c%d", &ctemp);
			mlevel->level_category[ctemp] = 1;
			p = &cate[i+1];
			continue;
		}
		if (i == l - 1 && cate[i] != ',') {
			tp = &cate[i];
			ucnt = tp - p + 1;
			memset(temp, 0, 5);
			strncpy(temp, p, ucnt);
			sscanf(temp, "c%d", &ctemp);
			mlevel->level_category[ctemp] = 1;
		}
	}

	l = 0;
	for (i = 0; i < MAC_CAT_MAX; i++)
		if (mlevel->level_category[i] == 1) {
			l++;
			pr_debug("category[%d] == 1\n", i);
		}
	mlevel->level_catsum = l;

	pr_debug("cti %d:%d:%d:%d, pid %d\n", mlevel->level_type,
		mlevel->level_value, mlevel->level_catsum,
		ilevel->level_value, current->pid);

	
	if (cate && strlen(cate))
		kfree(cate);

	return 0;
}

/* The inode's security attributes must be initialized before first use. */
static int inode_doinit_with_dentry(struct inode *inode,
				struct dentry *opt_dentry)
{
	struct inode_security_struct *isec = inode->i_security;
	struct dentry *dentry;
	char *context = NULL;
	unsigned len = 0;
	int rc = 0;

	if (isec && isec->initialized)
		return rc;

	mutex_lock(&isec->lock);
	if (!inode->i_op->getxattr) {
		/* no getxattr iop, so init */
		pr_debug("no xattr iop when init, pid %d\n", current->pid);
		isec->task_sid = current_uid();
		init_mlevel(&isec->mlevel);
		isec->ilevel.level_value = 0;
		goto out;
	}

	/* Need a dentry, since the xattr API requires one.
	   Life would be simpler if we could just pass the inode. */
	if (opt_dentry)
		/* Called from d_instantiate or d_splice_alias. */
		dentry = dget(opt_dentry);
	if (!dentry) {
		/*
		 * this is can be hit on boot when a file is accessed
		 * inode_doinit with a dentry, before these inodes could
		 * be used again by userspace.
		 */
		goto out;
	}

	/* Query for the right size. */
	rc = inode->i_op->getxattr(dentry, XATTR_NAME_KSE,
				   NULL, 0);
	if (rc < 0) {
		dput(dentry);
		goto out;
	}

	len = rc;
	context = kmalloc(len+1, GFP_NOFS);
	if (!context) {
		rc = -ENOMEM;
		dput(dentry);
		goto out;
	}
	context[len] = '\0';
	rc = inode->i_op->getxattr(dentry, XATTR_NAME_KSE,
				   context, len);
	if (rc == -ERANGE) {
		kfree(context);

		/* Need a larger buffer.  Query for the right size. */
		rc = inode->i_op->getxattr(dentry, XATTR_NAME_KSE,
					   NULL, 0);
		if (rc < 0) {
			dput(dentry);
			goto out;
		}
		len = rc;
		context = kmalloc(len+1, GFP_NOFS);
		if (!context) {
			rc = -ENOMEM;
			dput(dentry);
			goto out;
		}
		context[len] = '\0';
		rc = inode->i_op->getxattr(dentry,
					   XATTR_NAME_KSE,
					   context, len);
	}
	dput(dentry);
	if (rc < 0) {
		if (rc != -ENODATA) {
			printk(KERN_WARNING "KUXSE: %s:  getxattr returned "
				"%d for dev=%s ino=%ld\n", __func__,
				-rc, inode->i_sb->s_id, inode->i_ino);
			kfree(context);
			goto out;
		}
		/* NODATA just init */
		rc = 0; 
		pr_debug("no xattr data when init\n");
		isec->task_sid = current_uid();
		init_mlevel(&isec->mlevel);
		isec->ilevel.level_value = 0;
	} else {
		/* getxattr succeeded */
		ssize_t sclen = strlen(context);
		rc = context_to_iss(&isec->mlevel, &isec->ilevel,
				(const void **)&context, &sclen);
		if (rc) {
			printk(KERN_ERR "KUXSE:  unable to map value '%s' "
				"to context for (%s, %lu), rc=%d, size=%ld\n",
				(char *)context, inode->i_sb->s_id,
				inode->i_ino, -rc, sclen);
			dump_stack();
			return rc;
		}
	}
	kfree(context);

	isec->initialized = 1;
out:
	mutex_unlock(&isec->lock);
	return rc;
}

static void kse_d_instantiate(struct dentry *dentry, struct inode *inode)
{
	if (inode)
		inode_doinit_with_dentry(inode, dentry);
}

static int kse_inode_init_security(struct inode *inode, struct inode *dir,
				       char **name, void **value,
				       size_t *len)
{
	const struct cred *cred = current_cred();
	struct inode_security_struct *isec = inode->i_security;
	u32 clen;
	int rc;
	char *namep = NULL, *context;

	isec->task_sid = cred->uid;

	if (name) {
		namep = kstrdup(XATTR_KSE_SUFFIX, GFP_NOFS);
		if (!namep)
			return -ENOMEM;
		*name = namep;
	}

	if (value && len) {
		//rc = iss_to_context(1, isec, &context, &clen);
		rc = iss_to_context(1, &isec->mlevel, &isec->ilevel,
					&context, &clen);
		if (rc) {
			kfree(namep);
			return rc;
		}
		*value = context;
		*len = clen;
	}

	return 0;
}

static void kse_inode_post_setxattr(struct dentry *dentry, const char *name,
					const void *value, size_t size,
					int flags)
{
	struct inode *inode = dentry->d_inode;
	struct inode_security_struct *isec = inode->i_security;
	int rc;

	if (strcmp(name, XATTR_NAME_KSE)) {
		/* Not an attribute we recognize, so nothing to do. */
		return;
	}
	
	if (value == NULL || size == 0)
		return;

	if (isec == NULL) {
		printk("post setxattr:inode security null\n");
		return;
	}

	//rc = context_to_iss(isec, &value, &size);
	rc = context_to_iss(&isec->mlevel, &isec->ilevel, &value, &size);
	if (rc) {
		printk(KERN_ERR "KUXSE:  unable to map value '%s' to context"
		       "for (%s, %lu), rc=%d, size=%ld\n", (char *)value,
		       inode->i_sb->s_id, inode->i_ino, -rc, size);
		dump_stack();
		return;
	}

	return;
}

/*
 * Copy the inode security context value to the user.
 *
 * Permission check is handled by kse_inode_getxattr hook.
 */
static int kse_inode_getsecurity(const struct inode *inode, const char *name,
					void **buffer, bool alloc)
{
	u32 size;
	int error;
	char *value = NULL;
	struct inode_security_struct *isec = inode->i_security;

	if (strcmp(name, XATTR_KSE_SUFFIX))
		return -EOPNOTSUPP;

	error = iss_to_context(0, &isec->mlevel, &isec->ilevel, &value, &size);
	if (error)
		return error;

	error = size;
	if (alloc) {
		*buffer = value;
		goto out_nofree;
	}
	kfree(value);
out_nofree:
	return error;
}

static int kse_inode_setsecurity(struct inode *inode, const char *name,
				     const void *value, size_t size, int flags)
{
	struct inode_security_struct *isec = inode->i_security;
	int rc;

	if (strcmp(name, XATTR_KSE_SUFFIX))
		return -EOPNOTSUPP;

	if (!value || !size)
		return -EACCES;

	rc = context_to_iss(&isec->mlevel, &isec->ilevel, &value, &size);
	if (rc) {
		printk(KERN_ERR "KUXSE:  unable to map value '%s' to context"
		       "for (%s, %lu), rc=%d, size=%ld\n", (char *)value,
		       inode->i_sb->s_id, inode->i_ino, -rc, size);
		dump_stack();
		return rc;
	}

	return 0;
}

static int kse_inode_listsecurity(struct inode *inode, char *buffer,
					size_t buffer_size)
{
	const int len = sizeof(XATTR_NAME_KSE);
	if (buffer && len <= buffer_size)
		memcpy(buffer, XATTR_NAME_KSE, len);
	return len;
}

static int kse_bprm_set_creds(struct linux_binprm *bprm)
{
	const struct task_security_struct *old_tsec;
	struct task_security_struct *new_tsec;
	struct inode *inode = bprm->file->f_path.dentry->d_inode;
	int rc;

	rc = cap_bprm_set_creds(bprm);
	if (rc)
		return rc;

	if (bprm->cred_prepared)
		return 0;

	rc = task_inode_perm(current, inode, bprm->file->f_path.dentry,
				FILE__EXECUTE);
	if (rc)
		return rc;

	/* Default to the current task SID. */
	old_tsec = current_security();
	new_tsec = bprm->cred->security;
	copy_mlevel(&new_tsec->mlevel, &old_tsec->mlevel);
	new_tsec->ilevel.level_value =  old_tsec->ilevel.level_value;


#if 0
	if (bprm->file->f_path.mnt->mnt_flags & MNT_NOSUID)
		new_tsec->sid = old_tsec->sid;

	if (new_tsec->sid == old_tsec->sid) {
		rc = task_inode_perm(current, inode, FILE__EXECUTE_NO_TRANS);
		if (rc)
			return rc;
	} else {
		/* Check permissions for the transition. */
		rc = task_inode_perm(current, inode, PROCESS__TRANSITION);
		if (rc)
			return rc;

		rc = task_inode_perm(current, inode, FILE__ENTRYPOINT);
		if (rc)
			return rc;
	}
#endif

	return 0;
}

/*
 * Prepare a process for imminent new credential changes due to exec
 */
static void kse_bprm_committing_creds(struct linux_binprm *bprm)
{
	struct task_security_struct *new_tsec;

	new_tsec = bprm->cred->security;
	if (new_tsec)
		return;
}

/*
 * Clean up the process immediately after the installation of new credentials
 * due to exec
 */
static void kse_bprm_committed_creds(struct linux_binprm *bprm)
{
	const struct task_security_struct *tsec = current_security();

	if (tsec)
		return;
}

static int kse_bprm_secureexec(struct linux_binprm *bprm)
{
	return cap_bprm_secureexec(bprm);
}

/*
 * allocate the KUXSE part of blank credentials
 */
static int kse_cred_alloc_blank(struct cred *cred, gfp_t gfp)
{
	struct task_security_struct *tsec;

	tsec = kzalloc(sizeof(struct task_security_struct), gfp);
	if (!tsec)
		return -ENOMEM;

	cred->security = tsec;
	return 0;
}

/*
 * detach and free the LSM part of a set of credentials
 */
static void kse_cred_free(struct cred *cred)
{
	struct task_security_struct *tsec = cred->security;

	BUG_ON((unsigned long) cred->security < PAGE_SIZE);
	cred->security = (void *) 0x7UL;
	kfree(tsec);
}

/*
 * prepare a new set of credentials for modification
 */
static int kse_cred_prepare(struct cred *new, const struct cred *old,
				gfp_t gfp)
{
	const struct task_security_struct *old_tsec;
	struct task_security_struct *tsec;

	old_tsec = old->security;

	tsec = kmemdup(old_tsec, sizeof(struct task_security_struct), gfp);
	if (!tsec)
		return -ENOMEM;

	new->security = tsec;
	return 0;
}

/*
 * transfer the KUXSE data to a blank set of creds
 */
static void kse_cred_transfer(struct cred *new, const struct cred *old)
{
	const struct task_security_struct *old_tsec = old->security;
	struct task_security_struct *tsec = new->security;

	*tsec = *old_tsec;
}

static int kse_task_setpgid(struct task_struct *p, pid_t pgid)
{
	return task_task_perm(current, p, PROCESS__SETPGID);
}

static int kse_task_getpgid(struct task_struct *p)
{
	return task_task_perm(current, p, PROCESS__GETPGID);
}

static int kse_task_getsid(struct task_struct *p)
{
	return task_task_perm(current, p, PROCESS__GETSESSION);
}

static int kse_task_setnice(struct task_struct *p, int nice)
{
	int rc;

	rc = cap_task_setnice(p, nice);
	if (rc)
		return rc;

	return task_task_perm(current, p, PROCESS__SETSCHED);
}

static int kse_task_setioprio(struct task_struct *p, int ioprio)
{
	int rc;

	rc = cap_task_setioprio(p, ioprio);
	if (rc)
		return rc;

	return task_task_perm(current, p, PROCESS__SETSCHED);
}

static int kse_task_getioprio(struct task_struct *p)
{
	return task_task_perm(current, p, PROCESS__GETSCHED);
}

static int kse_task_setscheduler(struct task_struct *p, int policy,
				struct sched_param *lp)
{
	int rc;

	rc = cap_task_setscheduler(p, policy, lp);
	if (rc)
		return rc;

	return task_task_perm(current, p, PROCESS__SETSCHED);
}

static int kse_task_getscheduler(struct task_struct *p)
{
	return task_task_perm(current, p, PROCESS__GETSCHED);
}

static int kse_task_movememory(struct task_struct *p)
{
	return task_task_perm(current, p, PROCESS__SETSCHED);
}

static int kse_task_kill(struct task_struct *p, struct siginfo *info,
				int sig, u32 secid)
{
	return task_task_perm(current, p, PROCESS__SIGNAL);
}

static int kse_task_wait(struct task_struct *p)
{
	return task_task_perm(current, p, PROCESS__SIGCHLD);
}

static void kse_task_to_inode(struct task_struct *p,
				  struct inode *inode)
{
	struct inode_security_struct *iss = inode->i_security;
	struct task_security_struct *tss;

	tss = (struct task_security_struct *)task_cred_xxx(p, security);

	if (iss && tss && &tss->mlevel) {
		copy_mlevel(&iss->mlevel, &tss->mlevel);
		iss->ilevel.level_value = tss->ilevel.level_value;

		mutex_init(&iss->lock);
		iss->task_sid = current_uid();
	}
}

static int kse_inode_create(struct inode *dir, struct dentry *dentry, int mask)
{
	return task_inode_perm(current, dir, dentry, FILE__CREATE);
}

static int kse_inode_link(struct dentry *old_dentry, struct inode *dir,
				struct dentry *new_dentry)
{
	return task_inode_perm(current, dir, new_dentry, FILE__LINK);
}

static int kse_inode_unlink(struct inode *dir, struct dentry *dentry)
{
	return task_inode_perm(current, dir, dentry, FILE__UNLINK);
}

static int kse_inode_symlink(struct inode *dir, struct dentry *dentry,
				const char *name)
{
	return task_inode_perm(current, dir, dentry, FILE__LINK);
}

static int kse_inode_mkdir(struct inode *dir, struct dentry *dentry, int mask)
{
	return task_inode_perm(current, dir, dentry, FILE__WRITE);
}

static int kse_inode_rmdir(struct inode *dir, struct dentry *dentry)
{
	return task_inode_perm(current, dir, dentry, FILE__WRITE);
}

static int kse_inode_mknod(struct inode *dir, struct dentry *dentry,
				int mode, dev_t dev)
{
	return task_inode_perm(current, dir, dentry, FILE__WRITE);
}

static int kse_inode_rename(struct inode *old_inode, struct dentry *old_dentry,
			struct inode *new_inode, struct dentry *new_dentry)
{
	return task_inode_perm(current, old_inode, old_dentry, FILE__WRITE);
}

static int kse_inode_readlink(struct dentry *dentry)
{
	return task_inode_perm(current, dentry->d_inode, dentry, FILE__WRITE);
}

static int kse_inode_follow_link(struct dentry *dentry,
					struct nameidata *nameidata)
{
	return task_inode_perm(current, dentry->d_inode, dentry, FILE__WRITE);
}

/* Convert a Linux mode and permission mask to an access vector. */
static int file_mask_to_av_check(struct inode *inode, int mask)
{
	int mode = inode->i_mode;
	int rc;

	if ((mode & S_IFMT) != S_IFDIR) {
		if (mask & MAY_EXEC) {
			rc = task_inode_perm(current, inode, NULL, FILE__EXECUTE);
			if (rc)
				return rc;
		}

		if (mask & MAY_READ) {
			rc = task_inode_perm(current, inode, NULL, FILE__READ);
			if (rc)
				return rc;
		}

		if (mask & MAY_APPEND) {
			rc = task_inode_perm(current, inode, NULL, FILE__APPEND);
			if (rc)
				return rc;
		}
		
		if (mask & MAY_WRITE) {
			rc = task_inode_perm(current, inode, NULL, FILE__WRITE);
			if (rc)
				return rc;
		}

	} else {
		if (mask & MAY_EXEC) {
			rc = task_inode_perm(current, inode, NULL, FILE__EXECUTE);
			if (rc)
				return rc;
		}

		if (mask & MAY_WRITE) {
			rc = task_inode_perm(current, inode, NULL, FILE__WRITE);
			if (rc)
				return rc;
		}

		if (mask & MAY_READ) {
			rc = task_inode_perm(current, inode, NULL, FILE__READ);
			if (rc)
				return rc;
		}
	}
	return 0;
}

static int kse_inode_permission(struct inode *inode, int mask)
{
	if (!mask) {
		/* No permission to check.  Existence test. */
		return 0;
	}

	return file_mask_to_av_check(inode, mask);
}

static int kse_inode_setattr(struct dentry *dentry, struct iattr *iattr)
{
	return task_inode_perm(current, dentry->d_inode, dentry, FILE__SETATTR);
}

static int kse_inode_getattr(struct vfsmount *mnt, struct dentry *dentry)
{
	return task_inode_perm(current, dentry->d_inode, dentry, FILE__GETATTR);
}

static int kse_inode_setxattr(struct dentry *dentry, const char *name,
				  const void *value, size_t size, int flags)
{
	struct inode *inode = dentry->d_inode;
	struct inode_security_struct *isec = inode->i_security;
	const struct task_security_struct *tsec = current_security();

	pr_debug("kux sx isec %d:%d:%d:%d, pid %d\n", isec->mlevel.level_type,
		isec->mlevel.level_value, isec->mlevel.level_catsum,
		isec->ilevel.level_value, current->pid);

	pr_debug("kux sx tsec %d:%d:%d:%d, pid %d\n", tsec->mlevel.level_type,
		tsec->mlevel.level_value, tsec->mlevel.level_catsum,
		tsec->ilevel.level_value, current->pid);

	//if (strcmp(name, XATTR_NAME_KSE))
	//	return -EINVAL;

	if (!is_owner_or_cap(inode))
		return -EPERM;

	return task_inode_perm(current, dentry->d_inode, dentry, FILE__SETATTR);
}

static int kse_inode_getxattr(struct dentry *dentry, const char *name)
{
	return task_inode_perm(current, dentry->d_inode, dentry, FILE__GETATTR);
}

static int kse_inode_listxattr(struct dentry *dentry)
{
	return task_inode_perm(current, dentry->d_inode, dentry, FILE__GETATTR);
}

static int kse_inode_removexattr(struct dentry *dentry, const char *name)
{
	//if (strcmp(name, XATTR_NAME_KSE) != 0)
	//	return -EACCES;

	return task_inode_perm(current, dentry->d_inode, dentry, FILE__SETATTR);
}

/*
 * Minimal support for a secondary security module,
 * just to allow the use of the capability module.
 */

static struct security_operations kse_ops = {
	.name =				"kse",
#if 0
	.ptrace_access_check =		kse_ptrace_access_check,
	.ptrace_traceme =		kse_ptrace_traceme,
	.capget =			kse_capget,
	.capset =			kse_capset,
	.sysctl =			kse_sysctl,
	.capable =			kse_capable,
#endif
	.bprm_set_creds =		kse_bprm_set_creds,
	.bprm_committing_creds =	kse_bprm_committing_creds,
	.bprm_committed_creds =		kse_bprm_committed_creds,
	.bprm_secureexec =		kse_bprm_secureexec,

	.inode_alloc_security =		kse_inode_alloc_security,
	.inode_free_security =		kse_inode_free_security,
	.inode_init_security =		kse_inode_init_security,

	.inode_create =			kse_inode_create,
	.inode_link =			kse_inode_link,
	.inode_unlink =			kse_inode_unlink,
	.inode_symlink =		kse_inode_symlink,
	.inode_mkdir =			kse_inode_mkdir,
	.inode_rmdir =			kse_inode_rmdir,
	.inode_mknod =			kse_inode_mknod,
	.inode_rename =			kse_inode_rename,
	.inode_readlink =		kse_inode_readlink,
	.inode_follow_link =		kse_inode_follow_link,

	.inode_permission =		kse_inode_permission,
	.inode_setattr =		kse_inode_setattr,
	.inode_getattr =		kse_inode_getattr,

	.inode_setxattr =		kse_inode_setxattr,
	.inode_post_setxattr =		kse_inode_post_setxattr,
	.inode_getxattr =		kse_inode_getxattr,
	.inode_listxattr =		kse_inode_listxattr,
	.inode_removexattr =		kse_inode_removexattr,

	.inode_getsecurity =		kse_inode_getsecurity,
	.inode_setsecurity =		kse_inode_setsecurity,
	.inode_listsecurity =		kse_inode_listsecurity,
#if 0
	.inode_getsecid =		kse_inode_getsecid,

	.file_permission =		kse_file_permission,
	.file_alloc_security =		kse_file_alloc_security,
	.file_free_security =		kse_file_free_security,
	.file_ioctl =			kse_file_ioctl,
	.file_mmap =			kse_file_mmap,
	.file_mprotect =		kse_file_mprotect,
	.file_lock =			kse_file_lock,
	.file_fcntl =			kse_file_fcntl,
	.file_set_fowner =		kse_file_set_fowner,
	.file_send_sigiotask =		kse_file_send_sigiotask,
	.file_receive =			kse_file_receive,

	.dentry_open =			kse_dentry_open,
	.path_truncate = 		kse_path_truncate,
	.task_create =			kse_task_create,
#endif
	.cred_alloc_blank =		kse_cred_alloc_blank,
	.cred_free =			kse_cred_free,
	.cred_prepare =			kse_cred_prepare,
	.cred_transfer =		kse_cred_transfer,
#if 0
	.kernel_act_as =		kse_kernel_act_as,
	.kernel_create_files_as =	kse_kernel_create_files_as,
	.kernel_module_request =	kse_kernel_module_request,
	//.task_getsecid =		kse_task_getsecid,
	//.task_setrlimit =		kse_task_setrlimit,
#endif
	.task_setpgid =			kse_task_setpgid,
	.task_getpgid =			kse_task_getpgid,
	.task_getsid =			kse_task_getsid,
	.task_setnice =			kse_task_setnice,
	.task_setioprio =		kse_task_setioprio,
	.task_getioprio =		kse_task_getioprio,
	.task_setscheduler =		kse_task_setscheduler,
	.task_getscheduler =		kse_task_getscheduler,
	.task_movememory =		kse_task_movememory,
	.task_kill =			kse_task_kill,
	.task_wait =			kse_task_wait,
	.task_to_inode =		kse_task_to_inode,
#if 0
	.ipc_permission =		kse_ipc_permission,
	.ipc_getsecid =			kse_ipc_getsecid,

#endif
	.d_instantiate =		kse_d_instantiate,
#if 0
	.getprocattr =			kse_getprocattr,
	.setprocattr =			kse_setprocattr,
	.secid_to_secctx =		kse_secid_to_secctx,
	.secctx_to_secid =		kse_secctx_to_secid,
	.release_secctx =		kse_release_secctx,
	.inode_notifysecctx =		kse_inode_notifysecctx,
	.inode_setsecctx =		kse_inode_setsecctx,
	.inode_getsecctx =		kse_inode_getsecctx,
#endif
};

static int kse_seq_show(struct seq_file *seq, void *offset)
{
	seq_printf(seq, "%d\n", kse_enabled);
	return 0;
}

static ssize_t
kse_write(struct file *file, const char __user *buf, size_t len, loff_t * ppos)
{
	int reg;
	char line[2];
	int length;
	size_t linelen;

	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;

	memset(line, 0, 2);

	length = len;

	if (length > 1)
		length = 1;

	if (length < 0)
		return -EINVAL;

	if (copy_from_user(line, buf, length))
		return -EFAULT;

	line[1] = '\0';
	linelen = strlen(line);
	if (linelen != 1 || isdigit(line[0]) == 0)
		return -EINVAL;

	reg = line[0] - '0';

	if (reg == 0)
		kse_enabled = 0;
	else if (reg == 1)
		kse_enabled = 1;
	else
		return -EINVAL;

	return len;
}

static int kse_open(struct inode *inode, struct file *file)
{
	return single_open(file, kse_seq_show, NULL);
}

static const struct file_operations kse_fops = {
	.open			= kse_open,
	.read			= seq_read,
	.llseek			= seq_lseek,
	.write			= kse_write,
	.release		= single_release,
};

static __init int kse_init(void)
{
	if (!security_module_enable(&kse_ops)) {
		printk(KERN_INFO "KUXSE:  Initializing error.\n");
		kse_enabled = 0;
		return 0;
	}

	if (kse_enabled == 0) {
		printk(KERN_INFO "KUXSE:  Disabled.\n");
		return 0;
	}

	printk(KERN_INFO "KUXSE:  Initializing.\n");

	proc_create("kuxse", S_IWUSR | S_IRUGO, NULL, &kse_fops);

	/* Set the security state for the initial task. */
	cred_init_security();

	kse_inode_cache = kmem_cache_create("kse_inode_security",
					   sizeof(struct inode_security_struct),
					   0, SLAB_PANIC, NULL);
	
	/*
	secondary_ops = security_ops;
	if (!secondary_ops)
		panic("SELinux: No initial security operations\n");
	*/
	if (register_security(&kse_ops))
		panic("KUXSE: Unable to register with kernel.\n");

	return 0;
}

/* KUXSE requires early initialization in order to label
   all processes and objects when they are created. */
security_initcall(kse_init);
