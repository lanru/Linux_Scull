#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/time.h>
#include <linux/timer.h>
#include <linux/workqueue.h>
#include <asm/atomic.h>

#include <linux/module.h>

#include <linux/timer.h>

#include <linux/jiffies.h>

 

struct timer_list mytimer;

 

static void myfunc(struct timer_list *timer)

{

        printk("my_timer_callback called\n");

       mod_timer(&mytimer, jiffies + msecs_to_jiffies(1000));

}

 

static int __init mytimer_init(void)

{

        timer_setup(&mytimer, myfunc, (long unsigned int)"Hello, world!");

        mytimer.expires = jiffies + 10;

        add_timer(&mytimer);

 

        return 0;

}

 

static void __exit mytimer_exit(void)

{

        del_timer(&mytimer);

}

 

module_init(mytimer_init);

module_exit(mytimer_exit);
