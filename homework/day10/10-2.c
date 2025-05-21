/*************************************************************************
  > File Name:    10-2.c
  > Author:       魏新强
  > Description:  
  > Created Time: 2025年05月16日 星期五 19时07分44秒
 ************************************************************************/

/*
 *输入数字字符，返回该数字
 */

#include <stdio.h>

/*
 *@param c输入字符
 *return 返回数字
 */
int translate(char c)
{
    return c-'0';
}

int main(int argc,char *argv[])
{
    char c;
    while(scanf("%c",&c)!=EOF)
    {
        if(c<'0'||c>'9') printf("请输入0-9的单个字符\n");
        else{
            printf("%d\n",translate(c));
        }
            while(getchar()!='\n');
    }
    return 0;
}
