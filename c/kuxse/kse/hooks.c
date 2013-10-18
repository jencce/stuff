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
#include <linux/netfilter_ipv4.h>
#include <linux/netfilter_ipv6.h>
#include <linux/tty.h>
#include <net/icmp.h>
#include <net/ip.h>		/* for local_port_range[] */
#include <net/tcp.h>		/* struct or_callable used in sock_rcv_skb */
#include <net/net_namespace.h>
#include <net/netlabel.h>
#include <linux/uaccess.h>
#include <asm/ioctls.h>
#include <asm/atomic.h>
#include <linux/bitops.h>
#include <linux/interrupt.h>
#include <linux/netdevice.h>	/* for network interface checks */
#include <linux/netlink.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/dccp.h>
#include <linux/quota.h>
#include <linux/un.h>		/* for Unix socket types */
#include <net/af_unix.h>	/* for Unix socket types */
#include <linux/parser.h>
#include <linux/nfs_mount.h>
#include <net/ipv6.h>
#include <linux/hugetlb.h>
#include <linux/personality.h>
#include <linux/sysctl.h>
#include <linux/audit.h>
#include <linux/string.h>
#include <linux/kse.h>
#include <linux/mutex.h>
#include <linux/posix-timers.h>
#include <linux/lsm_audit.h>

#include "kseobj.h"
#include "flask.h"
//#include "permissions.h"
//#include "security.h"

#define XATTR_KSE_SUFFIX "kse"
#define XATTR_NAME_KSE XATTR_SECURITY_PREFIX XATTR_KSE_SUFFIX

//#define ZX

#ifdef CONFIG_SECURITY_KSE_BOOTPARAM
int kse_enabled = CONFIG_SECURITY_KSE_BOOTPARAM_VALUE;

static int __init kse_enabled_setup(char *str)
{
	unsigned long enabled;
	if (!strict_strtoul(str, 0, &enabled))
		kse_enabled = enabled ? 1 : 0;
	return 1;
}
__setup("kse=", kse_enabled_setup);
#else
int kse_enabled = 1;
#endif

static int init_mlevel(struct mac_level *mlevel)
{
	mlevel->level_type = MAC_TYPE_UNDEF;
	mlevel->level_value = 0;
	mlevel->level_flag = 0;
	mlevel->level_catsum = 0;

	return 0;
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
	pr_debug("kse inode alloc sec, %d\n", current->pid);
	return inode_alloc_security(inode);
}

static void kse_inode_free_security(struct inode *inode)
{
	pr_debug("kse inode free sec, %d\n", current->pid);
	inode_free_security(inode);
}

/* init inode security attrs and context string,
 * according to attrs of current task
 * i_security to xattr string
 */
static int iss_to_context(int init, struct inode_security_struct *iss,
				char **scontext, u32 *scontext_len)
{
	const struct task_security_struct *tss = current_security();
	int i = 0;
	char *cate = NULL;
	char *cp = NULL;
	char temp[5];

	*scontext = NULL;
	*scontext_len  = 0;

	if (init && tss) {
		memcpy(&iss->mlevel, &tss->mlevel, sizeof(struct mac_level));
		memcpy(&iss->ilevel, &tss->ilevel, sizeof(struct intigrety_level));
		pr_debug("zx3t,csum %d, ml %d, il %d, pid %d\n",
			tss->mlevel.level_catsum, tss->mlevel.level_value,
			tss->ilevel.level_value, current->pid);
		iss->initialized = 1;
	}

	pr_debug("zx3i,csum %d, ml %d, il %d, p %d\n", iss->mlevel.level_catsum,
		iss->mlevel.level_value, iss->ilevel.level_value, current->pid);

	if (iss->mlevel.level_catsum) 
		cp = kmalloc(iss->mlevel.level_catsum*3 - 1 + 8, GFP_KERNEL);
	else
		cp = kmalloc(MAC_CAT_MAX*3 - 1 + 8, GFP_KERNEL);

	if (cp == NULL) {
		pr_debug("zx31, csum %d\n", iss->mlevel.level_catsum);
		dump_stack();
		return -ENOMEM;
	}

	if (iss->mlevel.level_catsum) {
		cate = kmalloc(iss->mlevel.level_catsum*3-1, GFP_KERNEL);
		if (cate == NULL) {
			pr_debug("zx32, csum %d\n", iss->mlevel.level_catsum);
			dump_stack();
			return -ENOMEM;
		}
		memset(cate, 0, iss->mlevel.level_catsum*3-1);
	}

	for (i = 0; cate && i < MAC_CAT_MAX; i++) {
		if (iss->mlevel.level_category[i] == 1) {
			if (strlen(cate) > 0)
				strcat(cate, ",");
			memset(temp, 0, 5);
			sprintf(temp, "c%d", i);
			strcat(cate, temp);
		}
	}

	/* empty category */
	if (cate == NULL)
		cate = kstrdup("c10", GFP_KERNEL);

	sprintf(cp, "%d:%d:%s:%d", iss->mlevel.level_type,
		iss->mlevel.level_value, cate, iss->ilevel.level_value);
	pr_debug("zx33 %d:%d:%s:%d, pid %d\n", iss->mlevel.level_type,
		iss->mlevel.level_value, cate, iss->ilevel.level_value,
		current->pid);

	if (cp == NULL)
		cp = kstrdup("2:2:c0:2", GFP_KERNEL);

	if (cate) 
		kfree(cate);
	
	*scontext = cp;
	*scontext_len = strlen(*scontext);

	return 0;

}

/* context string to inode->i_security  */
static int context_to_iss(struct inode_security_struct *iss,
				const void **scontext, ssize_t *scontext_len)
{
	char *cate = NULL;
	char *p = NULL;
	char *cp = NULL;
	char *tp = NULL;
	char temp[5];
	int i, l, ucnt, ctemp;

	if ( *scontext_len < 5 || *scontext == NULL )
		return 0;

	p = (char *)*scontext;
	ucnt = 0;
	for (i = 0; p[i] && i < *scontext_len; i++) {
		if (p[i] == ':')
			ucnt++;
	}
	if (ucnt != 3)
		return 0;

	cp = (char *)*scontext;
	if ( cp[0] >= '0' && cp[0] <= '9' )
		iss->mlevel.level_type = cp[0] - '0';
	else
		iss->mlevel.level_type = 9;

	while (*cp != ':')
		cp++;
	cp++;

	if ( *cp >= '0' && *cp <= '9' )
		iss->mlevel.level_value = cp[0] - '0';
	else
		iss->mlevel.level_value = 9;

	while (*cp != ':')
		cp++;
	cp++;

	cate = kmalloc(*scontext_len, GFP_KERNEL);
	if (cate == NULL) {
		pr_debug("zx1\n");
		return -ENOMEM;
	}
	memset(cate, 0, (int)*scontext_len);
	memset(temp, 0, 5);

	p = cate;
	while (*cp != ':')
		*p++ = *cp++;
	cp++;

	if ( *cp >= '0' && *cp <= '9' )
		iss->ilevel.level_value = cp[0] - '0';
	else
		iss->ilevel.level_value = 9;

	pr_debug("zx11 scontext:%s, %d:%d:%s:%d, pid %d\n", (char *)*scontext,
			iss->mlevel.level_type, iss->mlevel.level_value,
			cate, iss->ilevel.level_value, current->pid);

	for (i = 0; i < MAC_CAT_MAX; i++)
		iss->mlevel.level_category[i] = 0;

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
			iss->mlevel.level_category[ctemp] = 1;
			p = &cate[i+1];
			continue;
		}
		if (i == l - 1 && cate[i] != ',') {
			tp = &cate[i];
			ucnt = tp - p + 1;
			memset(temp, 0, 5);
			strncpy(temp, p, ucnt);
			sscanf(temp, "c%d", &ctemp);
			iss->mlevel.level_category[ctemp] = 1;
		}
	}

	l = 0;
	for (i = 0; i < MAC_CAT_MAX; i++)
		if (iss->mlevel.level_category[i] == 1)
			l++;
	iss->mlevel.level_catsum = l;

	pr_debug("zx12 %d:%d:%d:%d, pid %d\n", iss->mlevel.level_type,
		iss->mlevel.level_value, iss->mlevel.level_catsum,
		iss->ilevel.level_value, current->pid);

	
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
#define INITCONTEXTLEN 255
	char *context = NULL;
	unsigned len = 0;
	int rc = 0;

	if (isec && isec->initialized)
		return rc;

	mutex_lock(&isec->lock);
	if (!inode->i_op->getxattr) {
		/* no getxattr iop, so init */
		pr_debug("no xattr iop when init\n");
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

	len = INITCONTEXTLEN;
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
		//printk("context %s, rc %d\n", context, rc);
		//kfree(context);
		//goto out;
		rc = context_to_iss(isec, (const void **)&context, &sclen);
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

	pr_debug("kse inode init sec, %d\n", current->pid);

	isec->task_sid = cred->uid;

	if (name) {
		namep = kstrdup(XATTR_KSE_SUFFIX, GFP_NOFS);
		if (!namep)
			return -ENOMEM;
		*name = namep;
	}

	if (value && len) {
		rc = iss_to_context(1, isec, &context, &clen);
		if (rc) {
			kfree(namep);
			return rc;
		}
		*value = context;
		*len = clen;
	}

	return 0;
}

static int kse_inode_setxattr(struct dentry *dentry, const char *name,
				  const void *value, size_t size, int flags)
{
	struct inode *inode = dentry->d_inode;
	struct inode_security_struct *isec = inode->i_security;
	const struct cred *cred = current_cred();
	const struct task_security_struct *tsec = cred->security;
	//u32 newsid, sid = current_uid();
	//int rc = 0;

	pr_debug("zxsx isec %d:%d:%d:%d, pid %d\n", isec->mlevel.level_type,
		isec->mlevel.level_value, isec->mlevel.level_catsum,
		isec->ilevel.level_value, current->pid);

	pr_debug("zxsx tsec %d:%d:%d:%d, pid %d\n", tsec->mlevel.level_type,
		tsec->mlevel.level_value, tsec->mlevel.level_catsum,
		tsec->ilevel.level_value, current->pid);

	if (strcmp(name, XATTR_NAME_KSE))
		return -EINVAL;

	if (!is_owner_or_cap(inode))
		return -EPERM;

#if 0
	rc = security_context_to_sid(value, size, &newsid);
	if (rc == -EINVAL) {
		if (!capable(CAP_MAC_ADMIN))
			return rc;
		rc = security_context_to_sid_force(value, size, &newsid);
	}
	if (rc)
		return rc;

	rc = avc_has_perm(sid, newsid, isec->sclass,
			  FILE__RELABELTO, &ad);

	return rc;
#endif

	return 0;
}

static void kse_inode_post_setxattr(struct dentry *dentry, const char *name,
					const void *value, size_t size,
					int flags)
{
	struct inode *inode = dentry->d_inode;
	struct inode_security_struct *isec = inode->i_security;
	int rc;

	pr_debug("kse inode post sx, %d\n", current->pid);

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

	rc = context_to_iss(isec, &value, &size);
	if (rc) {
		printk(KERN_ERR "KUXSE:  unable to map value '%s' to context"
		       "for (%s, %lu), rc=%d\n", (char *)value,
		       inode->i_sb->s_id, inode->i_ino, -rc);
		return;
	}

	return;
}

#if 0
static int kse_inode_getxattr(struct dentry *dentry, const char *name)
{
	return 0;
}

static int kse_inode_listxattr(struct dentry *dentry)
{
	return 0;
}
#endif

/*
 * Copy the inode security context value to the user.
 *
 * Permission check is handled by kse_inode_getxattr hook.
 */
static int kse_inode_getsecurity(const struct inode *inode, const char *name, void **buffer, bool alloc)
{
	u32 size;
	int error;
	char *value = NULL;
	struct inode_security_struct *isec = inode->i_security;

	pr_debug("kse inode getsec, %d\n", current->pid);
	//dump_stack();

	if (strcmp(name, XATTR_KSE_SUFFIX))
		return -EOPNOTSUPP;

	//error = selinux_capable(current, current_cred(), CAP_MAC_ADMIN,
	//			SECURITY_CAP_NOAUDIT);

	error = iss_to_context(0, isec, &value, &size);
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
	//u32 newsid;
	int rc;

	pr_debug("kse inode setsec, %d\n", current->pid);

	if (strcmp(name, XATTR_KSE_SUFFIX))
		return -EOPNOTSUPP;

	if (!value || !size)
		return -EACCES;

	rc = context_to_iss(isec, &value, &size);
	if (rc)
		return rc;

	return 0;
}

static int kse_inode_listsecurity(struct inode *inode, char *buffer, size_t buffer_size)
{
	const int len = sizeof(XATTR_NAME_KSE);
	if (buffer && len <= buffer_size)
		memcpy(buffer, XATTR_NAME_KSE, len);
	return len;
}

#if 0

/* Check whether a task has a particular permission to an inode.
   The 'adp' parameter is optional and allows other audit
   data to be passed (e.g. the dentry). */
static int inode_has_perm(const struct cred *cred,
			  struct inode *inode,
			  u32 perms,
			  struct common_audit_data *adp)
{
	struct inode_security_struct *isec;
	struct common_audit_data ad;
	u32 sid;

	validate_creds(cred);

	if (unlikely(IS_PRIVATE(inode)))
		return 0;

	sid = cred_sid(cred);
	isec = inode->i_security;

	if (!adp) {
		adp = &ad;
		COMMON_AUDIT_DATA_INIT(&ad, FS);
		ad.u.fs.inode = inode;
	}

	//return avc_has_perm(sid, isec->sid, isec->sclass, perms, adp);
	return 0;
}

static int kse_inode_permission(struct inode *inode, int mask)
{
	const struct cred *cred = current_cred();

	if (!mask) {
		/* No permission to check.  Existence test. */
		return 0;
	}

	return inode_has_perm(cred, inode,
			      file_mask_to_av(inode->i_mode, mask), NULL);
}

static int file_alloc_security(struct file *file)
{
	struct file_security_struct *fsec;
	u32 sid = current_sid();

	fsec = kzalloc(sizeof(struct file_security_struct), GFP_KERNEL);
	if (!fsec)
		return -ENOMEM;

	fsec->sid = sid;
	fsec->fown_sid = sid;
	file->f_security = fsec;

	return 0;
}

static void file_free_security(struct file *file)
{
	struct file_security_struct *fsec = file->f_security;
	file->f_security = NULL;
	kfree(fsec);
}

static int kse_bprm_set_creds(struct linux_binprm *bprm)
{
	const struct task_security_struct *old_tsec;
	struct task_security_struct *new_tsec;
	struct inode_security_struct *isec;
	struct common_audit_data ad;
	struct inode *inode = bprm->file->f_path.dentry->d_inode;
	int rc;

	rc = cap_bprm_set_creds(bprm);
	if (rc)
		return rc;

	/* SELinux context only depends on initial program or script and not
	 * the script interpreter */
	if (bprm->cred_prepared)
		return 0;

	old_tsec = current_security();
	new_tsec = bprm->cred->security;
	isec = inode->i_security;

	/* Default to the current task SID. */
	new_tsec->sid = old_tsec->sid;
	new_tsec->osid = old_tsec->sid;

	/* Reset fs, key, and sock SIDs on execve. */
	new_tsec->create_sid = 0;
	new_tsec->keycreate_sid = 0;
	new_tsec->sockcreate_sid = 0;

	if (old_tsec->exec_sid) {
		new_tsec->sid = old_tsec->exec_sid;
		/* Reset exec SID on execve. */
		new_tsec->exec_sid = 0;
	}
#if 0
	} else {
		/* Check for a default transition on this program. */
		rc = security_transition_sid(old_tsec->sid, isec->sid,
					     SECCLASS_PROCESS, &new_tsec->sid);
		if (rc)
			return rc;
	}
#endif

	COMMON_AUDIT_DATA_INIT(&ad, FS);
	ad.u.fs.path = bprm->file->f_path;

	if (bprm->file->f_path.mnt->mnt_flags & MNT_NOSUID)
		new_tsec->sid = old_tsec->sid;

#if 0
	if (new_tsec->sid == old_tsec->sid) {
		rc = avc_has_perm(old_tsec->sid, isec->sid,
				  SECCLASS_FILE, FILE__EXECUTE_NO_TRANS, &ad);
		if (rc)
			return rc;
	} else {
		/* Check permissions for the transition. */
		rc = avc_has_perm(old_tsec->sid, new_tsec->sid,
				  SECCLASS_PROCESS, PROCESS__TRANSITION, &ad);
		if (rc)
			return rc;

		rc = avc_has_perm(new_tsec->sid, isec->sid,
				  SECCLASS_FILE, FILE__ENTRYPOINT, &ad);
		if (rc)
			return rc;

		/* Check for shared state */
		if (bprm->unsafe & LSM_UNSAFE_SHARE) {
			rc = avc_has_perm(old_tsec->sid, new_tsec->sid,
					  SECCLASS_PROCESS, PROCESS__SHARE,
					  NULL);
			if (rc)
				return -EPERM;
		}

		/* Make sure that anyone attempting to ptrace over a task that
		 * changes its SID has the appropriate permit */
		if (bprm->unsafe &
		    (LSM_UNSAFE_PTRACE | LSM_UNSAFE_PTRACE_CAP)) {
			struct task_struct *tracer;
			struct task_security_struct *sec;
			u32 ptsid = 0;

			rcu_read_lock();
			tracer = tracehook_tracer_task(current);
			if (likely(tracer != NULL)) {
				sec = __task_cred(tracer)->security;
				ptsid = sec->sid;
			}
			rcu_read_unlock();

			if (ptsid != 0) {
				rc = avc_has_perm(ptsid, new_tsec->sid,
						  SECCLASS_PROCESS,
						  PROCESS__PTRACE, NULL);
				if (rc)
					return -EPERM;
			}
		}

		/* Clear any possibly unsafe personality bits on exec: */
		bprm->per_clear |= PER_CLEAR_ON_SETID;
	}
#endif

	return 0;
}

static int kse_file_alloc_security(struct file *file)
{
	return file_alloc_security(file);
}

static void kse_file_free_security(struct file *file)
{
	file_free_security(file);
}

/* Check whether a task can use an open file descriptor to
   access an inode in a given way.  Check access to the
   descriptor itself, and then use dentry_has_perm to
   check a particular permission to the file.
   Access to the descriptor is implicitly granted if it
   has the same SID as the process.  If av is zero, then
   access to the file is not checked, e.g. for cases
   where only the descriptor is affected like seek. */
static int file_has_perm(const struct cred *cred,
			 struct file *file,
			 u32 av)
{
	//struct file_security_struct *fsec = file->f_security;
	struct inode *inode = file->f_path.dentry->d_inode;
	struct common_audit_data ad;
	//u32 sid = cred_sid(cred);
	int rc;

	COMMON_AUDIT_DATA_INIT(&ad, FS);
	ad.u.fs.path = file->f_path;

#if 0
	if (sid != fsec->sid) {
		rc = avc_has_perm(sid, fsec->sid,
				  SECCLASS_FD,
				  FD__USE,
				  &ad);
		if (rc)
			goto out;
	}
#endif

	/* av is zero if only checking access to the descriptor. */
	rc = 0;
	if (av)
		rc = inode_has_perm(cred, inode, av, &ad);

//out:
	return rc;
}

/* Convert a Linux file to an access vector. */
static inline u32 file_to_av(struct file *file)
{
	u32 av = 0;

	if (file->f_mode & FMODE_READ)
		av |= FILE__READ;
	if (file->f_mode & FMODE_WRITE) {
		if (file->f_flags & O_APPEND)
			av |= FILE__APPEND;
		else
			av |= FILE__WRITE;
	}
	if (!av) {
		/*
		 * Special file opened with flags 3 for ioctl-only use.
		 */
		av = FILE__IOCTL;
	}

	return av;
}

struct dentry *kse_null;
struct vfsmount *kse_mount;
/* Derived from fs/exec.c:flush_old_files. */
static inline void flush_unauthorized_files(const struct cred *cred,
					    struct files_struct *files)
{
	struct common_audit_data ad;
	struct file *file, *devnull = NULL;
	struct tty_struct *tty;
	struct fdtable *fdt;
	long j = -1;
	int drop_tty = 0;

	tty = get_current_tty();
	if (tty) {
		file_list_lock();
		if (!list_empty(&tty->tty_files)) {
			struct inode *inode;

			/* Revalidate access to controlling tty.
			   Use inode_has_perm on the tty inode directly rather
			   than using file_has_perm, as this particular open
			   file may belong to another process and we are only
			   interested in the inode-based check here. */
			file = list_first_entry(&tty->tty_files, struct file, f_u.fu_list);
			inode = file->f_path.dentry->d_inode;
			if (inode_has_perm(cred, inode,
					   FILE__READ | FILE__WRITE, NULL)) {
				drop_tty = 1;
			}
		}
		file_list_unlock();
		tty_kref_put(tty);
	}
	/* Reset controlling tty. */
	if (drop_tty)
		no_tty();

	/* Revalidate access to inherited open files. */

	COMMON_AUDIT_DATA_INIT(&ad, FS);

	spin_lock(&files->file_lock);
	for (;;) {
		unsigned long set, i;
		int fd;

		j++;
		i = j * __NFDBITS;
		fdt = files_fdtable(files);
		if (i >= fdt->max_fds)
			break;
		set = fdt->open_fds->fds_bits[j];
		if (!set)
			continue;
		spin_unlock(&files->file_lock);
		for ( ; set ; i++, set >>= 1) {
			if (set & 1) {
				file = fget(i);
				if (!file)
					continue;
				if (file_has_perm(cred,
						  file,
						  file_to_av(file))) {
					sys_close(i);
					fd = get_unused_fd();
					if (fd != i) {
						if (fd >= 0)
							put_unused_fd(fd);
						fput(file);
						continue;
					}
					if (devnull) {
						get_file(devnull);
					} else {
						devnull = dentry_open(
							dget(kse_null),
							mntget(kse_mount),
							O_RDWR, cred);
						if (IS_ERR(devnull)) {
							devnull = NULL;
							put_unused_fd(fd);
							fput(file);
							continue;
						}
					}
					fd_install(fd, devnull);
				}
				fput(file);
			}
		}
		spin_lock(&files->file_lock);

	}
	spin_unlock(&files->file_lock);
}

/*
 * Prepare a process for imminent new credential changes due to exec
 */
static void kse_bprm_committing_creds(struct linux_binprm *bprm)
{
	struct task_security_struct *new_tsec;
	//struct rlimit *rlim, *initrlim;
	//int rc, i;

	new_tsec = bprm->cred->security;
	if (new_tsec->sid == new_tsec->osid)
		return;

#if 0
	/* Close files for which the new task SID is not authorized. */
	flush_unauthorized_files(bprm->cred, current->files);

	/* Always clear parent death signal on SID transitions. */
	current->pdeath_signal = 0;

	/* Check whether the new SID can inherit resource limits from the old
	 * SID.  If not, reset all soft limits to the lower of the current
	 * task's hard limit and the init task's soft limit.
	 *
	 * Note that the setting of hard limits (even to lower them) can be
	 * controlled by the setrlimit check.  The inclusion of the init task's
	 * soft limit into the computation is to avoid resetting soft limits
	 * higher than the default soft limit for cases where the default is
	 * lower than the hard limit, e.g. RLIMIT_CORE or RLIMIT_STACK.
	 */
	rc = avc_has_perm(new_tsec->osid, new_tsec->sid, SECCLASS_PROCESS,
			  PROCESS__RLIMITINH, NULL);
	if (rc) {
		for (i = 0; i < RLIM_NLIMITS; i++) {
			rlim = current->signal->rlim + i;
			initrlim = init_task.signal->rlim + i;
			rlim->rlim_cur = min(rlim->rlim_max, initrlim->rlim_cur);
		}
		update_rlimit_cpu(current,
				current->signal->rlim[RLIMIT_CPU].rlim_cur);
	}
#endif
}

/*
 * Clean up the process immediately after the installation of new credentials
 * due to exec
 */
static void kse_bprm_committed_creds(struct linux_binprm *bprm)
{
	const struct task_security_struct *tsec = current_security();
	//struct itimerval itimer;
	u32 osid, sid;
	//int rc, i;

	osid = tsec->osid;
	sid = tsec->sid;

	if (sid == osid)
		return;

#if 0
	/* Check whether the new SID can inherit signal state from the old SID.
	 * If not, clear itimers to avoid subsequent signal generation and
	 * flush and unblock signals.
	 *
	 * This must occur _after_ the task SID has been updated so that any
	 * kill done after the flush will be checked against the new SID.
	 */
	rc = avc_has_perm(osid, sid, SECCLASS_PROCESS, PROCESS__SIGINH, NULL);
	if (rc) {
		memset(&itimer, 0, sizeof itimer);
		for (i = 0; i < 3; i++)
			do_setitimer(i, &itimer, NULL);
		spin_lock_irq(&current->sighand->siglock);
		if (!(current->signal->flags & SIGNAL_GROUP_EXIT)) {
			__flush_signals(current);
			flush_signal_handlers(current, 1);
			sigemptyset(&current->blocked);
		}
		spin_unlock_irq(&current->sighand->siglock);
	}
#endif

	/* Wake up the parent if it is waiting so that it can recheck
	 * wait permission to the new task SID. */
	read_lock(&tasklist_lock);
	__wake_up_parent(current, current->real_parent);
	read_unlock(&tasklist_lock);
}

static int kse_bprm_secureexec(struct linux_binprm *bprm)
{
	const struct cred *cred = current_cred();
	const struct task_security_struct *tsec = cred->security;
	u32 sid, osid;
	int atsecure = 0;

	sid = tsec->sid;
	osid = tsec->osid;

#if 0
	if (osid != sid) {
		/* Enable secure mode for SIDs transitions unless
		   the noatsecure permission is granted between
		   the two SIDs, i.e. ahp returns 0. */
		atsecure = avc_has_perm(osid, sid,
					SECCLASS_PROCESS,
					PROCESS__NOATSECURE, NULL);
	}
#endif

	return (atsecure || cap_bprm_secureexec(bprm));
}
#endif

/*
 * allocate the KUXSE part of blank credentials
 */
static int kse_cred_alloc_blank(struct cred *cred, gfp_t gfp)
{
	struct task_security_struct *tsec;

#ifdef ZX
	printk("zx in alloc_blank\n");
	dump_stack();
#endif
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

#ifdef ZX
	printk("zx in cred_free\n");
	dump_stack();
#endif
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

#ifdef ZX
	printk("zx in cred prepare\n");
	dump_stack();
#endif
	old_tsec = old->security;

	tsec = kmemdup(old_tsec, sizeof(struct task_security_struct), gfp);
	if (!tsec)
		return -ENOMEM;

	/* for test*/
#if 0
	if (current_cred()->uid != 0) {
		tsec->mlevel.level_value = 1;
		tsec->ilevel.level_value = 1;
	}
#endif

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
#ifdef ZX
	printk("zx in cred transfer\n");
	dump_stack();
#endif

	*tsec = *old_tsec;
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

	.bprm_set_creds =		kse_bprm_set_creds,
	.bprm_committing_creds =	kse_bprm_committing_creds,
	.bprm_committed_creds =		kse_bprm_committed_creds,
	.bprm_secureexec =		kse_bprm_secureexec,

#endif
	.inode_alloc_security =		kse_inode_alloc_security,
	.inode_free_security =		kse_inode_free_security,
	.inode_init_security =		kse_inode_init_security,
#if 0
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
#endif
	.inode_setxattr =		kse_inode_setxattr,
	.inode_post_setxattr =		kse_inode_post_setxattr,
#if 0
	.inode_getxattr =		kse_inode_getxattr,
	.inode_listxattr =		kse_inode_listxattr,
	.inode_removexattr =		kse_inode_removexattr,
#endif
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
	.task_setpgid =			kse_task_setpgid,
	.task_getpgid =			kse_task_getpgid,
	.task_getsid =			kse_task_getsid,
	.task_getsecid =		kse_task_getsecid,
	.task_setnice =			kse_task_setnice,
	.task_setioprio =		kse_task_setioprio,
	.task_getioprio =		kse_task_getioprio,
	.task_setrlimit =		kse_task_setrlimit,
	.task_setscheduler =		kse_task_setscheduler,
	.task_getscheduler =		kse_task_getscheduler,
	.task_movememory =		kse_task_movememory,
	.task_kill =			kse_task_kill,
	.task_wait =			kse_task_wait,
	.task_to_inode =		kse_task_to_inode,

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

static __init int kse_init(void)
{
	if (!security_module_enable(&kse_ops)) {
		printk(KERN_INFO "KUXSE:  Initializing error.\n");
		kse_enabled = 0;
		return 0;
	}

	printk(KERN_INFO "KUXSE:  Initializing.\n");

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
