/*
编写一个程序，读取用户输入的字符串，并将其反转输出。
*/
#include <stdio.h>
#include <string.h>

#define MAX 40

int main()
{
    char str[MAX];
    scanf("%s",str);

    int len=strlen(str);

    // 不使用其它空间，在原数组上进行调换
    int i=0,j=len-1;
    while(i<=j)
    {
        char tmp=str[i];
        str[i]=str[j];
        str[j]=tmp;
        i++;j--;
    }
    printf("%s",str);

    return 0;
}