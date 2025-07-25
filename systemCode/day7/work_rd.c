#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdlib.h>

#define FIFO_PATH "/tmp/fifo"

// 数据传输结构体
typedef struct {
    int len;
    char buf[256];
} Packet;

int main(int argc,char** argv)
{
    // 1.接收传进来的目标文件名
    char *desFilname = getenv("DESFILENAME");

    // 2.创建目标文件
    int fd_write = open(desFilname,O_RDWR | O_CREAT,0644);
    if(fd_write == -1)
    {
        perror("write open");
        exit(-1);
    }

    // 3.打开管道接收文件
    int fd_fifo = open(FIFO_PATH,O_RDWR);
    if(-1 == fd_fifo)
    {
        perror("write fifo open");
        close(fd_write);
        exit(-1);
    }
    Packet revData;
    while(read(fd_fifo,&revData,sizeof(revData)) > 0)
    {
        if(revData.len == 0) break;
        write(fd_write,revData.buf,revData.len);
    }

    //4.结束处理
    close(fd_fifo);
    close(fd_write);
    return 0;
}