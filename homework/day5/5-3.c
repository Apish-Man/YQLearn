/*
作业3：设计一个计算个人所得税的软件
应纳税所得额 = 工资收入金额 － 各项社会保险费(650元) － 起征点(3500元)
应纳税额 = 应纳税所得额 x 税率 － 速算扣除数
通过输入工资总额和社保费用计算应纳税额
*/
#include <stdio.h>

#define INSURANCE 650
#define TAX_THRESHOLD 3500

int main()
{
    double salary,tax;
    printf("请输入每月工资:\n");
    scanf("%lf",&salary);
    double tmp=(salary-INSURANCE-TAX_THRESHOLD)*12;
    if(tmp<=36000) tax=tmp*0.03-0;
    if(tmp>36000&&tmp<=144000) tax=tmp*0.10-2520;
    if(tmp>144000&&tmp<=300000) tax=tmp*0.20-16920;
    if(tmp>300000&&tmp<=420000) tax=tmp*0.25-31920;
    if(tmp>420000&&tmp<=660000) tax=tmp*0.30-52920;
    if(tmp>660000&&tmp<=960000) tax=tmp*0.35-85920;
    if(tmp>960000) tax=tmp*0.45-181920;
    printf("res:%lf\n",tax);
    return 0;
}
