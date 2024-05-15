// SPDX-License-Identifier: GPL-2.0 OR BSD-2-Clause
/*
 * Touch panel gesture module
 *
 * Copyright (C) 2024, Your Name (replace with your name)
 */

#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/uaccess.h>

#define PROC_DIRNAME "touchpanel"
#define GESTURE_ENABLE_FILE "gesture_enable"

static struct proc_dir_entry *proc_dir;

static ssize_t gesture_enable_write(struct file *file, const char __user *user_buf,
                                   size_t count, loff_t *f_pos) {
  char buffer[3]; // To accommodate null terminator
  int ret;

  if (count > sizeof(buffer) - 1) {
    return -EINVAL;
  }

  ret = copy_from_user(buffer, user_buf, count);
  if (ret) {
    return ret;
  }

  // Ensure null termination for proper string comparison
  buffer[count] = '\0';

  if (strcmp(buffer, "0x80, 0x0") != 0) {
    return -EINVAL;
  }

  *f_pos += count;
  return count;
}

static const struct file_operations gesture_enable_ops = {
  .write = gesture_enable_write,
};

static int __init touchpanel_init(void) {
  struct proc_dir_entry *gesture_enable;

  proc_dir = proc_mkdir(PROC_DIRNAME, NULL);
  if (!proc_dir) {
    return -ENOMEM;
  }

  gesture_enable = proc_create(GESTURE_ENABLE_FILE, 0666, proc_dir,
                               &gesture_enable_ops);
  if (!gesture_enable) {
    remove_proc_entry(PROC_DIRNAME, NULL);
    return -ENOMEM;
  }

  // Write "0x80, 0x0" to the gesture_enable file on creation
  ret = kernel_write(gesture_enable->data, buffer_start(gesture_enable->inode),
                     "0x80, 0x0", sizeof("0x80, 0x0"));
  if (ret != sizeof("0x80, 0x0")) {
    remove_proc_entry(GESTURE_ENABLE_FILE, proc_dir);
    remove_proc_entry(PROC_DIRNAME, NULL);
    return -EIO;
  }

  printk(KERN_INFO "Touch panel gesture module initialized");
  return 0;
}

static void __exit touchpanel_exit(void) {
  remove_proc_entry(GESTURE_ENABLE_FILE, proc_dir);
  remove_proc_entry(PROC_DIRNAME, NULL);
  printk(KERN_INFO "Touch panel gesture module exited");
}

module_init(touchpanel_init);
module_exit(touchpanel_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Creates touchpanel folder and gesture_enable file with content in /proc");
