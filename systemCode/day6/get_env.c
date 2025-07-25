#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc,char** argv)
{
    puts("get_env is running\n");

    char *oper=getenv("OPERATION");
    char *tar=getenv("TARGET");
    if(!oper || !tar)
    {
        fprintf(stderr,"getenv error");
    }
    // 判断
    if(strcmp(oper,"ls")==0)
    {
        // ls操作
        execlp("ls","ls",tar,NULL);
    }else if(strcmp(oper,"rm")==0)
    {
        // rm操作
        execlp("rm","rm",tar,NULL);
    }
    return 0;
}
