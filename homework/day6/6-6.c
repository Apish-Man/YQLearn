/*
6. 求输入的十个整数中正数的个数及其平均值
*/
#include <stdio.h>

int main()
{
    
    int a[10]={0};
    int i=0,sum=0,count=0;
    for(i=0;i<10;i++) 
    {
        scanf("%d",&a[i]);
        sum+=a[i];
        if(a[i]>0) count++;
    }
    double ave=sum/10.0; 
    printf("正数个数:%d ave:%.5lf",count,ave);
    return 0;
}