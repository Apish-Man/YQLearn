#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/mman.h>

#define LEN (800 * 480)

int drawbmp(const char *file)
{
    // 1.打开文件
    FILE* fp_bmp = fopen(file,"r");
    if(fp_bmp == NULL)
    {
        perror("fopen");
        return -1;
    }
    // 2.获取像素数据
    fseek(fp_bmp,54,SEEK_SET);
    char* filebuf=(char *)malloc((LEN+1) * 3 *sizeof(char));
    if(!filebuf)
    {
        fprintf(stderr,"%s error\n","缓冲区开辟");
        return -1;
    }
    fread(filebuf,sizeof(char)*LEN * 3,1,fp_bmp);
    fclose(fp_bmp);

    // 3.数据打印到进程空间的映射区
    // 3.1 打开设备文件
    int fd_oled = open("/dev/fb0",O_RDWR);
    if(-1 == fd_oled)
    {
        perror("open");
        free(filebuf);
        return -1;
    }
    // 3.2 文件映射
    uint32_t* addr = mmap(NULL,LEN * 4,PROT_WRITE | PROT_READ,MAP_SHARED,fd_oled,0);
    if(addr == MAP_FAILED)
    {
        perror("mmap");
        free(filebuf);
        close(fd_oled);
        return -1;
    } 
    // 3.3写入数据
    int i = 0;
    for(int h = 0;h<480;h++)
    {
        for(int w=0;w<800;w++)
        {
            uint8_t b = filebuf[i++];
            uint8_t g = filebuf[i++];
            uint8_t r = filebuf[i++];
            uint32_t color = (r<<16) | (g<<8) | b;
            int index = (479-h)*800 + w;
            addr[index] = color;
        }
    }

    // 4.关闭文件
    close(fd_oled);
    free(filebuf);
    munmap(addr,LEN * 3);

    return 0;
}

int main(int argc,char** argv)
{
    if(argc < 2)
    {
        fprintf(stderr,"Usage <%s> bmpfile\n",argv[0]);
        return -1;
    }
    drawbmp(argv[1]);
    return 0;
}