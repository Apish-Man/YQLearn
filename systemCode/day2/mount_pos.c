#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <stdio.h>

int main(int argc,char** argv)
{
    // 1.打开文件
    int fd = open("/dev/input/event0", O_RDONLY);
    if (fd == -1) {
        perror("无法打开设备");
        return 1;
    }
 
    // 获取X轴的绝对值信息
    struct input_absinfo abs_x;
    ioctl(fd, EVIOCGABS(ABS_X), &abs_x);
    int min_x = abs_x.minimum;
    int max_x = abs_x.maximum;
 
    // 获取Y轴的绝对值信息
    struct input_absinfo abs_y;
    ioctl(fd, EVIOCGABS(ABS_Y), &abs_y);
    int min_y = abs_y.minimum;
    int max_y = abs_y.maximum;
 
    // 假设屏幕分辨率为800x480，根据实际修改
    const int screen_width = 800;
    const int screen_height = 480;
 
    struct input_event ev;

    while (1) {
        if (read(fd, &ev, sizeof(ev)) == sizeof(ev)) {
            if (ev.type == EV_ABS) {
                int value = ev.value;
                if (ev.code == ABS_X) {
                    // 映射X坐标到屏幕宽度
                    int x = (value - min_x) * screen_width / (max_x - min_x);
                    printf("X: %d\n", x);
                } else if (ev.code == ABS_Y) {
                    // 映射Y坐标到屏幕高度
                    int y = (value - min_y) * screen_height / (max_y - min_y);
                    printf("Y: %d\n", y);
                }
            }
        }
    }

    close(fd);
    
    return 0;
}
