/*************************************************************************
  > File Name:    test.c
  > Author:       SunTeng
  > Description:  
  > Created Time: 2025年05月14日 星期三 18时46分59秒
 ************************************************************************/

#include <stdio.h>

int main(int argc,char *argv[])
{
  int x=65000;
  int* p1=&x;
  char* p2=(char*)p1;
  printf("%c\n",*(p2));
  printf("%c\n",*(p2+1));
  printf("%c\n",*(p2+2));
  printf("%c\n",*(p2+3));
}
