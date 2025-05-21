
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
/*
 *已知某8位二进制数的补码为 1101 1011 ，求其原码和对应的十进制真值。
 */

// 补码->原码
char *Convert(const char *src)
{
    char *result = (char *)malloc(sizeof(char) * 40);
    memset(result, '\0', sizeof(result));
    // 如果是正数，直接返回其原码即可
    // 如果是负数，符号位不变，最左边1之前按位取反
    int len_src = strlen(src);
    if (len_src == 0)
        return result;
    result[0] = src[0];
    bool flag = 0;
    for (int i = len_src-1; i > 0; i--)
    {
        // 过滤非0,1字符
        if (src[i] == '0' || src[i] == '1')
        {
            if (src[0] == '0')
                result[i] = src[i];
            if (src[0] == '1')
            {
                if (flag == 1)
                    result[i] = '1' - src[i] + '0';
                else
                {
                    result[i] = src[i];
                    if (result[i] == '1')
                        flag = 1;
                }
            }
        }else result[i] = src[i];
    }
    result[len_src]='\0';
    // printf("%s", result);
    return result;
}

// 补码->真值
int get_number(const char* src)
{
    int result=0;
    // 求原码
    char *tmp=Convert(src);
    int key=1,i=0,len_tmp=strlen(tmp);
    for(i=len_tmp;i>0;i--)
    {
        if (src[i] == '0' || src[i] == '1'){
            result+=key*(tmp[i]-'0');
            key*=2;
        }
    }
    if(src[0]=='1') result=0-result;
    return result;
}

int main()
{
    char src[40] = "1101 1011";
    printf("原码：%s\n",Convert(src));
    printf("十进制真值:%d",get_number(src));
    return 0;
}