#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>


#define FIFO_PATH "/tmp/fifo"
// 数据传输结构体
typedef struct {
    int len;
    char buf[256];
} Packet;


int main(int argc,char** argv)
{
    if(argc < 3)
    {
        fprintf(stderr,"Usage:\n    <%s> srcFile desFilname\n",argv[0]);
        return -1;
    }

    // 1.打开源文件
    int fd = open(argv[1],O_RDWR);
    if(-1 == fd)
    {
        perror("open");
        return -1;
    }

    // 2.创建子进程
    pid_t cpid = fork();
    if(cpid < 0)
    {
        perror("fork");
        return -1;
    }else if(cpid == 0)
    {
        // 3.子进程
        char envpBuf[50]={"DESFILENAME="};
        strcat(envpBuf,argv[2]);
        // 创建子进程，并传递desFilname
        char *const envp[]={
            envpBuf,
            NULL
        };
        char *const argp[]={
            "./work_rd",
            NULL
        };
        if(-1 == execve("./work_rd",argp,envp))
        {
            perror("execve");
            return -1;
        }
    }else{
        // 4.父进程
        // 4.1创建管道,4读，2写，1执行
        if(-1 == mkfifo(FIFO_PATH,0644))
        {
            perror("mkfifo");
            close(fd);
            return -1;
        }
        int fd_fifo = open(FIFO_PATH,O_RDWR);
        // 4.2父进程发送数据
        Packet sendData;
        sendData.len = 0;
        while(1)
        {
            sendData.len = read(fd,sendData.buf,sizeof(sendData.buf));
            // 写入管道
            write(fd_fifo,&sendData,sizeof(sendData));
            // 读取到文件末尾
            if(sendData.len == 0) break;
        }

        // 4.3父进程等待子进程接受数据
        int status = 0;
        wait(&status);
        if(WEXITSTATUS(status) == 0)
        {
            printf("转存成功\n");
        }else 
            printf("转存失败\n");
        close(fd_fifo);
    }
    // 5.退出操作
    unlink(FIFO_PATH);
    close(fd);

    return 0;
}