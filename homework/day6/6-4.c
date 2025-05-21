/*
4. 计算n的阶乘 ? n! = 123.....*n n值通过键盘输入
*/
#include <stdio.h>

int fab(int n)
{
    if(n<=0) return 0;
    if(n==1) return 1;
    return fab(n-1)*n;
}

int fab2(int n)
{
    if(n<=0) return 0;
    int ans=1;
    while(n>1) ans=ans*(n--);
    return ans;
}

int main()
{
    int n;
    scanf("%d",&n);
    printf("ans:%d\n",fab(n));
    printf("ans:%d\n",fab2(n));
    return 0;
}