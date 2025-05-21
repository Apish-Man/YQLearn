/*
7. 打印出100以内能整除7之外的的自然数
*/
#include <stdio.h>

int main()
{
    int i=0;
    for(i=0;i<100;i++)
    {
        if(i%7!=0) printf("%d ",i);
    }
    return 0;
}