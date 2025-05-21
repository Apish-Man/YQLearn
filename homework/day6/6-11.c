/*
1. 判断一个数是不是回文数。(回文数是指正序（从左向右）和倒序（从右向左）读都是一样的整
数。如：12321
*/
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

bool judge(char* str)
{
    int i=0,j=strlen(str)-1;
    bool flag=1;
    while(i<j)
    {
        // 过滤无效字符
        if(str[i]<'0'||str[i]>'9') 
        {
            i++;
            continue;
        }
        if(str[j]<'0'||str[j]>'9') 
        {
            j++;
            continue;
        }
        if(str[i++]!=str[j--]) flag=0;
    }
    return flag;
}

int main()
{
    int x,i=0;
    scanf("%d",&x);
    char str[20]={0};
    while(x>0)
    {
        str[i++]=x%10+'0';
        x=x/10;
    }
    printf("%s\n",judge(str)?"YES":"NO");
    return 0;
}