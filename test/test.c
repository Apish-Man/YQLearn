/*************************************************************************
  > File Name:    test.c
  > Author:       SunTeng
  > Description:  
  > Created Time: 2025年05月14日 星期三 18时46分59秒
 ************************************************************************/

#include <stdio.h>

int main(int argc,char *argv[])
{
  char *str = "I love you";
  printf("%s",str);
  str[0] = 'n'; // 危险操作！运行时可能崩溃（gcc会警告）
  printf("%s",str);
}
