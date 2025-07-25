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

    // 创建进程使用返回值进行统计,linux中退出码范围为0-255，超出会被截断(%255)，只能用来统计数量较少目录
    pid_t cpid = fork(); 
    if(cpid < 0)
    {
        perror("fork");
        return -1;
    }else if(cpid == 0)
    {
        //子进程
        int numFile=0,numDir=0;
        countNum(argv[1],&numFile,&numDir);
        printf("childProcess:File %3d, Dir %3d\n",numFile,numDir);
        // 进程退出
        exit((numDir+numFile));
    }
    else
    {
        //父进程
        printf("Parent is waitting the child\n");
        int totalNum = 0;
        wait(&totalNum);
        printf("Parent:%d\n",WEXITSTATUS(totalNum));
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