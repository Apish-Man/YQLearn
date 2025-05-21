/*************************************************************************
  > File Name:    10-8.c
  > Author:       魏新强
  > Description:  
  > Created Time: 2025年05月16日 星期五 20时45分03秒
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 100

/*
 *@param num传入数字
 *return 对应字符
 */

char* convert(int num)
{
    char * res=malloc(sizeof(char)*MAX);
    memset(res,'\0',MAX);
    int i=0,j;
    // 倒存
    while(num)
    {
        int tmp=num%10;
        res[i++]=tmp+'0';
        num=num/10;
    }
    // 数组反转
    i=0;j=strlen(res)-1;
    while(i<j)
    {
        // 反转
        char tmp=res[i];
        res[i]=res[j];
        res[j]=tmp;
        i++;j--;
    }
    return res;
}


int main(int argc,char *argv[])
{
    int x;
    scanf("%d",&x);
    char* str=convert(x);
    printf("%s",str);
    free(str);
    return 0;
}
