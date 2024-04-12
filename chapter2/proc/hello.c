#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>

#define BUFFER_SIZE 128
#define PROC_NAME "hello"

ssize_t proc_read(struct file *file, char __user *usr_buf,
    size_t count, loff_t *pos);

static struct file_operations proc_ops = {
    .owner = THIS_MODULE,
    .read = proc_read,
};

// called when module is loaded
int proc_init(void) {
    // creates /proc/hello entry
    printk(KERN_INFO "Gonz: module loding");
    // wrong line:
    // proc_create(PROC_NAME, 0666, NULL, &proc_ops);
    proc_create(PROC_NAME, 0666, NULL, (const struct proc_ops*)&proc_ops);
    return 0;
}

// called when module is removed
void proc_exit(void) {
    printk(KERN_INFO "Gonz: module removed");
    remove_proc_entry(PROC_NAME, NULL);
}

// called is time /proc/hello is read
ssize_t proc_read(struct file *file, char __user *usr_buf,
    size_t count, loff_t *pos) {

    int rv = 0;
    char buffer[BUFFER_SIZE];
    static int completed = 0;

    if (completed) {
        completed = 0;
        return 0;
    }

    completed = 1;

    rv = sprintf(buffer, "Hello world, Gonz!\n");

    // copies kernel space buffer to user space usr_buf
    copy_to_user(usr_buf, buffer, rv);

    return rv;
}


module_init(proc_init);
module_exit(proc_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Hello, world");
MODULE_AUTHOR("SGG");

