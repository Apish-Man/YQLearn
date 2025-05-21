/*
用补码计算 -15 + 27 （使用8位二进制表示）
1. 十进制转化为2进制原码
2. 2进制原码转2进制补码
3. 2进制补码运算
4. 2进制补码转化为2进制原码
5. 2进制原码转10进制
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 1. 十进制转化为2进制原码
char* trans_10to2(int x)
{
    // int 4个字节，需要32位
    // 0存符号位，1-31存数据，32位为'\0'
    char* ans=malloc(33*sizeof(char));
    memset(ans,'\0',33);
    // 符号位
    if(x<0) {
        ans[0]='1';
        x=-x;
    }
    else ans[0]='0';
    // 数据位
    int i=31;
    while(x){
        ans[i--]=(x%2+'0');
        x=x/2;
    }
    while(i>0) ans[i--]='0';
    // 终止符
    ans[32]='\0';
    return ans;
}

// 2. 2进制原码转2进制补码
char* trans_ytob(char *s)
{
    char* ans=malloc(33*sizeof(char));
    memset(ans,'\0',33);
    for(int i=0;i<33;i++) ans[i]=s[i];
    // 若为正数，返回
    if(ans[0]=='0') return ans;
    // 负数，将从31为开始，找到第一个1，前面全部取反
    int i=31,flag=0;
    while(i>0)
    {
        if(!flag&&ans[i--]=='1') flag=1;
        if(flag) ans[i--]=(1-(ans[i]-'0'))+'0';
    }
    return ans;
}

// 3. 2进制补码运算
char* compute(char* s1,char* s2)
{
    char* ans=malloc(33*sizeof(char));
    memset(ans,'\0',33);
    // 从最低位31开始计算到0位
    int i=31,carry=0;;
    while(i>=0){
        int tmp=(s1[i]-'0')+(s2[i]-'0')+carry;
        carry=tmp/2;
        ans[i--]=tmp%2+'0';
    }
    return ans;
}

// 4. 2进制补码转化为2进制原码
char* trans_btoy(char *s)
{
    char* ans=malloc(33*sizeof(char));
    memset(ans,'\0',33);
    for(int i=0;i<33;i++) ans[i]=s[i];
    // 若为正数，返回
    if(ans[0]=='0') return ans;
    // 负数，将从31为开始，找到第一个1，前面全部取反
    int i=31,flag=0;
    while(i>0)
    {
        if(!flag&&ans[i--]=='1') flag=1;
        if(flag) ans[i--]=(1-(ans[i]-'0'))+'0';
    }
    return ans;
}


// 5.2进制原码转10进制
int trans_2to10(char* s)
{
    int ans=0,weight=1,i=31;
    for(i=31;i>0;i--)
    {
        int tmp=(s[i]-'0')*weight;
        ans+=tmp;
        weight*=2;
    }
    if(s[0]=='0')
        return ans;
    else return -ans;
}
int main()
{
    int a=11,b=-2327;
    char *a1=trans_10to2(a),*b1=trans_10to2(b);
    printf("原码：\n%s\n%s\n",a1,b1);
    char *a2=trans_ytob(a1),*b2=trans_ytob(b1);
    printf("补码：\n%s\n%s\n",a2,b2);
    char *ans=compute(a2,b2);
    printf("2进制补码结果：\n%s\n",ans);
    char *ans1=trans_btoy(ans);
    printf("2进制原码结果：\n%s\n",ans1);
    int res=trans_2to10(ans1);
    printf("10进制结果：\n%d\n",res);
    return 0;
}