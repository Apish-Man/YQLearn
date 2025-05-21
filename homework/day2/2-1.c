#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

const char* CONVERT[16]={"0000","00001","0010","0011",
                        "0100","0101","0110","0111",
                        "1000","1001","1010","1011",
                        "1100","1101","1110","1111"};

// 16->2
char* convert16_2(const char* s)
{
    static char result[40]="";
    int i=0,len_res=0;
    for(i=0;i<strlen(s);i++)
    {
        int index=0;
        if(s[i]<='9'&&s[i]>='0') index=s[i]-'0';
        else if(s[i]>'9') index=s[i]-'A'+10;
        
        // 保存结果
        for(int i=0;i<4;i++){
            result[len_res++]=CONVERT[index][i];
        }
        result[len_res++]=' ';
    }
    // printf("%s\n",result);
    return result;
}
// 16->8,需要先转化为2进制，然后转化为8进制，每次取3位，如果不足，整数部分左补0，小数部分右补0
char* convert2_8(const char* s,int flag)
{
    char *result=(char*)malloc(sizeof(char) * 40);
    memset(result,'\0',sizeof(result));
    int len_result=0;
    // flag=1表示整数，flag=0表示小数
    if(flag==1)
    {
        int len_s=strlen(s),i=0;
        // printf("%d\n",len_s);
        int flag=0;//记录当前位数所在，只取0,1,2；
        int num=0;
        for(i=len_s-1;i>=0;i--){
            // 若当前字符有效，记录并取值，若无效，进行下一个
            if(s[i]=='0'||s[i]=='1')
            {
                num=num+(s[i]-'0')*pow(2,flag);
                if(flag==2)
                {
                    flag=0;
                    result[len_result++]='0'+num;
                    num=0;
                }else
                    flag++;
            }else continue;
        }
        // 最后一个处理
        if(num!=0) result[len_result++]='0'+num;
        // 字符串反转后返回
        char tmp[40];
        strcpy(tmp,result);
        i=len_result-1;
        int j=0;
        while(i>=0) result[j++]=tmp[i--];
        // printf("%s\n",result);
        return result;
    }else if(flag==0)
    {
        // 从前向后处理
        int len_s=strlen(s),i=0;
        // printf("%d\n",len_s);
        int flag=2;//记录当前位数所在，只取0,1,2；
        int num=0;
        for(i=0;i<len_s;i++){
            // 若当前字符有效，记录并取值，若无效，进行下一个
            if(s[i]=='0'||s[i]=='1')
            {
                num=num+(s[i]-'0')*pow(2,flag);
                if(flag==0)
                {
                    flag=2;
                    result[len_result++]='0'+num;
                    num=0;
                }else
                    flag--;
            }else continue;
        }
        // 最后一个处理
        if(num!=0) result[len_result++]='0'+num;
        // printf("%s\n",result);
        return result;
    }
}

int main()
{
    char s[10]="0x3A.2C";
    //提取整数部分
    char s1[10]="";
    int i=0,s1_len=0;
    for(i=2;i<strlen(s);i++)
    {
        if(s[i]=='.') break;
        if(s[i]!='0') s1[s1_len++]=s[i];
    }
    // printf("%d %d\n",s1_len,strlen(s1));
   	//提取小数部分
    char s2[10]="";
    int s2_len=0;
    for(i=2+strlen(s1)+1;i<strlen(s);i++)
    {
        s2[s2_len++]=s[i];
    }
    // printf("%s %s",s1,s2);
    // 16-2
    printf("二进制：%s.",convert16_2(s1));
    printf("%s\n",convert16_2(s2));
    printf("八进制：%s.",convert2_8(convert16_2(s1),1));
    printf("%s\n",convert2_8(convert16_2(s2),0));
}