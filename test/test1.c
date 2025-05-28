#include <stdio.h>
void fun(char *str, int len)
{
    // 在函数内部不能对字符串中的字符做修改，否则报错 段错误
    *(str+1) = 'E';
    str[2] = 'L';
    printf("%s\n",str); // hELlo
}
int main(int argc,char *argv[])
{
	char str1[] = "hello";
    char *str = str1;
    int len = sizeof(str)/sizeof(str[0]);
    fun(str,len);
    return 0;
}