#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/mman.h>
#include <stdlib.h>

#define LEN (800 * 480)
#define LEN_PIC 4

int drawbmp(const char *file);

 char* pic[LEN_PIC] = {"./beauty.bmp","./beauty1.bmp","./beauty2.bmp","../end.bmp"};
//char* pic[LEN_PIC] = {"./beauty.bmp","../end.bmp"};

int main(int argc,char** argv)
{
    // 1.打开屏幕输入设备
    int fd = open("/dev/input/event0",O_RDONLY);
    if(-1 == fd)
    {
        perror("open");
        return -1;
    }

    // 2.获取屏幕大小
    struct input_absinfo fb_var_x,fb_var_y;
    ioctl(fd,EVIOCGABS(ABS_X),&fb_var_x);
    int min_x = fb_var_x.minimum;
    int max_x = fb_var_x.maximum;
    ioctl(fd,EVIOCGABS(ABS_Y),&fb_var_y);
    int min_y = fb_var_y.minimum;
    int max_y = fb_var_y.maximum;

    // 3.屏幕分辨率为800x480
    const int screen_width = 800;
    const int screen_height = 480;
 
    // 4.捕获输入事件
    struct input_event ev;
    
    int cur_x=0,cur_y=0;
    int cur_index = 0;
    // 读入x,y,打印---确保动作一致性
    int flag=0;
    while(1)
    {
        if(read(fd,&ev,sizeof(ev))==sizeof(ev))
        {
            if (ev.type == EV_ABS) {
                int value = ev.value;
                if (ev.code == ABS_X) {
                    // 映射X坐标到屏幕宽度
                    int x = (value - min_x) * screen_width / (max_x - min_x);
                    printf("X: %d\n", x);
                    cur_x = x;
                    if(flag==0) flag = 1; //获取y
                    else flag = 0; //失败，重新开始
                } else if (ev.code == ABS_Y) {
                    // 映射Y坐标到屏幕高度
                    int y = (value - min_y) * screen_height / (max_y - min_y);
                    printf("Y: %d\n", y);
                    cur_y = y;
                    if(flag==1) flag = 2; //切换
                    else flag = 0; //失败，重新开始
                }
            }
            if(flag !=2 ) 
            {
                continue; // 非正确读取，不更新
            }
            // 更新图片
            if(cur_y<300 && cur_y>100)
            {
                if(cur_x<100)
                {
                    // 左侧，上一张
                    cur_index = (cur_index - 1 + LEN_PIC)%LEN_PIC;
                    drawbmp(pic[cur_index]);
                    flag = 0;
                }else if(cur_x > 700)
                {
                    // 右侧，下一张
                    cur_index = (cur_index + 1)%LEN_PIC;
                    drawbmp(pic[cur_index]);
                    flag = 0;
                }
            }
        }
    }
    
    // 5.关闭
    close(fd);

    return 0;
}

// 绘制图片
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
