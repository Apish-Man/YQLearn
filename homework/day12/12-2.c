/*************************************************************************
  > File Name:    12-2.c
  > Author:       魏新强
  > Description:  
  > Created Time: 2025年05月22日 星期四 21时19分13秒
 ************************************************************************/

#include <stdio.h>

int sum_odd(int* p,int len)
{
  int ans=0,i=0;
  do{
    if(i%2==1) ans+=p[i];
    i++;
  }while(i<len);
  return ans;
}

int main(int argc,char *argv[])
{
  int arr[10]={0},i=0;
  // 初始化
  for(i=1;i<10;i++) arr[i]=arr[i-1]+1;
  // 求和
  int len=sizeof(arr)/sizeof(arr[0]);
  int ans=sum_odd(arr,len);
  printf("%d\n",ans);
  return 0;
}
