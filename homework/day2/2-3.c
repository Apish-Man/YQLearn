// 将十进制数 -125.625 转换为32位浮点数格式（IEEE 754单精度）。

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static char* CON[10][4]={""};

char* convert10_2_int(int src)
{
    char* result=malloc(sizeof(char)*40);
    memset(result,'\0',sizeof(result));
    int len_res=0;
    printf("%d\n",src);
    // 整数位转化为2进制
    while(src>0)
    {
        result[len_res]=src%2+'0';
        src=src/2;
        len_res++;
    }
    // printf("%s\n",result);
    int i=0,j=len_res-1;
    while(i<j)
    {
        // 交换位置
        char tmp=result[i];
        result[i]=result[j];
        result[j]=tmp;
        i++;j--;
    }
    // printf("%s\n",result);
    return result;
}

int main()
{
    double x=-125.625;
    char result[40];
    int len=0;
    memset(result,'\0',sizeof(result));
    // 1.设置符号位
    if(x<0) {
        result[0]='1';
        x=-x;
    }else if(x>0) result[0]='0';
    len++;
    result[1]=' ';//便于查看区分
    len++;
    // 2.整数部分转化为2进制
    char *tmp=convert10_2_int((int)x);
    strcpy(result+2,tmp);
    len+=strlen(tmp);
    // printf("%s\n",tmp);
    printf("%s\n",result);

    return 0;
}