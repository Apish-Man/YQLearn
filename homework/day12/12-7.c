/*************************************************************************
  > File Name:    12-7.c
  > Author:       魏新强
  > Description:  
  > Created Time: 2025年05月22日 星期四 21时50分53秒
  7. 编写函数,要求用指针做形参，实现将二维数组(行列相同)的进行转置（行列数据互换）： int
(*p)[N]
 ************************************************************************/

#include <stdio.h>

void Transpose(int (*arr)[4],int n)
{
  int i=0,j=0;
  for(i=0;i<n;i++)
  {
    for(j=0;j<i;j++)
    {
      int tmp=arr[i][j];
      arr[i][j]=arr[j][i];
      arr[j][i]=tmp;
    }
  }
}

int main(int argc,char *argv[])
{
    int array[4][4]={{1,2,3,4},{5,6,7,8},{9,10,11,12},{13,14,15,16}};
    int n=sizeof(array)/sizeof(array[0]);
    Transpose(array,n);
    int i=0,j=0;
    for(i=0;i<n;i++)
    {
      for(j=0;j<n;j++) printf("%d ",array[i][j]);
      printf("\n");
    }
    return 0;
}
