#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdint.h>

#define LCD_W   800
#define LCD_H   480

int main(int argc,char* argv[])
{
    if(argc < 2)
    {
        fprintf(stderr,"Usage %s Color\n",argv[0]);
        return -1;
    }
    int lcdfd = open("/dev/fb0",O_RDWR);
    if(lcdfd == -1)
    {
        perror("open");
        return -1;
    }
    uint32_t color = 0x00000000;
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
    uint32_t *paddr = (uint32_t*) mmap(NULL, LCD_W * LCD_H * 4,PROT_READ|PROT_WRITE,MAP_SHARED,lcdfd,0);
    if(paddr == MAP_FAILED)
    {
        perror("mmap");
        close(lcdfd);
        return -1;
    }
    for(int i = 0; i < LCD_W * LCD_H; i++)
         *(paddr + i) = color; 

    munmap(paddr,LCD_W * LCD_H * 4);
    close(lcdfd);

    return 0;
}
