#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>

// 删除一个目录
int dirRmR(const char* dirpath);

int main(int argc,char** argv)
{
    // 1.输入验证
    if(argc < 2)
    {
        fprintf(stderr,"Usage <%s> dirPath\n",argv[0]);
        return -1;
    }

    // 2.目录是否存在
    if(dirRmR(argv[1]) == -1)
    {
        // 删除失败
        return -1;
    }

    return 0;
}

// 删除一个目录./tmp/rar 下有t1/1.txt t1/2.txt t2/1.txt t2/2.txt
int dirRmR(const char* dirpath)
{
    // 打开目录
    DIR* rmDir = opendir(dirpath);
    if(rmDir == NULL)
    {
        // 打开失败
        perror("opendir");
        return -1;
    }
    // 递归目录文件
    struct dirent* p;
    while(p = readdir(rmDir))
    {
        // 目录，递归
        if(p -> d_type == DT_DIR)
        { 
            // 跳过.和..
            if(strcmp(p->d_name,".")==0 || strcmp(p->d_name,"..")==0 )    
                continue;  
            char buf[255] = {0};
            sprintf(buf,"%s/%s",dirpath,p->d_name);
            if(dirRmR(buf) == -1)
            {
                return -1;// 删除失败
            }
        }
        // 文件，删除
        else{
            char buf[255] = {0};
            sprintf(buf,"%s/%s",dirpath,p->d_name);
            printf("rm %s\n",buf);
            if(-1 == unlink(buf))
            {
                perror("unlink");
            }
        }
    }
    // 删除空目录
    if(rmdir(dirpath) == -1)
    {
        perror("rmdir");
        return -1;
    }
    closedir(dirpath);
    
    printf("rm %s\n",dirpath);
    return 0;
}