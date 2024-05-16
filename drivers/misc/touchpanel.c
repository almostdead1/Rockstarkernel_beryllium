#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/init.h>
#include <linux/errno.h>

#define PROC_DIRNAME "touchpanel"
#define GESTURE_ENABLE_FILE "gesture_enable"

static struct proc_dir_entry *proc_dir;

static int __init touchpanel_init(void) {
  proc_dir = proc_mkdir(PROC_DIRNAME, NULL);
  if (!proc_dir) {
    return -ENOMEM; // Out of memory
  }

  if (proc_create(GESTURE_ENABLE_FILE, 0666, proc_dir, NULL) == NULL) {
    remove_proc_entry(PROC_DIRNAME, NULL);
    return -ENOMEM;
  }

  printk(KERN_INFO "Touch panel folder and gesture_enable file created successfully\n");
  return 0;
}

static void __exit touchpanel_exit(void) {
  printk(KERN_INFO "Touch panel folder and gesture_enable exit\n");
}

module_init(touchpanel_init);
module_exit(touchpanel_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Creates touchpanel folder and gesture_enable file in /proc");
