/*
作业7：实现一个简单的计算器，要求根据输入的运算符实现相应的运算。
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