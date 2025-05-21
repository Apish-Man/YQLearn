/*
3. 计算1+1/2 - 1/3 + 1/4 - 1/5 ...+1/n= ? n通过键盘输入（int k = -1）
*/

#include <stdio.h>

double compute(int n)
{
    double ans=1;
    int flag=1,i=2;
    while (i<=n)
    {
        ans=ans+flag*(1.0/i);
        flag=-1;
        i++;
    }
    return ans;
}

int main()
{
    int n;
    scanf("%d",&n);
    printf("ans:%lf",compute(n));
    return 0;
}