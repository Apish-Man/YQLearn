/*************************************************************************
  > File Name:    12-7.c
  > Author:       魏新强
  > Description:  
  > Created Time: 2025年05月22日 星期四 21时50分53秒
  8. 编写函数,要求用指针做形参，实现统计二维数组上三角中的0 的数量：
 ************************************************************************/

#include <stdio.h>

int get_num_zero(int (*arr)[4],int n)
{
  int i=0,j=0,num=0;
  for(i=0;i<n;i++)
  {
    for(j=0;j<=i;j++)
    {
      if(arr[j][i]==0) num++;
    }
  }
  return num;
}

int main(int argc,char *argv[])
{
    int array[4][4]={{1,0,3,4},{5,0,7,8},{0,10,11,12},{13,14,15,0}};
    int n=sizeof(array)/sizeof(array[0]);
    int num=get_num_zero(array,n);
    printf("%d\n",num);
    return 0;
}
