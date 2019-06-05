#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/init.h>		/* Needed for the macros */

MODULE_LICENSE("GPL");            ///< The license type -- this affects available functionality
MODULE_AUTHOR("Matias Navarro");    ///< The author -- visible when you use modinfo
MODULE_DESCRIPTION("Mi primer Driver");  ///< The description -- see modinfo
MODULE_VERSION("0.1");            ///< A version number to inform users

static int __init hello_2_init(void)
{
	printk(KERN_INFO "Hola Mundo!\n");
	return 0;
}

static void __exit hello_2_exit(void)
{
	printk(KERN_INFO "Adios Mundo Cruel\n");
}

module_init(hello_2_init);
module_exit(hello_2_exit);
