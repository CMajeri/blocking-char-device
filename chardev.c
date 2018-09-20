#include <linux/module.h> 
#include <linux/device.h> 
#include <linux/kernel.h> 
#include <linux/fs.h>     
#include <linux/uaccess.h>
#include <linux/sched.h>
#include <linux/time.h>

static int    majorNumber;
static DECLARE_WAIT_QUEUE_HEAD(wq);
static int flag = 0;
static int     dev_open(struct inode *, struct file *);
static int     dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);

static struct file_operations fops =
{
   .open = dev_open,
   .read = dev_read,
   .write = dev_write,
   .release = dev_release,
};

int init_module(void)
{
   printk(KERN_INFO "chardev: Initializing chardev LKM\n");
   majorNumber = register_chrdev(0, "chardev", &fops);
   if (majorNumber<0){
      printk(KERN_ALERT "chardev failed to register a major number\n");
      return majorNumber;
   }
   printk(KERN_INFO "chardev: registered correctly with major number %d\n", majorNumber);
   return 0;
}
void cleanup_module(void) 
{
   unregister_chrdev(majorNumber, "chardev");
   printk(KERN_INFO "Chardev: unregistered.\n");
}

static int dev_open(struct inode *inodep, struct file *filep)
{
   return 0;
}

static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset) 
{
    int error_count = 0;
    error_count = copy_to_user(buffer, "Hello\0", 6);
    wait_event(wq, flag != 0);
    flag = 0;
    printk(KERN_INFO "chardev: Wait done");
    if (error_count==0) {
        printk(KERN_INFO "chardev: Sent hello to the user\n");
        return 6;
    }
    else {
        printk(KERN_INFO "chardev: Failed to send hello characters to the user, %d\n", error_count);
        return -EFAULT;
    }
}

static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset)
{
    printk(KERN_INFO "chardev: Writing to device");
    flag = 1;
    wake_up(&wq);
    return len;
}

static int dev_release(struct inode *inodep, struct file *filep) 
{
   printk(KERN_INFO "Chardev: closed\n");
   return 0;
}

