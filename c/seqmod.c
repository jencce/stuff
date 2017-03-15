#include <linux/module.h>
#include <linux/seq_file.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <asm/uaccess.h>

struct t_node {
	int v;
	char c;
};

struct t_node *sqt_data;

static int counts, cleanall = 0;

static int t_show(struct seq_file *m, void *v)
{
	int i;
	struct t_node *tn = v;

	seq_printf(m, "%d %c, ", tn->v, tn->c);
	return 0;
}

static void *t_stop(struct seq_file *m, void *v)
{
	seq_printf(m, "\n");
	return NULL;
}

static void *t_start(struct seq_file *m, loff_t *pos)
{
#if 1
	printk("%ld start pos %d count %d m %p index %d\n", current->pid, *pos, counts, m, m->index);
	if (*pos == 12 && m->index == 0 && cleanall == 0) {
		printk("%ld flag\n", current->pid);
		cleanall = 1;
		m->index = 4;
		msleep(10000);
	}
#else
	*pos = 0;
	m->index = 4;
#endif
	if (*pos <= counts && sqt_data != NULL)
		return &sqt_data[*pos];
	return NULL;
}

static void *t_next(struct seq_file *m, void *v, loff_t *pos)
{
	(*pos)++;
	return t_start(m, pos);
}

const struct seq_operations sqt_op = {
	.start = t_start,
	.next = t_next,
	.stop = t_stop,
	.show = t_show,
};

static int sqt_open(struct inode *inode, struct file *file)
{
	return seq_open(file, &sqt_op);
}

static const struct file_operations sm_ops = {
	.open = sqt_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = seq_release,
};

static int seq_thread(void *v)
{
	int i;

	//msleep(5000);
	for (i = 0; i < 12; i++) {
		//msleep(2000);
		counts = i;
		if (sqt_data) {
			sqt_data = (struct t_node *)krealloc(sqt_data, (i + 1)*sizeof(struct t_node), GFP_KERNEL);
			//printk("realloced data\n");
		} else {
			sqt_data = (struct t_node *)kmalloc((i + 1)*sizeof(struct t_node), GFP_KERNEL);
			//printk("malloced data\n");
			sqt_data[0].v = 23;
			sqt_data[0].c = 'x';
		}
		if (sqt_data == NULL)
			return 1;
		sqt_data[i].v = i;
		sqt_data[i].c = 'a' + i;
#if 0
		if (i == 5) {
			counts = 0;
			//sqt_data = (struct t_node *)krealloc(sqt_data, sizeof(struct t_node), GFP_KERNEL);
			if (sqt_data) {
				kfree(sqt_data);
				sqt_data = NULL;
				//printk("freed data\n");
				msleep(6000);
			}
		}
#endif
	}
	printk("%d kthread exit\n", current->pid);
	return 0;
}

static struct task_struct *sqts;

static int seqmod_init(void)
{
	proc_create("tsq", S_IRUGO, NULL, &sm_ops);
	sqts = kthread_create(seq_thread, NULL, "%s", "sqts");
	if (! IS_ERR(sqts))
		wake_up_process(sqts);
	printk("%ld seqmod init\n", current->pid);
	return 0;
}

static int seqmod_exit(void)
{
	remove_proc_entry("tsq", NULL);
	if (sqt_data)
		kfree(sqt_data);
	printk("%ld seqmod exit\n", current->pid);
	return 0;
}

MODULE_LICENSE("GPL");
module_init(seqmod_init);
module_exit(seqmod_exit);
