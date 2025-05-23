/*************************************************************************
  > File Name:    12-5.c
  > Author:       魏新强
  > Description:  
  > Created Time: 2025年05月22日 星期四 21时36分31秒
  5. 编写一个函数，用指针变量做参数，用于求出一个浮点型数组元素的平均值。
 ************************************************************************/

#include <stdio.h>

double get_ave(double *arr,int len)
{
  double sum=0.0;
  int i=0;
  for(i=0;i<len;i++) sum+=arr[i];
  return sum/len;
}

int main(int argc,char *argv[])
{
    double arr[5]={12.2,1.1,2.1,3.2,4.1};
    int len=sizeof(arr)/sizeof(arr[0]);
    double ave=get_ave(arr,len);
    printf("%lf\n",ave);
    return 0;
}
