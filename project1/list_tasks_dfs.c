#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/types.h>
#include <linux/uaccess.h>
#include <linux/version.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 6, 0)
#define HAVE_PROC_OPS
#endif

#define SIZE 50

#define PROCFS_NAME "list_tasks_dfs"

static struct proc_dir_entry* proc_file;

typedef struct {
  struct list_head* tasks[SIZE];
  size_t count;
} task_stack;

static void hw_push_stack(task_stack* stack, struct list_head* task) {
  if (stack->count >= SIZE) {
    pr_info("The stack is full\n");
    return;
  }
  stack->tasks[stack->count] = task;
  ++stack->count;
}

static void hw_pop_stack(task_stack* stack) {
  if (stack->count == 0) {
    pr_info("The stack is empty\n");
    return;
  }
  --stack->count;
}

static struct list_head* hw_top_stack(task_stack* stack) {
  if (stack->count == 0) {
    pr_info("The stack is empty\n");
    return NULL;
  }
  return stack->tasks[stack->count - 1];
}

static ssize_t procfile_read(struct file* file_pointer, char __user* buffer,
                             size_t buffer_length, loff_t* offset) {
  task_stack stack;
  struct task_struct* task;

  stack.count = 0;
  task = &init_task;
  do {
    if (list_empty(&task->children) ||
        (stack.count > 0 &&
         (&task->children) == hw_top_stack(&stack))) {  // no child or visited
      if (&task->children ==
          hw_top_stack(&stack)) {  // if we have visit the task
        hw_pop_stack(&stack);
      } else {
        pr_info("name: %-16s, state: %-6u, pid: %-6d\n", task->comm,
                task->__state, task->pid);
      }
      if (task->sibling.next ==
          hw_top_stack(&stack)) {  // if next sibling is parent
        task = list_entry(task->sibling.next, struct task_struct, children);
      } else {
        task = list_entry(task->sibling.next, struct task_struct, sibling);
      }
    } else {  // have child and haven't yet visit
      pr_info("name: %-16s, state: %-6u, pid: %-6d\n", task->comm,
              task->__state, task->pid);
      hw_push_stack(&stack, &task->children);
      task = list_entry(task->children.next, struct task_struct, sibling);
    }
  } while (task && task != &init_task);

  return 0;
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