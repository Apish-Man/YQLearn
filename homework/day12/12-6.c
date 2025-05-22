/*************************************************************************
  > File Name:    12-6.c
  > Author:       魏新强
  > Description:  
  > Created Time: 2025年05月22日 星期四 21时40分23秒
 ************************************************************************/

#include <stdio.h>
#include <string.h>

int get_str_len(char *str)
{
  int i=0;
  while (*(str+i)!='\0')
  {
    i++;
  }
  return i;
}

int get_num_big(char *str)
{
  int i=0,ans=0;
  while (*(str+i)!='\0')
  {
    if(str[i]>='A'&&str[i]<='Z') ans++;
    i++;
  }
  return ans;
}

int get_num_char_num(char* str)
{
  int i=0,ans=0;
  while (*(str+i)!='\0')
  {
    if(str[i]>='0'&&str[i]<='9') ans++;
    i++;
  }
  return ans;
}

int main(int argc,char *argv[])
{
    char str[]="AbcDreADsytATSY1213AGSsasa";
    // 1.求字符串长度
    int str_len=get_str_len(str);
    printf("%d %d\n",str_len,strlen(str));
    // 2.统计大写字母个数
    int num_big=get_num_big(str);
    printf("%d\n",num_big);
    // 3.统计数字字符个数
    int num_char_num=get_num_char_num(str);
    printf("%d\n",num_char_num);
    return 0;
}
