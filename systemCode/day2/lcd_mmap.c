#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdint.h>

#define LEN (800 * 480)

int main(int argc,char** argv)
{
    // 0.验证参数
    if(argc < 2)
    {
        fprintf(stderr,"Usage <%s> color\n",argv[0]);
        return -1;
    }
    // 1.打开文件
    int lcdfd = open("/dev/fb0",O_RDWR);
    if(-1 == lcdfd)
    {
        perror("open");
        return -1;
    }

    // 2.操作屏幕
    uint32_t color;
    switch (argv[1][0])
    {
    case 'r':
        color = 0x00FF0000;
        break;
    
    case 'g':
        color = 0x0000FF00;
        break;

    case 'b':
        color = 0x000000FF;
        break;

    default:
        break;
    }

    // mmap
    uint32_t * addr=(uint32_t*)mmap(NULL,LEN * 4,PROT_WRITE | PROT_READ,MAP_SHARED,lcdfd,0);
    if(addr == MAP_FAILED)
    {
        perror("mmap");
        close(lcdfd);
        return -1;
    }

    for(int i=0;i<LEN;i++)
    {
       addr[i] = color; 
    }

    // 3.关闭文件
    munmap(addr,LEN*4);
    close(lcdfd);

    return 0;
}