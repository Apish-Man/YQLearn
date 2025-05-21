/*
计算 1 + 1/(2 * 3) + 1/(3 * 4) + ...+ 1/(n * (n + 1)) = ?直到最后一相值小于0.00001为至。
*/

#include <stdio.h>

int main()
{
    double n=1,sum=0;
    double tmp=1;
    while(tmp>0.00001)
    {
        sum+=tmp;
        n++;
        tmp=1.0/(n*(n+1));
    }
    printf("%.5lf",sum);
    return 0;
}