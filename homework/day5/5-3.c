/*
作业3：设计一个计算个人所得税的软件
应纳税所得额 = 工资收入金额 － 各项社会保险费(650元) － 起征点(3500元)
应纳税额 = 应纳税所得额 x 税率 － 速算扣除数
通过输入工资总额和社保费用计算应纳税额
*/
#include <stdio.h>

#define TAX_THRESHOLD 3500

int main() {
    double salary, insurance;
    printf("请输入每月工资和社保费用:\n");
    scanf("%lf%lf", &salary, &insurance);
    
    double annual_income = (salary - insurance - TAX_THRESHOLD) * 12;
    double tax = 0;
    
    if (annual_income <= 0) {
        tax = 0;
    } else if (annual_income <= 36000) {
        tax = annual_income * 0.03;
    } else if (annual_income <= 144000) {
        tax = annual_income * 0.10 - 2520;
    } else if (annual_income <= 300000) {
        tax = annual_income * 0.20 - 16920;
    } else if (annual_income <= 420000) {
        tax = annual_income * 0.25 - 31920;
    } else if (annual_income <= 660000) {
        tax = annual_income * 0.30 - 52920;
    } else if (annual_income <= 960000) {
        tax = annual_income * 0.35 - 85920;
    } else {
        tax = annual_income * 0.45 - 181920;
    }
    
    printf("应纳税额: %.2lf元\n", tax/12); // 输出月均税额
    return 0;
}

