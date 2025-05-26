/*
作业5：出租车费计算程序，起步公里数3公里，车费8元，超出3公里，每公里1.8元，不足一公
里，按照1公里计算。 如果输入一个钱数，计算最多能坐多少公里，该如何实现？
*/
#include <stdio.h>
#include <math.h>

#define START_DIS 3
#define START_PRICE 8
#define UNIT_PRICE 1.8

// 计算车费
double calculate_fare(double distance) {
    if (distance < 0) return -1;
    int total_km = ceil(distance); // 向上取整
    if (total_km <= START_DIS) return START_PRICE;
    return START_PRICE + (total_km - START_DIS) * UNIT_PRICE;
}

// 计算最大里程
double calculate_distance(double money) {
    if (money < START_PRICE) return 0;
    int extra = (money - START_PRICE) / UNIT_PRICE;
    return START_DIS + extra + 1; // 剩余金额至少够1公里才+1
}

int main() {
    int mode;
    printf("请选择模式：1-计算车费 2-计算里程\n");
    scanf("%d", &mode);

    if (mode == 1) {
        double distance;
        printf("请输入行驶公里数：");
        scanf("%lf", &distance);
        double fare = calculate_fare(distance);
        printf(fare >= 0 ? "车费：%.1f元\n" : "输入错误\n", fare);
    } 
    else if (mode == 2) {
        double money;
        printf("请输入金额：");
        scanf("%lf", &money);
        double max_distance = calculate_distance(money);
        printf("最多可乘坐：%.1f公里\n", max_distance - 0.1); // 显示最大临界值
    }
    return 0;
}
