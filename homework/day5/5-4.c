/*
作业4：设计TVM（地铁自动售票机）机软件。
输入站数，计算费用，计费规则，6站2元，7-10站3元，11站以上为4元。 
输入钱数，计算找零(找零时优先找回面额大的钞票)，找零方式为各种面额张数，可识别面额：100,50,20,10,5,1
*/
#include <stdio.h>

int main() {
    int num, price;
    printf("请输入站数：\n");
    scanf("%d", &num);
    
    // 票价计算
    if (num <= 6) price = 2;
    else if (num <= 10) price = 3;
    else price = 4;
    
    printf("应支付金额为:%d元\n请支付：\n", price);
    int pay_amount;
    scanf("%d", &pay_amount);
    
    // 支付校验
    if (pay_amount == price) {
        printf("无需找零！\n");
    } else if (pay_amount < price) {
        printf("支付金额不足\n");
    } else {
        int change = pay_amount - price;
        printf("找零金额为:%d元\n", change);
        
        // 面额数组按从大到小排列
        int denominations[] = {100, 50, 20, 10, 5, 1};
        for (int i = 0; i < 6; i++) {
            int count = change / denominations[i];
            if (count > 0) {
                printf("找零%d元，%d张\n", denominations[i], count);
                change %= denominations[i];
            }
        }
    }
    return 0;
}
