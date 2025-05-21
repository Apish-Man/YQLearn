/*************************************************************************
  > File Name:    10-4.c
  > Author:       魏新强
  > Description:  
  > Created Time: 2025年05月16日 星期五 19时54分49秒
 ************************************************************************/

#include <stdio.h>

/*
 *@param nums输入整数
 *return ans 平方和
 */
int get_square_num(int a)
{
    int ans=0;
    while(a)
    {
        int tmp=a%10;
        ans+=tmp*tmp;
        a=a/10;
    }
    return ans;
}

int main(int argc,char *argv[])
{
    int a;
    scanf("%d",&a);
    printf("%d\n",get_square_num(a));
    return 0;
}
