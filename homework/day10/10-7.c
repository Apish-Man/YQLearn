/*************************************************************************
  > File Name:    10-7.c
  > Author:       魏新强
  > Description:  
  > Created Time: 2025年05月16日 星期五 20时28分05秒
  7. 设计一个函数，用来查找一个字符串中某个字符的位置；
 ************************************************************************/

#include <stdio.h>
#include <string.h>


/*
 *@param str 传入的字符串;c 查找的字符
 *return 字符位置
 */
int get_position(char str[],char c,int len)
{
    int i=0;
    for(i=0;i<len;i++)
    {
        if(str[i]==c) return i+1;//i为下标，i+1为位置
    }
    if(i==len) return -1;//查不到
}

int main(int argc,char *argv[])
{
    char str[30]={0};
    printf("请输入字符串");
    if(fgets(str,sizeof(str),stdin)==NULL)//获取一行
    {
        printf("输入错误\n");
        return 0;
    }
    size_t len=strlen(str);
    if(len>0&&str[len-1]=='\n'){
        str[--len]='\0';
    }else if(len==sizeof(str)-1){
        while(getchar()!='\n');
    }
    if(len==0) {
        printf("不能输入空字符串\n");
        return 0; // 增加return终止程序
    }

    printf("请输入字符:");
    char c;
    scanf("%c",&c);

    int ans=get_position(str,c,len);
    if(ans == -1) {
        printf("字符未找到\n");
    } else {
        printf("查找到的位置:%d\n对应位置数据：%c", ans, str[ans-1]);
    }

    return 0;
}
