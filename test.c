#include <unistd.h>
#include <sys/syscall.h>

#define MY_SYS_HELLO 451

int main(int argc, char *argv[]) {
    int ret = syscall(MY_SYS_HELLO);
    return 0;
}
