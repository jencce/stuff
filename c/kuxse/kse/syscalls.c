/*
 *  KUX security enhance(KSE) module
 *
 *  This file contains the KSE definenition of syscalls
 *
 *  Authors:  Zhou Xiong, <zhouxiong@inspur.com>
 *
 *  Copyright (C) 2013,2014 Inspur, inc
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License version 2,
 *	as published by the Free Software Foundation.
 */

#include <linux/cred.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/fs.h>
#include <linux/namei.h>

#include "kseobj.h"

/* sys_mac_task_ctl - get/set current task mac level
 * @cmd get/set 0/1.
 * @tss tss struct from user if needed.
 */
SYSCALL_DEFINE2(mac_task_ctl, int __user, cmd,
		struct task_security_struct __user *, tss)
{
	pr_debug("sys_mac_task_ctl in\n");

	if (tss == NULL || cmd < 0)
		return -EINVAL;
	
	if (cmd == 0) {                                  /* get */
		const struct cred *cred = current_cred();
		struct task_security_struct *sp;
		int ret = 0;

		sp = (struct task_security_struct *)cred->security;
		ret = copy_to_user(tss, sp, sizeof(struct task_security_struct));

		return ret ? -EFAULT : 0;

	} else {                                         /* set */

		struct task_security_struct tss1;
		struct task_security_struct *tsp;
		struct cred *new = NULL;
		int i;

		if (copy_from_user(&tss1, tss, sizeof(struct task_security_struct)))
			return -EFAULT;
		
		new = prepare_creds();
		if (new == NULL)
			return -ENOMEM;

		tsp = (struct task_security_struct *)new->security;
		tsp->mlevel.level_type = tss1.mlevel.level_type;
		tsp->mlevel.level_value = tss1.mlevel.level_value;
		tsp->mlevel.level_flag = tss1.mlevel.level_flag;
		tsp->mlevel.level_catsum = tss1.mlevel.level_catsum;

		for (i = 0; i < MAC_CAT_MAX; i++)
			tsp->mlevel.level_category[i] = tss1.mlevel.level_category[i];

		tsp->ilevel.level_value = tss1.ilevel.level_value;

		return commit_creds(new);
	}
}

/* sys_mac_file_ctl - get/set file mac level
 * @cmd get/set 0/1.
 * @name full file name
 * @tss tss struct from user if needed.
 */
SYSCALL_DEFINE3(mac_file_ctl, int __user, cmd, char __user *, name,
		struct user_inode_security_struct __user *, iss)
{
	struct user_inode_security_struct uiss;
	struct inode_security_struct *isp;
	struct inode *inode;
	struct path path;
	ssize_t error;
	int i;

	pr_debug("sys_mac_file_ctl in\n");

	if (iss == NULL || cmd < 0)
		return -EINVAL;
	
	error = user_path(name, &path);
	if (error)
		return error;

	if (cmd == 0) {                                  /* get */
		int ret = 0;

		inode = path.dentry->d_inode;
		isp = inode->i_security;

		uiss.mlevel.level_type = isp->mlevel.level_type;
		uiss.mlevel.level_value = isp->mlevel.level_value;
		uiss.mlevel.level_flag = isp->mlevel.level_flag;
		uiss.mlevel.level_catsum = isp->mlevel.level_catsum;
		for (i = 0; i < MAC_CAT_MAX; i++)
			uiss.mlevel.level_category[i] = isp->mlevel.level_category[i];
		uiss.ilevel.level_value = isp->ilevel.level_value;
		uiss.task_sid = isp->task_sid;
		uiss.initialized = isp->initialized;

		path_put(&path);
		ret = copy_to_user(iss, &uiss, sizeof(struct user_inode_security_struct));

		return ret ? -EFAULT : 0;

	} else {                                         /* set */

		if (copy_from_user(&uiss, iss, sizeof(struct user_inode_security_struct)))
			return -EFAULT;

		inode = path.dentry->d_inode;
		isp = inode->i_security;

		mutex_lock(&isp->lock);
		isp->mlevel.level_type = uiss.mlevel.level_type;
		isp->mlevel.level_value = uiss.mlevel.level_value;
		isp->mlevel.level_flag = uiss.mlevel.level_flag;
		isp->mlevel.level_catsum = uiss.mlevel.level_catsum;

		for (i = 0; i < MAC_CAT_MAX; i++)
			isp->mlevel.level_category[i] = uiss.mlevel.level_category[i];

		isp->ilevel.level_value = uiss.ilevel.level_value;
		mutex_unlock(&isp->lock);
		path_put(&path);

		mark_inode_dirty_sync(inode);
		
		return 0;
	}
}
