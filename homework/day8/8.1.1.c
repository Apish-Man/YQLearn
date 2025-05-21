/*
* 1. 键盘录入一组数列，利用冒泡排序将数据由大到小排序
*/
#include <stdbool.h>
#include <stdio.h>


void bubble_sort(int* src,int len)
{
    // 循环len躺，每次把一个最小值放到合适位置
    int i=0,j=0;
    for(i=0;i<len;i++)
    {
        bool flag=0;
        for(j=0;j<len-i;j++)
        {
            if(j+1<len&&src[j]<src[j+1])
            {
                // 交换
                int tmp=src[j];
                src[j]=src[j+1];
                src[j+1]=tmp;
                flag=1;
            }
        }
        if(!flag) return;
    }

}

int main()
{
    int a[]={5,4,3,2,1,9,0,2,-1,-8};
    int i=0,len=sizeof(a)/sizeof(int);
    bubble_sort(a,len);
    for(i=0;i<len;i++) printf("%d ",a[i]);
    return 0;
}