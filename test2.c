#include <unistd.h>
#include <stdio.h>
#include <sys/syscall.h>

#define MY_SYS_PROCSCHED 452

int main(int argc, char *argv[]) {
    int ret = syscall(MY_SYS_PROCSCHED, 2274);
    printf("pcound of 1234 = %d \n", ret);
    return 0;
}
