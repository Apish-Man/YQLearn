/*
10. 从键盘上输入多个无符号整型数据，直到 0 结束 ，输出所输入数据中的最大值。
*/
#include <stdio.h>

int main()
{
    unsigned int max=0,x;
    while(scanf("%d",&x)&&x!=0)
    {
        if(x>max) max=x;
    }
    printf("max:%d\n",max);
    return 0;
}