/*
* 从键盘录入一组数列，判断是否是回文，举例：12321，abba，121
*/
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool judge(const char* src,int len)
{
    bool res=true;
    int i=0,j=len-1;
    while(i<=j)
    {
        if(src[i]!=src[j]) {res=false;break;}
        i++;j--;
    }
    return res;
}

int main()
{
    char str[40];
    scanf("%s",str);
    int len=strlen(str);
    if(judge(str,len)) printf("Yes\n");
    else printf("No\n");
    return 0;
}