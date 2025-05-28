/*************************************************************************
  > File Name:    test.c
  > Author:       SunTeng
  > Description:  
  > Created Time: 2025年05月14日 星期三 18时46分59秒
 ************************************************************************/

#include <stdio.h>

struct Student {
    int score[5];
};

struct Student2 {
    int score2;
};

int main() {
    struct Student s = {{10, 20, 30, 40, 50}};
    struct Student *stu = &s;
    // stu->score[2]
    printf("%d\n", stu->score[2]);  // 输出30
    // printf("%d\n", stu->(score[2]));  // 输出30
    // stu2[2]->score2
    struct Student2 s1={1},s2={2},s3={3};
    struct Student2* stu2[3]={&s1,&s2,&s3};
    // stu2[2]->score2
    printf("%d\n", stu2[2]->score2);  // 输出30

    char arr[3] = {}; // {'\0'}
arr[2] = 'A'; // 正确，对字符数组中的元素赋值
arr = {'E','D','F'}; // 错误，对整个数组赋值（可以理解为数组名就是一个常量，一旦创建，就无法改变）
    return 0;
}
