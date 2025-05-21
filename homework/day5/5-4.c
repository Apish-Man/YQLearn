/*
作业4：设计TVM（地铁自动售票机）机软件。
输入站数，计算费用，计费规则，6站2元，7-10站3元，11站以上为4元。 
输入钱数，计算找零(找零时优先找回面额大的钞票)，找零方式为各种面额张数，可识别面额：100,50,20,10,5,1
*/
#include <stdio.h>

int main()
{
    int pay_price,num,price;
    printf("请输入站数：\n");
    scanf("%d",&num);
    if(num<=6) price=2;
    else if(num<=10) price=3;
    else price=4;
    printf("应支付金额为:%d\n请支付！\n",price);
    scanf("%d",&pay_price);
    if(pay_price==num) printf("无需找零！\n");
    else if(pay_price<num) printf("支付金额不足\n");
    else{
        // 找零
        int tmp=pay_price-price;
        printf("找零金额为:%d\n",tmp);
        if(tmp>100){
            printf("找零100元，%d张\n",tmp/100);
            tmp%=100;
        }
        if(tmp>50){
            printf("找零50元，%d张\n",tmp/50);
            tmp%=50;
        }
        if(tmp>20){
            printf("找零20元，%d张\n",tmp/20);
            tmp%=20;
        }
        if(tmp>10){
            printf("找零10元，%d张\n",tmp/10);
            tmp%=10;
        }
        if(tmp>5){
            printf("找零5元，%d张\n",tmp/5);
            tmp%=5;
        }
        if(tmp>1){
            printf("找零1元，%d张\n",tmp/1);
            tmp%=1;
        }
    }
    return 0;
}