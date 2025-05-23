/*************************************************************************
  > File Name:    12-4.c
  > Author:       魏新强
  > Description:  
  > Created Time: 2025年05月22日 星期四 21时33分02秒
  4. 利用指针变量输出字符数组中的所有字符。
 ************************************************************************/

#include <stdio.h>

int main(int argc,char *argv[])
{
    char str[10]="abcdefghi";
    int len=sizeof(str)/sizeof(str[0]);
    char *p=str;
    while(p<str+len)
    {
      printf("%c",*p);
      p++;
    }
    return 0;
}
