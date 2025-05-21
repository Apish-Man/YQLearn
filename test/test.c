/*************************************************************************
  > File Name:    test.c
  > Author:       SunTeng
  > Description:  
  > Created Time: 2025年05月14日 星期三 18时46分59秒
 ************************************************************************/

#include <stdio.h>

int p;
void f1()
{
  printf("%d\n",p);
  printf("%d\n",q);
}

int q;
void f2()
{
  printf("%d\n",p);
  printf("%d\n",q);
  printf("%d\n",w);
}
int w;

int main(int argc,char *argv[])
{
  f1();
  f2();
  return 0;
}
