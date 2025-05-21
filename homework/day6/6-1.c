// 1. 计算n以内所有正奇数的和 ? n值通过键盘输入

#include <stdio.h>

int sum_of_odd(int x)
{
    int sum=0;
    while(x>0)
    {
        if(x%2) sum+=x;
        x--;
    }
    return sum;
}

int main()
{
    int n;
    scanf("%d",&n);
    printf("%d\n",sum_of_odd(n));
    return 0;
}

