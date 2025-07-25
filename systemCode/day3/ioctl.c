#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <stdio.h>

int main(int argc,char* argv[])
{
    int lcdfd = open("/dev/fb0",O_RDWR);
    if(lcdfd == -1)
    {
        perror("open");
        return -1;
    }
    struct fb_var_screeninfo  scinfo = {0};
    ioctl(lcdfd,FBIOGET_VSCREENINFO,&scinfo);

    printf("Resolution:%u * %u\n",scinfo.xres,  scinfo.yres);
    close(lcdfd);

    return 0;
}