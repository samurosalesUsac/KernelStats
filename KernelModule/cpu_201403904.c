#include <linux/sched.h>
#include <linux/sched/signal.h>
#include <linux/sched/task.h>
#include <linux/moduleparam.h>
#include <linux/fs.h>
#include <linux/seq_file.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/init.h>
#include <linux/mm.h>

#include <linux/list.h>


#include <linux/kernel.h>
#include <linux/module.h>

	
struct task_struct *task, *curr, *idle, *stop; //Pointer to the task whose info will be printed
int procX = 0, procS = 0, procD = 0, procZ = 0 , procT = 0;
struct task_struct *task_child;
struct list_head *list;


static int load_file (struct seq_file *m, void *v) {

	seq_printf(m, "{\"cpu\" : {\n");

	for_each_process(curr) { //Loop over the tasks using the macro for_each_process
		
		if(curr->state == 0){
			procX++;
		}else if(curr->state == 1 || curr->state == 1026){
			procS++;
		}else if(curr->state == 128){
			procD++;
		}else if(curr->state == 260){
			procZ++;
		}
		procT++;
	}

	seq_printf(m, "\"procCount\" : {\"procX\" : %d,\"procS\" : %d,\"procD\" : %d,\"procZ\" : %d,\"procT\" : %d},\n\"procList\" : [\n", procX, procS, procD, procZ, procT);

	for_each_process(task) { //Loop over the tasks using the macro for_each_process

		/* on each iteration task points to the next task */
		//task->comm is the task' name
		//task->state is the task's state (-1 unrunnable, 0 runnable, >0 stopped)
		//task->pid is the task's process ID
		//printk(KERN_INFO "Name: %-20s State: %ld\tProcess ID: %d\n", task->comm, task->state, task->pid);
		unsigned long size = 0;
		unsigned long Tsize = 0;
		if(task->mm != NULL){
			size = (task->mm->mmap->vm_end - task->mm->mmap->vm_start);
			// Tsize = task->mm->total_vm;
			Tsize = ((get_mm_rss(task->mm)) << (PAGE_SHIFT - 10));
		}
		seq_printf(m, "{\"User\" : \"%-20d\",\t\"Name\" : \"%-20s\", \t\"State\" : %ld,\t\"ID\": %d,\t\"ramm\": %ld,\n",task->cred->uid.val, task->comm, task->state, task->pid, Tsize);

		seq_printf(m, "\"childs\" : [");

		list_for_each(list, &task->children){
			task_child = list_entry(list, struct task_struct, sibling);
			
			seq_printf(m, "{\"Name\" : \"%-20s\",\t\"ID\": %d},\n", task_child->comm, task_child->pid);

		}
		seq_printf(m, "{}]},");
	}


	seq_printf(m, "{}]\n");
	seq_printf(m, "\n},\n");
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

	
	proc_create("cpu_201403904",0,NULL,&myops);
	printk(KERN_INFO "Samuel Rosales\n");
	return 0;
}

/* This function is called when the module is removed. */
static void exitPoint(void) {
	
	// printk(KERN_INFO "“Sayonara mundo, somos el grupo 18\n");
    remove_proc_entry("cpu_201403904",NULL);
	printk(KERN_INFO "Diciembre 2020.\n");
}


/* Macros for registering module entry and exit points. */
module_init(entryPoint);
module_exit(exitPoint);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Lista de Procesos");
MODULE_AUTHOR("Samuel Rosales - 201403904");

