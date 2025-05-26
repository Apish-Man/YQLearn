/*
作业7：实现一个简单的计算器，要求根据输入的运算符实现相应的运算。
*/

#include <stdio.h>
#include <ctype.h>

void clear_buffer() {
    while(getchar() != '\n');
}

int main() {
    int x1, x2;
    char oper;
    
    while(1) {
        printf("输入算式（格式：数字 运算符 数字 / q退出）\n");
        int ret = scanf("%d %c %d", &x1, &oper, &x2);
        
        if (ret != 3) {
            if (getchar() == 'q') break;
            printf("输入格式错误！\n");
            clear_buffer();
            continue;
        }

        switch(oper) {
            case '+':
                printf("%d + %d = %d\n", x1, x2, x1+x2);
                break;
            case '-':
                printf("%d - %d = %d\n", x1, x2, x1-x2);
                break;
            case '*':
                printf("%d * %d = %d\n", x1, x2, x1*x2);
                break;
            case '/':
                if(x2 == 0) {
                    printf("错误：除数不能为零\n");
                    break;
                }
                printf("%d / %d = %.2f\n", x1, x2, (double)x1/x2);
                break;
            default:
                printf("错误：无效运算符'%c'\n", oper);
        }
        clear_buffer();
    }
    return 0;
}