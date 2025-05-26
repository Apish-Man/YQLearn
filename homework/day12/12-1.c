/*************************************************************************
  > File Name:    12-1.c
  > Author:       魏新强
  > Description:  
  > Created Time: 2025年05月22日 星期四 20时37分28秒
  1. 利用指针变量将一个数组中的数据反向输出。
 ************************************************************************/

#include <stdio.h>

//数组反向输出
int convert_printf(char* arr,int len)
{
    int i=len-1;
    while(i>=0)
    {
        printf("%c",arr[i]);
        i--;
    }
    return 0;
}

int main(int argc,char *argv[])
{
    char str[7]="abcdef";
    // int len=sizeof(str)/sizeof(str[0]);
    int len = strlen(str);  // 有效字符数为6   注意区分字符数组的字符串长度和字符数组长度
    int i=0;
    for(i=0;i<len;i++) printf("%c",str[i]);
    printf("\n");
    convert_printf(str,len);
    return 0;
}
