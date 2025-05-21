/*
9. 我国古代数学家张丘建在《算经》一书中提出的数学问题：鸡翁一值钱五，鸡母一值钱三，鸡雏三值钱一。百钱买百鸡，问鸡翁、鸡母、鸡雏各几何？
*/
#include <stdio.h>

int main()
{
    // 暴力求解
    int num_i=0,num_j=0,num_k=0;
    int price_i=0,price_j=0,price_k=0;
    int sum_num=0,sum_price=0;
    for(num_i=0;num_i*5<=100;num_i++)
    {
        for(num_j=0;num_i*5+num_j*3<=100;num_j++)
        {
            num_k=100-num_i-num_j;
            if(num_k%3!=0) continue;
            else if((num_i*5+num_j*3+num_k/3)==100) printf("翁：%d 母：%d 雏：%d\n",num_i,num_j,num_k);
        }
    }
    return 0;
}