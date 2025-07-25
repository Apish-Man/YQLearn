#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc,char** argv)
{
    if(argc < 2)
    {
        fprintf(stderr,"Usage <%s> operation\n",argv[0]);
        return -1;
    }

    pid_t cpid = fork(); 
    if(cpid < 0)
    {
        perror("fork");
        return -1;
    }else if(cpid == 0)
    {
        //子进程
        // 直接在此处进行判断，来决定子进程执行什么
        if(strcmp(argv[1],"ls")==0)
        {
            // ls操作
            execlp("ls","ls","./",NULL);
        }else if(strcmp(argv[1],"rm")==0)
        {
            // rm操作
            execlp("rm","rm","./a.out",NULL);
        }
        // 正常执行的话，此处不会执行
        exit(-1);
    }
    else
    {
        //父进程
        printf("Parent is running\n");
        int res = 0;
        wait(&res);
        printf("Parent:%d\n",WEXITSTATUS(res));
    }
    return 0;
}
