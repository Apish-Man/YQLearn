/*************************************************************************
  > File Name:    10-11.c
  > Author:       魏新强
  > Description:  
  > Created Time: 2025年05月16日 星期五 21时31分56秒
 ************************************************************************/

#include <stdio.h>

// 判断输入字符是不是数字
int judge_is_num(char c)
{
  if(c>='0'&&c<='9') return 1;
  return 0;
}

int main(int argc,char *argv[])
{
  char c;
  while (scanf("%c",&c)!=EOF)
  {
    printf("%s\n",judge_is_num(c)?"YES":"NO");
    while(getchar()!='\n');
  }
  
  return 0;
}
