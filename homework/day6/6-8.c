/*
8. 打印乘法表
*/
#include <stdio.h>

int main()
{
    int i=1,j=1;
    for(i=1;i<=9;i++)
    {
        for(j=1;j<=i;j++)
        {
            printf("%d*%d=%d ",j,i,i*j);
        }
        printf("\n");
    }
    return 0;
}