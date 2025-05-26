/*************************************************************************
  > File Name:    10-10.c
  > Author:       魏新强
  > Description:  
  > Created Time: 2025年05月16日 星期五 21时11分44秒
  10. 设计函数，实现strcmp 的功能；
 ************************************************************************/

#include <stdio.h>
#include <string.h>

/*
* 字符串s1，s2；从头开始比较，若s1>s2,返回正数，s1=s2返回0,s1<s2返回负数
*/
int my_strcmp(char* str1,char* str2)
{
  int len1=strlen(str1),len2=strlen(str2);
  int i=0;
  // 逐字符比较
  while(i<len1&&i<len2)
  {
    if(str1[i]==str2[i]) i++;
    else return str1[i]-str2[i];
  }
  return str1[i]-str2[i];
}

int my_strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}


int main(int argc,char *argv[])
{
  char s1[20]="abadef";
  char s2[20]="abcdef";
  printf("%d\n",my_strcmp(s1,s2)); 
  return 0;
}
