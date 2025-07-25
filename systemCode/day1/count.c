#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <ctype.h> 
#include <string.h> 

int main(int argc,char** argv)
{
    // 验证输入参数
    if(argc != 3)
    {
        printf("Usage:<%s> articleFile resultFile\n",argv[0]);
        return -1;
    }

    // 验证第一个文件是否存在
    int fd_article = open(argv[1],O_RDONLY);
    if(-1 == fd_article)
    {
        perror("file article");
        return -1;
    }

    // 第二个文件打开/创建
    int fd_result = open(argv[2],O_WRONLY | O_CREAT,00644);
    if(-1 == fd_result)
    {
        perror("file result");
        close(fd_article);
        return -1;
    }

    // 统计第一个文件字符
    int res[26] = {0};
    char buf[50] = {0};//每次读取50个字符
    ssize_t len = 0; //记录读取到的字符个数

    while(len=read(fd_article,buf,sizeof(buf) > 0))
    {
        // 遍历buf
        while(len--)
        {
            if(buf[len] < 'A' || buf[len] > 'z') continue;
            res[toupper(buf[len])-'A']++;
        }
    }
    
    if(len == -1)
    {
        perror("read");
        close(fd_article);
        close(fd_result);
        return -1;
    }

    char res_buf[50] = {0};
    for(int i=0;i<sizeof(res)/sizeof(res[0]);i++)
    {
        // 格式化结果
        sprintf(res_buf,"%c : %d\n",i+'a',res[i]);
        // 写入结果
        if(-1 == write(fd_result,res_buf,strlen(res_buf)))
        {
            perror("write");
            close(fd_article);
            close(fd_result);
            return -1;
        }
    }

    printf("统计完成！\n");

    close(fd_article);
    close(fd_result);
    return 0;
}