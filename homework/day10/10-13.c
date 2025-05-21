/*************************************************************************
  > File Name:    10-13.c
  > Author:       魏新强
  > Description:  
  > Created Time: 2025年05月17日 星期六 14时04分41秒
 ************************************************************************/

#include <stdio.h>

// 从小到大排序
void bubble_sort(int* arr,int len)
{
  // 循环len躺,每次把最大值放到最后
  int i=0,j=0;
  for(i=0;i<len;i++)
  {
    int flag=0;
    for(j=0;j<len-i-1;j++)
    {
      if(arr[j+1]<arr[j])
      {
        int tmp=arr[j];
        arr[j]=arr[j+1];
        arr[j+1]=tmp;
        flag=1;
      }
    }
    if(flag==0) return;
  }
  return;
}

int main(int argc,char *argv[])
{
  int arr[]={12,23,11,24,25,35,22,12,34,20,9,-9};
  int len=sizeof(arr)/sizeof(int);
  bubble_sort(arr,len);
  for(int i=0;i<len;i++) printf("%d ",arr[i]);
  return 0;
}
