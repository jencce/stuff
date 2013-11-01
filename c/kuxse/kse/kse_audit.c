/*
 * Implementation of the KUXSE audit module
 *
 * Authors:  zhouxiong <zhouxiong@inspur.com>
 *
 *
 * Copyright (C) 2013  Inspur, Inc.
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License version 2,
 *	as published by the Free Software Foundation.
 */
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/audit.h>
#include <linux/lsm_audit.h>

#include "classmap.h"
#include "kseobj.h"


/**
 * kse_dump_av - Display an access vector in human-readable form.
 * @tclass: target security class
 * @av: access vector
 */
static void kse_dump_av(struct audit_buffer *ab, struct common_audit_data *ad)
{
	const char **perms;
	int i, perm;
	int rq = ad->kse_audit_data.requested;
	int tclass = ad->kse_audit_data.tclass;

	if (rq == 0) {
		audit_log_format(ab, " null");
		return;
	}

	perms = secclass_map[tclass-1].perms;

	audit_log_format(ab, " {");
	i = 0;
	perm = 1;
	while (i < (sizeof(u32) * 8)) {
		if ((perm & rq) && perms[i]) {
			audit_log_format(ab, " %s", perms[i]);
			rq &= ~perm;
		}
		i++;
		perm <<= 1;
	}

	if (rq)
		audit_log_format(ab, " 0x%x", rq);

	audit_log_format(ab, " }");
}

/**
 * kse_dump_query - Display a SID pair and a class in human-readable form.
 * @ssid: source security identifier
 * @tsid: target security identifier
 * @tclass: target security class
 */
static void kse_dump_query(struct audit_buffer *ab, struct common_audit_data *ad)
{
	int rc;
	char *scontext;
	u32 scontext_len;
	struct task_security_struct *tss1 = ad->kse_audit_data.tss1;
	int flag = ad->kse_audit_data.flag;
	int tclass = ad->kse_audit_data.tclass;

	if (tss1 == NULL)
		return;

	rc = iss_to_context(0, &tss1->mlevel, &tss1->ilevel, &scontext, &scontext_len);
	if (rc)
		audit_log_format(ab, "source type %d ml %d il %d ",
					tss1->mlevel.level_type,
					tss1->mlevel.level_value,
					tss1->ilevel.level_value);
	else {
		audit_log_format(ab, "scontext=%s ", scontext);
		kfree(scontext);
	}

	if (flag == 0) {
		struct task_security_struct *tss2 = ad->kse_audit_data.tss2;

		if (tss2 == NULL)
			return;

		rc = iss_to_context(0, &tss2->mlevel, &tss2->ilevel, &scontext, &scontext_len);
		if (rc)
			audit_log_format(ab, "target type %d ml %d il %d ",
						tss2->mlevel.level_type,
						tss2->mlevel.level_value,
						tss2->ilevel.level_value);
		else {
			audit_log_format(ab, "tcontext=%s ", scontext);
			kfree(scontext);
		}
	} else if (flag == 1) {
		struct inode_security_struct *iss = ad->kse_audit_data.iss;

		if (iss == NULL)
			return;

		rc = iss_to_context(0, &iss->mlevel, &iss->ilevel, &scontext, &scontext_len);
		if (rc)
			audit_log_format(ab, "target type %d ml %d il %d ",
						iss->mlevel.level_type,
						iss->mlevel.level_value,
						iss->ilevel.level_value);
		else {
			audit_log_format(ab, "tcontext=%s ", scontext);
			kfree(scontext);
		}
	}

	BUG_ON(tclass >= ARRAY_SIZE(secclass_map));
	audit_log_format(ab, " tclass=%s", secclass_map[tclass-1].name);
}

/**
 * kse_audit_pre_callback - SELinux specific information
 * will be called by generic audit code
 * @ab: the audit buffer
 * @a: audit_data
 */
static void kse_audit_pre_callback(struct audit_buffer *ab, void *a)
{
	struct common_audit_data *ad = a;
	pr_debug("avc:  %s ",
			 ad->kse_audit_data.result ? "denied" : "granted");
	audit_log_format(ab, "avc:  %s ",
			 ad->kse_audit_data.result ? "denied" : "granted");
	kse_dump_av(ab, ad);
	audit_log_format(ab, " for ");
}

/**
 * kse_audit_post_callback - SELinux specific information
 * will be called by generic audit code
 * @ab: the audit buffer
 * @a: audit_data
 */
static void kse_audit_post_callback(struct audit_buffer *ab, void *a)
{
	struct common_audit_data *ad = a;
	pr_debug("kse post \n");
	audit_log_format(ab, " ");
	kse_dump_query(ab, ad);
}

/**
 * kse_audit - Audit the granting or denial of permissions.
 * @ssid: source security identifier
 * @tsid: target security identifier
 * @tclass: target security class
 * @requested: requested permissions
 * @avd: access vector decisions
 * @result: result from kse_has_perm_noaudit
 * @a:  auxiliary audit data
 *
 * Audit the granting or denial of permissions in accordance
 * with the policy.  This function is typically called by
 * kse_has_perm() after a permission check, but can also be
 * called directly by callers who use kse_has_perm_noaudit()
 * in order to separate the permission check from the auditing.
 * For example, this separation is useful when the permission check must
 * be performed under a lock, to allow the lock to be released
 * before calling the auditing code.
 */
void kse_audit(struct task_security_struct *tss1,
		struct task_security_struct *tss2,
		struct inode_security_struct *iss,
		int flag, int tclass, int requested,int result,
		struct common_audit_data *a)
{
	struct common_audit_data stack_data;

	if (!a) {
		a = &stack_data;
		memset(a, 0, sizeof(*a));
		a->type = LSM_AUDIT_NO_AUDIT;
	}
	a->kse_audit_data.tclass = tclass;
	a->kse_audit_data.requested = requested;
	a->kse_audit_data.tss1 = tss1;
	a->kse_audit_data.flag = flag;
	
	if(flag == 0) {
		a->kse_audit_data.tss2 = tss2;
		a->kse_audit_data.iss = NULL;
	} else if(flag == 1) {
		a->kse_audit_data.tss2 = NULL;
		a->kse_audit_data.iss = iss;
	}

	a->kse_audit_data.result = result;
	a->lsm_pre_audit = kse_audit_pre_callback;
	a->lsm_post_audit = kse_audit_post_callback;
	common_lsm_audit(a);
}
