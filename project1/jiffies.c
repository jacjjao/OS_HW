#include <linux/jiffies.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/time64.h>
#include <linux/uaccess.h>
#include <linux/version.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 6, 0)
#define HAVE_PROC_OPS
#endif

#define PROCFS_NAME "jiffies"
#define SIZE 50

static struct proc_dir_entry *proc_file;

static ssize_t procfile_read(struct file *file_pointer, char __user *buffer,
                             size_t buffer_length, loff_t *offset) {
  unsigned long j;
  char buf[SIZE];
  int len;

  j = jiffies;
  len = snprintf(buf, SIZE, "%lu\n", j);

  if (*offset >= len || copy_to_user(buffer, buf, len)) {
    pr_info("copy_to_user failed\n");
    return 0;
  } else {
    pr_info("procfile read %s\n", file_pointer->f_path.dentry->d_name.name);
    *offset += len;
  }
  return len;
}

#ifdef HAVE_PROC_OPS

static const struct proc_ops proc_file_fops = {.proc_read = procfile_read};

#else

static const struct file_operations proc_file_fops = {

    .read = procfile_read

};

#endif

static int __init procfs2_init(void) {
  proc_file = proc_create(PROCFS_NAME, 0644, NULL, &proc_file_fops);

  if (NULL == proc_file) {
    pr_alert("Error:Could not initialize /proc/%s\n", PROCFS_NAME);
    return -ENOMEM;
  }

  pr_info("/proc/%s created\n", PROCFS_NAME);

  return 0;
}

static void __exit procfs2_exit(void) {
  proc_remove(proc_file);

  pr_info("/proc/%s removed\n", PROCFS_NAME);
}

module_init(procfs2_init);

module_exit(procfs2_exit);

MODULE_LICENSE("GPL");