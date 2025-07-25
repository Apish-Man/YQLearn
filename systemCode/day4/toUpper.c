#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>

//mkdir tmp/rar/t1 tmp/rar/t2 -p
//touch tmp/rar/t1/1.txt tmp/rar/t1/2.txt tmp/rar/t2/1.txt tmp/rar/t2/2.txt

// 递归大写文件名
int upperName(const char* dirpath);

int main(int argc,char** argv)
{
    // 1.输入验证
    if(argc < 2)
    {
        fprintf(stderr,"Usage <%s> dirPath\n",argv[0]);
        return -1;
    }

    // 2.大写
    if(upperName(argv[1]) == -1)
    {
        // 失败
        return -1;
    }

    return 0;
}

// 对单个文件进行重命名
static int __upperOneName(const char* oldname)
{
    // 1.获取重命名后的名字
    char buf[255] = {0};
    int len = strlen(oldname);
    for(int i=0;i<len;i++)
    {
        if(oldname[i]<='z' && oldname[i]>='a')
            buf[i]=oldname[i]-'a'+'A';
        else buf[i]=oldname[i];
    }
    // 2.重命名
    if(-1 == rename(oldname,buf)){
        perror("rename");
        return -1;
    }
    return 0;
}

// 递归大写一个目录下所有文件名./tmp/rar 下有t1/1.txt t1/2.txt t2/1.txt t2/2.txt
int upperName(const char* dirpath)
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
            if(upperName(buf) == -1)
            {
                return -1;
            }
            // 获取当前目录
            char cwd[1024];
            getcwd(cwd, sizeof(cwd));
            // 切换工作目录
            chdir(dirpath);
            printf("rename %s/%s\n",dirpath,p->d_name);
            if(-1 == __upperOneName(p->d_name))
            {
                return -1;
            }
            // 切换回原始目录
            chdir(cwd);
        }
        // 文件，进行大写
        else{
            // 获取当前目录
            char cwd[1024];
            getcwd(cwd, sizeof(cwd));
            // 切换工作目录
            chdir(dirpath);
            printf("rename %s/%s\n",dirpath,p->d_name);
            if(-1 == __upperOneName(p->d_name))
            {
                return -1;
            }
            // 切换回原始目录
            chdir(cwd);
        }
    }
    
    return 0;
}