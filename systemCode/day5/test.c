#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    int data = 42;  // 初始共享数据
    pid_t pid = fork();

    if (pid == 0) {  // 子进程
        data = 100;  // 触发 COW：复制父进程的 data 所在内存页
        printf("子进程 data=%d\n", data); // 100
    } else {         // 父进程
        sleep(1);    // 等待子进程先修改
        printf("父进程 data=%d\n", data); // 42（未被子进程修改影响）
    }
    return 0;
}