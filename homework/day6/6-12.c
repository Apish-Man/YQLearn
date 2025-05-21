/*
2. 通过键盘录入一个整数，判断这个整数是否是水仙花数。123
*/
#include <stdio.h>
#include <stdbool.h>

// 水仙花数是指一个 3 位数，它的每个数位上的数字的 3次幂之和等于它本身。例如：1^3 + 5^3+ 3^3 = 153。
bool judge(int x)
{
    int ans=0,i=0,tmp=x;
    while(tmp)
    {
        int w=tmp%10;
        ans=ans+w*w*w;
        tmp=tmp/10;
    }
    return ans==x;
}

int main()
{
    int x;
    scanf("%d",&x);
    printf("%s\n",judge(x)?"YES":"NO");
    return 0;
}