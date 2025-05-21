/*
电影院为了答谢影迷的支持，在某一排的某一列座位上放置了一个大礼包，放置礼物的位置具
有这样的规则(行和列的平方和为开店日期 512(5月12日）); 请设计程序找出大礼包的位置，(假
定电影院有20排，每排25个座位)
*/
#include <stdio.h>

#define LEN 20
#define WIDTH 25
#define ANS 512

int A[LEN][WIDTH];

int main()
{
    int i=0,j=0;
    // 遍历数组，查找
    for(i=0;i<LEN;i++)
    {
        for(j=0;j<WIDTH;j++)
        {
            int tmp=i*i+j*j;
            if(tmp==ANS)
            {
                printf("行：%d 列：%d",i+1,j+1);
            }
        }
    }
    return 0;
}