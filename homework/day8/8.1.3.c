/*
* 3. 键盘录入一组数列，求最大数、最小数、均值
*/
#include <stdbool.h>
#include <stdio.h>

int max(const int* src,int len)
{
    if(len<=0) return 0;
    int res=src[0],i=0;//把第一个数看做最大数
    for(i=1;i<len;i++)
    {
        if(src[i]>res)
        {
            //更新最大数
            res=src[i];
        }
    }
    return res;
}

int min(const int* src,int len)
{
    if(len<=0) return 0;
    int res=src[0],i=0;//把第一个数看做最小数
    for(i=1;i<len;i++)
    {
        if(src[i]<res)
        {
            //更新最小数
            res=src[i];
        }
    }
    return res;
}

double average(const int* src,int len)
{
    double sum=0.0;
    int i=0;
    if(len==0) return 0;
    for(i=0;i<len;i++) sum+=src[i];
    double res= sum/(double)len;
    return res;
}

int main()
{
    // int array[10]={1,2,3,4,5,6,7,8,9,10};
    // int len=sizeof(array)/sizeof(int);
    int n;
    printf("输入数字个数：");
    scanf("%d", &n);
    
    int array[n];
    for(int i=0; i<n; i++){
        printf("输入第%d个数：", i+1);
        scanf("%d", &array[i]);
    }
    int len=n;
    printf("%d %d %f",max(array,len),min(array,len),average(array,len));
    return 0;
}