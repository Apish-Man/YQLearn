#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc,char** argv)
{
    // 参数校验
    if(argc < 2)
    {
        printf("Usage:<%s> file1 file2 ...\n",argv[0]);
        return -1;
    }
    
    // 验证文件是否存在
    for(int i=1;i<argc;i++)
    {
        int res = access(argv[i],F_OK);
        if(0 == res)
        {
            printf("%s : File exist!\n",argv[i]);
        }else{
            perror(argv[i]);
        }
    }

    return 0;
}