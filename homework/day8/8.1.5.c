/*
*5.用数组存储10个整型数，通过键盘输入一个数，找出该数在数组中的下标值
*/
#include <stdio.h>

#define MAX 10

const int array[MAX]={1,2,3,4,5,6,7,8,9,10};

int main()
{
    int target=0;
    scanf("%d",&target);
    int i=0,flag=0;
    for(i=0;i<MAX;i++) if(array[i]==target) {printf("%d\n",i);flag=1;}
    if(!flag) printf("不存在\n");
    return 0;
}