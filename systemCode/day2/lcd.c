#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc,char** argv)
{
    // 0.验证参数
    if(argc < 2)
    {
        fprintf(stderr,"Usage <%s> color\n",argv[0]);
        return -1;
    }
    // 1.打开文件
    int lcdfd = open("/dev/fb0",O_WRONLY);
    if(-1 == lcdfd)
    {
        perror("open");
        return -1;
    }

    // 2.操作屏幕
    __uint32_t color;
    switch (argv[1][0])
    {
    case 'r':
        /* code */
        color = 0x00FF0000;
        break;
    
    case 'g':
        /* code */
        color = 0x0000FF00;
        break;

    case 'b':
        /* code */
        color = 0x000000FF;
        break;

    default:
        break;
    }

    for(int i=0;i<800 * 480;i++)
    {
        write(lcdfd,&color,sizeof(color));
    }

    // 3.关闭文件
    close(lcdfd);

    return 0;
}