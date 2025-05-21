/*
作业5：出租车费计算程序，起步公里数3公里，车费8元，超出3公里，每公里1.8元，不足一公
里，按照1公里计算。 如果输入一个钱数，计算最多能坐多少公里，该如何实现？
*/
#include <stdio.h>

#define START_DIS 3
#define START_DIS_PRICE 8
#define PRICE 1.8

int main()
{
    double distance,price=-1;
    printf("请输入公里数：\n");
    scanf("%lf",&distance);
    if(distance<0) price=-1;
    else{
        if(distance<3) price=8;
        if(distance>8) price=(int)(distance+0.5-3) * PRICE;
    }
    printf("%s%lf",price>0?"价格为：":"输入错误",price);
    return 0;
}