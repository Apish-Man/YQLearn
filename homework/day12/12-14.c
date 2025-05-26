/*************************************************************************
  > File Name:    12-14.c
  > Author:       魏新强
  > Description:  
  > Created Time: 2025年05月23日 星期五 18时45分33秒
  14. 附加题【选做】： 编写一个函数，实现 memmove 的功能。
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 *1.内存复制:将源内存地址src的n个字节复制到目标地址dest
 *2.处理内存重叠，若src和dest指向内存有重叠，需要处理
 */
int my_mommove(char *src,char *dest,int len)
{
    int i=0;
    if(dest>src)
    {
        //从高地址开始复制
        for(i=len-1;i>=0;i--) *(dest+i)=*(src+i);
        return 1;
    }else{
        for(i=0;i<len;i++) *(dest+i)=*(src+i);
        return 1;
    }
    return 0;
}

int main(int argc,char *argv[])
{
    double arr[]={1.1,4.1,8.1,0,1.2};
    int len=sizeof(arr),num=len/sizeof(arr[0]);//记录的是字节数
    char *copy_arr=malloc(len);
    int flag=my_mommove((char*)arr,copy_arr,len);
    if(flag)
    {
        //成功，打印数据
        int i=0;
        for(i=0;i<num;i++)
        {
            printf("%lf ",*((double *)(copy_arr)+i));
        }
        printf("\n");
    }else{
        printf("复制失败\n");
    }
    return 0;
}
