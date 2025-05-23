/*************************************************************************
  > File Name:    12-3.c
  > Author:       魏新强
  > Description:  
  > Created Time: 2025年05月22日 星期四 21时27分28秒
  3. 确认整型，字符型，浮点型指针变量的大小；
 ************************************************************************/

#include <stdio.h>

int main(int argc,char *argv[])
{
    int a=10;
    char b='c';
    float c=1.23;
    double d=2.34;
    int *p1=&a;
    char *p2=&b;
    float *p3=&c;
    double *p4=&d;
    printf("%d %d %d %d\n",(char)sizeof(p1),(char)sizeof(p2),(char)sizeof(p3),(char)sizeof(p4));
    return 0;
}
