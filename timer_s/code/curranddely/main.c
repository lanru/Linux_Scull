/*************************************************************************
	> File Name: main.c
	> Author: 
	> Mail: 
	> Created Time: 2017年03月24日 星期五 11时41分42秒
 ************************************************************************/

//#include <linux/config.h>
#include <linux/module.h>
#include <linux/ktime.h>
#include <linux/timekeeping.h>
#include <linux/moduleparam.h>
#include <linux/init.h>

#include <linux/kernel.h>	/* printk() */
#include <linux/slab.h>		/* kmalloc() */
#include <linux/errno.h>	/* error codes */
#include <linux/types.h>	/* size_t */
#include <linux/proc_fs.h>
#include <linux/fcntl.h>	/* O_ACCMODE */
#include <linux/seq_file.h>  /*创建/proc 文件以用来和用户交互数据*/
#include <linux/cdev.h>
#include <linux/proc_ns.h>  /*proc 在新版中移动到了此目录下*/
//#include <asm/system.h>		/* cli(), *_flags */
#include <asm/uaccess.h>	/* copy_*_user */
#include <net/snmp.h>
#include <linux/ipv6.h>
#include <net/if_inet6.h>
#include "scull.h"		/* local definitions */

int scull_major = SCULL_MAJOR;
int scull_minor = 0;
int scull_nr_devs = SCULL_NR_DEVS;
int scull_quantum = SCULL_QUANTUM;
int scull_qset    = SCULL_QSET;


int delay = HZ; /* the default delay, expressed in jiffies */

module_param(delay, int, 0);

MODULE_AUTHOR("Alessandro Rubini");
MODULE_LICENSE("Dual BSD/GPL");

/* use these as data pointers, to implement four files in one function */
enum jit_files {
	JIT_BUSY,
	JIT_SCHED,
	JIT_QUEUE,
	JIT_SCHEDTO
};



static void *scull_seq_start(struct seq_file *s, loff_t *pos)
{
return NULL;
}

static void *scull_seq_next(struct seq_file *s, void *v, loff_t *pos)
{
return NULL;
}

static void scull_seq_stop(struct seq_file *s, void *v)
{
	/* Actually, there's nothing to do here */
return ;
}


static void* dely_on(struct seq_file *s, void *v,int flag){
	unsigned long j0, j1; /* jiffies */
	wait_queue_head_t wait;

	init_waitqueue_head (&wait);
	j0 = jiffies;
	j1 = j0 + 10;

	switch((long)flag) {
		case JIT_BUSY:
			while (time_before(jiffies, j1))
			break;
		case JIT_SCHED:
			while (time_before(jiffies, j1)) {
				schedule();
			}
			break;
		case JIT_QUEUE:
			wait_event_interruptible_timeout(wait, 0, delay);
			break;
		case JIT_SCHEDTO:
			set_current_state(TASK_INTERRUPTIBLE);
			schedule_timeout (delay);
			break;
	}
	j1 = jiffies; /* actual value after we delayed */

	//len = sprintf(buf, "%9li %9li\n", j0, j1);
	//*start = buf;
	return flag;


}


static int scull_seq_show(struct seq_file *s, void *v)
{
	// struct timespec tv2;
	//   printk("scull_seq_show");
	   seq_printf(s, "My data: ");
	   return 0;
	   
	unsigned long j1;
	u64 j2;

	/* get them four */
	j1 = jiffies;
	j2 = get_jiffies_64();
	ktime_t  tv1 = ktime_get();
	// tv2 = current_kernel_time();

    
	dely_on(s,v,JIT_BUSY);
	

 	seq_printf(s,"0x%08lx 0x%016Lx %lld\n",
		       j1, j2,
		       (long long) tv1);

	//*start = buf;
	return 0;
}



/*便利设备链表*/
struct scull_qset *scull_follow(struct scull_dev *dev, int n)
{

	return 0;
}



/*
 * Tie the sequence operators up.
 */
static struct seq_operations scull_seq_ops = {
	.start = scull_seq_start,
	.next  = scull_seq_next,
	.stop  = scull_seq_stop,
	.show  = scull_seq_show
};

/*
 * Now to implement the /proc file we need only make an open
 * method which sets up the sequence operators.
 */
static int scull_proc_open(struct inode *inode, struct file *file)
{
	return seq_open(file, &scull_seq_ops);
}

/*
 * Create a set of file operations for our proc file.
 */
static struct proc_ops scull_proc_ops = {
	// .owner   = THIS_MODULE,
	.proc_open    = scull_proc_open,
	.proc_read    = seq_read,
	.proc_lseek  = seq_lseek,
	.proc_release = seq_release
};
	

/*proc 文件的创建与移除*/

static void scull_create_proc(void)
{
	struct proc_dir_entry *entry;

    proc_create("currentime",0,NULL,&scull_proc_ops);

    printk("create mode");
	
}

static void scull_remove_proc(void)
{
	remove_proc_entry("currentime", NULL /* parent dir */);
}


/*_______________________________________________________________________________________*/


void scull_cleanup_module(void)
{
    int devno;
    scull_remove_proc();/*创建测试的proc 文件*/
	
	unregister_chrdev_region(devno, scull_nr_devs);

	

}






int scull_init_module(void)   /*获取主设备号，或者创建设备编号*/
{
  
    dev_t dev;
    int result;
    if(scull_major){
        dev = MKDEV(scull_major,scull_minor);     
        result = register_chrdev_region(dev,scull_nr_devs,"scull");
    }else{
        result = alloc_chrdev_region(&dev,scull_minor,scull_nr_devs,"scull");
        scull_major = MAJOR(dev);
    }
  
    scull_create_proc(); 
 	printk("scull_init_module");
    return 0;


}





module_init(scull_init_module);
module_exit(scull_cleanup_module);

