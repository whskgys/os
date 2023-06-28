#include <linux/kernel.h>
#include <linux/syscalls.h>

SYSCALL_DEFINE0(hello)
{
    printk("hello, nahyun\n");
    printk("211852\n");
    return 0;
}
