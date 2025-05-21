/*************************************************************************
  > File Name:    10-9.c
  > Author:       魏新强
  > Description:  
  > Created Time: 2025年05月16日 星期五 21时07分08秒
 ************************************************************************/

#include <stdio.h>
#include <string.h>

int get_num(char* str,int len)
{
  int num=0,i=0;
  for(i=0;i<len;i++)
  {
    if(str[i]>='A'&&str[i]<='Z') num++;
  }
  return num;
}

int main(int argc,char *argv[])
{
    char str[50]={0};
    scanf("%s",str);
    int len=strlen(str);
    printf("大写字母个数:%d",get_num(str,len));
    return 0;
}
