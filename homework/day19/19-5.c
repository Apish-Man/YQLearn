/*************************************************************************
  > File Name:    19-5.c
  > Author:       魏新强
  > Description:  
  > Created Time: 2025年05月30日 星期五 18时06分55秒
 ************************************************************************/

#include <stdio.h>

union EndianCheck{
    int num;
    char bytes[4];
};

int main(int argc,char *argv[])
{
    union EndianCheck check;
    check.num=1;
    //不需要强转为char，指向同一空间，
    if(check.bytes[0]==0) printf("当前主机为大端序(低字节高地址)\n");
    else printf("当前主机为小端序(低字节低地址)\n");

    check.num=0x12345678;
    //不需要强转为char，指向同一空间，
    if(check.bytes[0]==0x12) printf("当前主机为大端序(低字节高地址)\n");
    else printf("当前主机为小端序(低字节低地址)\n");
    return 0;
}
