#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

// 统计子目录和文件数量
int countNum(const char* filePath,int *numFile,int* numDir);
// 判断当前目录是否为空，是的话返回1，否则返回0
int isEmpty(const char* filePath);

int main(int argc,char** argv)
{
    if(argc < 2)
    {
        fprintf(stderr,"Usage <%s> filePath\n",argv[0]);
        return -1;
    }

    // 子进程和父进程对同一进程进行操作，实现类似“共享”的操作，统计数据可超出255
    
    int numFile=0,numDir=0; //创建一个变量，允许子进程对其操作，(不安全行为？)
    pid_t cpid = vfork(); 
    if(cpid < 0)
    {
        perror("fork");
        return -1;
    }else if(cpid == 0)
    {
        //子进程
        countNum(argv[1],&numFile,&numDir);
        printf("childProcess:File %3d, Dir %3d\n",numFile,numDir);
        // 进程退出
        _exit(0);
    }
    else
    {
        //父进程
        printf("Parent is waitting the child\n");
        int totalNum = numFile + numDir;
        wait(NULL);
        printf("Parent:%d\n",totalNum);
    }
    return 0;
}

// 统计子目录和文件数量
int countNum(const char* filePath,int *numFile,int* numDir)
{
    DIR* dr = opendir(filePath);
    if(dr == NULL)
    {
        perror("opendir");
        return -1;//目录不存在
    }
    // 遍历目录
    struct dirent* fd = NULL;
    while(fd = readdir(dr))
    {
        if(strcmp(fd->d_name,".")==0 || strcmp(fd->d_name,"..")==0)
            continue;
        if(fd->d_type == DT_DIR)
        {
            // 目录，递归
            (*numDir)++;
            char nextPath[255]={0};
            sprintf(nextPath,"%s/%s",filePath,fd->d_name);
            countNum(nextPath,numFile,numDir);
        }else if(fd->d_type == DT_REG)
        {
            // 文件，记录
            (*numFile)++;
        }
    }

    return 0;
}

// 判断当前目录是否为空，是的话返回1，否则返回0
int isEmpty(const char* filePath)
{
    DIR* dr = opendir(filePath);
    if(dr == NULL)
    {
        perror("opendir");
        return -1;//目录不存在
    }
    // 遍历目录
    int count = 0;
    struct dirent* fd = NULL;
    while(fd = readdir(dr))
    {
        if(strcmp(fd->d_name,".") || strcmp(fd->d_name,".."))
            continue;
        count++;
    }
    if(count==0) return 1;
} 