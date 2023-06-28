#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/linkage.h>
#include <linux/sched.h>
#include <linux/types.h>

SYSCALL_DEFINE1 (procsched,pid_t, pid)
{
    struct task_struct *task;
    struct sched_info *sched;

    task = find_task_by_vpid(pid);
    if (task == NULL){
        return -ESRCH;
    }

    sched = &(task->sched_info);
    return sched -> pcount;
}
