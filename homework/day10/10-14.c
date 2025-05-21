/*************************************************************************
  > File Name:    10-14.c
  > Author:       魏新强
  > Description:  
  > Created Time: 2025年05月17日 星期六 14时25分40秒
 ************************************************************************/

#include <stdio.h>

void reverse(int* arr,int len)
{
  int i=0,j=len-1;
  while(i<j){
    int tmp=arr[i];
    arr[i]=arr[j];
    arr[j]=tmp;
    i++;j--;
  }
  return;
}

int main(int argc,char *argv[])
{
  int arr[]={12,23,11,24,25,35,22,12,34,20,9,-9};
  int len=sizeof(arr)/sizeof(int);
  for(int i=0;i<len;i++) printf("%d ",arr[i]);
  printf("交换后:\n");
  reverse(arr,len);
  for(int i=0;i<len;i++) printf("%d ",arr[i]);
  return 0;
}
