/*
2. 编写一个程序，实现简单的计算器功能：
从键盘输入两个整数和一个运算符（+、-、*、/）
根据运算符进行相应的计算
输出计算结果
要求：
1. 输入格式为"数字 运算符 数字"，例如"12 + 34"
2. 处理除法时的除零错误
3. 输出格式为"12 + 34 = 46"
*/
#include <stdio.h>

int main()
{
    int x1,x2;
    char oper;
    while((scanf("%d%c%c%d",&x1,&oper,&oper,&x2)!=EOF))
    {
        printf("%d %c %d\n",x1,oper,x2);
        switch(oper){
            case '+':{
                printf("%d %c %d = %d\n",x1,oper,x2,x1+x2);
                break;
            }
            case '-':{
                printf("%d %c %d = %d\n",x1,oper,x2,x1-x2);
                break;
            }
            case '*':{
                printf("%d %c %d = %d\n",x1,oper,x2,x1*x2);
                break;
            }
            case '/':{
                printf("%d %c %d = %d\n",x1,oper,x2,x1/x2);
                break;
            }
            default:{
                printf("输入错误\n");
                break;
            }
        }

    }
    return 0;
}