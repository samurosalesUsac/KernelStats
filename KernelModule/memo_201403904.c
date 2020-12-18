#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/kernel.h>   
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/utsname.h>
#include <linux/mm.h>
#include <linux/swapfile.h>
#include <linux/seq_file.h>
#define BUFSIZE  1000

struct sysinfo i;
	
struct task_struct *task, *curr, *idle, *stop; //Pointer to the task whose info will be printed
int procX = 0, procS = 0, procD = 0, procZ = 0 , procT = 0;
static int load_file (struct seq_file *m, void *v) {

	si_meminfo(&i);
	// unsigned long totalSize = ((get_mm_rss(i.to)) << (PAGE_SHIFT - 10));
    seq_printf(m,"\n \"ram\" : {\"total\":%ld, \"libre\": %ld}\n}", i.totalram, i.freeram);
	return 0;
}

static int myOpen(struct inode *inode, struct  file *file) {
  return single_open(file, load_file, NULL);
}

static struct proc_ops myops = 
{
	.proc_open = myOpen,
	.proc_read = seq_read,
};

/* This function is called when the module is loaded. */
static int entryPoint(void) {

	
	proc_create("memo_201403904",0,NULL,&myops);
	printk(KERN_INFO "201403904\n");
	return 0;
}

/* This function is called when the module is removed. */
static void exitPoint(void) {
	
	// printk(KERN_INFO "“Sayonara mundo, somos el grupo 18\n");
    remove_proc_entry("memo_201403904",NULL);
	printk(KERN_INFO "Sistemas Operativos 1.\n");
}


/* Macros for registering module entry and exit points. */
module_init(entryPoint);
module_exit(exitPoint);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Stats de Ram");
MODULE_AUTHOR("Samuel Rosales - 201403904");

