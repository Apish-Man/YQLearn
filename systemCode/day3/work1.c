#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>
#include <time.h>

int getFileType(const struct stat* st,char *info);
int getFileOwnerPerm(const struct stat* st,char *info);
int getFileGroupPerm(const struct stat* st,char *info);
int getFileOtherPerm(const struct stat* st,char *info);

int getLinkNum(const struct stat* st,char *info);

int getUserName(const struct stat* st,char *info);

int getGroupName(const struct stat* st,char *info);

int getFileSize(const struct stat* st,char *info);

int getFileMTime(const struct stat* st,char *info);

int getFileName(const struct stat* st,char *info,const char* filePath);

int main(int argc,char** argv)
{
    if(argc < 2)
    {
        fprintf(stderr,"Usage:<%s> filePath\n",argv[0]);
        return -1;
    }
    // 1.获取文件信息
    struct stat status={0};
    if(stat(argv[1],&status) == -1)
    {
        perror("stat");
        return -1;
    }
    // 2.解析文件信息
    char info_buf[64]={0};
    getFileType(&status,info_buf);
    getFileOwnerPerm(&status,info_buf);
    getFileGroupPerm(&status,info_buf);
    getFileOtherPerm(&status,info_buf);

    getLinkNum(&status,info_buf);

    getUserName(&status,info_buf);

    getGroupName(&status,info_buf);
    
    getFileSize(&status,info_buf);

    getFileMTime(&status,info_buf);

    getFileName(&status,info_buf,argv[1]);

    puts(info_buf);
    return 0;
}

int getLinkNum(const struct stat* st,char *info)
{
    char buf[16]={0};
    if(S_ISDIR(st->st_mode)==1)
    {
        // 后续实现
        sprintf(buf," %lu ",1l);
    }
    else
    {
        sprintf(buf," %lu ",st->st_nlink);
    }
    strcat(info,buf);
    return 0;
}

int getUserName(const struct stat* st,char *info)
{
    struct passwd* pwd = getpwuid(st->st_uid);
    char buf[16] = {0};
    sprintf(buf,"%s ",pwd->pw_name);
    strcat(info,buf);
    return 0;
}

int getGroupName(const struct stat* st,char *info)
{
    struct group* grp = getgrgid(st->st_uid);
    char buf[16] = {0};
    sprintf(buf,"%s ",grp->gr_name);
    strcat(info,buf);
    return 0;
}

int getFileSize(const struct stat* st,char *info)
{
    char buf[16] = {0};
    sprintf(buf,"%ld ",st->st_size);
    strcat(info,buf);
    return 0;
}

int getFileMTime(const struct stat* st,char *info)
{
    char buf[40] = {0};
    // 注意数据覆盖
    // 获取当前系统时间
    time_t t  = time(NULL);
    struct tm* sysTime = localtime(&t);
    int year = sysTime->tm_year;

    struct tm* fileMTime = localtime(&st->st_mtime);

    if(fileMTime->tm_year == year)
    {
        sprintf(buf,"%2d月 %02d %02d:%02d ",fileMTime->tm_mon+1, fileMTime->tm_mday, fileMTime->tm_hour, fileMTime->tm_min);
    }
    else
    {
        sprintf(buf,"%2d月 %02d %4d ",fileMTime->tm_mon+1, fileMTime->tm_mday, fileMTime->tm_year + 1900);
    }

    strcat(info,buf);
    return 0;
}

int getFileName(const struct stat* st,char *info,const char* filePath)
{
    char tmp[50] = {0};
    strcpy(tmp,filePath); //strtok会修改源字符串，拷贝一份进行修改
    // strtok分割/
    char* token = strtok(tmp,"/");
    char* last = token;     // 存储上一次分割得到的下标，在toekn为NULL时，其指向最后一个字符串
    while(token!=NULL)
    {
        last = token;
        token = strtok(NULL,"/");
    }
    if(last==NULL && filePath[0]=='/') strcat(info,"/");
    else if(last!=NULL) strcat(info,last);
    return 0;
}

int getFileOwnerPerm(const struct stat* st,char *info)
{
    switch (st->st_mode & 0700)
    {
    case 0700:
        strcat(info,"rwx");
        break;
    
    case 0600:
        strcat(info,"rw-");
        break;
    
    case 0500:
        strcat(info,"r-x");
        break;
    
    case 0400:
        strcat(info,"r--");
        break;
    
    case 0300:
        strcat(info,"-wx");
        break;
    
    case 0200:
        strcat(info,"-w-");
        break;
    
    case 0100:
        strcat(info,"--r");
        break;

    default:
        strcat(info,"---");
        break;
    }
    return 0;
}
int getFileGroupPerm(const struct stat* st,char *info)
{
    switch (st->st_mode & 0070)
    {
    case 0070:
        strcat(info,"rwx");
        break;
    
    case 0060:
        strcat(info,"rw-");
        break;
    
    case 0050:
        strcat(info,"r-x");
        break;
    
    case 0040:
        strcat(info,"r--");
        break;
    
    case 0030:
        strcat(info,"-wx");
        break;
    
    case 0020:
        strcat(info,"-w-");
        break;
    
    case 0010:
        strcat(info,"--r");
        break;

    default:
        strcat(info,"---");
        break;
    }
    return 0;
}
int getFileOtherPerm(const struct stat* st,char *info)
{
    switch (st->st_mode & 0007)
    {
    case 0007:
        strcat(info,"rwx");
        break;
    
    case 0006:
        strcat(info,"rw-");
        break;
    
    case 0005:
        strcat(info,"r-x");
        break;
    
    case 0004:
        strcat(info,"r--");
        break;
    
    case 0003:
        strcat(info,"-wx");
        break;
    
    case 0002:
        strcat(info,"-w-");
        break;
    
    case 0001:
        strcat(info,"--r");
        break;

    default:
        strcat(info,"---");
        break;
    }
    return 0;
}

int getFileType(const struct stat* st,char *info)
{
    if(S_ISLNK(st->st_mode)==1)
        info[0]='l';
    else if(S_ISREG(st->st_mode)==1)
        info[0]='-';
    else if(S_ISDIR(st->st_mode)==1)
        info[0]='d';
    else if(S_ISCHR(st->st_mode)==1)
        info[0]='c';
    else if(S_ISBLK(st->st_mode)==1)
        info[0]='b';
    else if(S_ISSOCK(st->st_mode)==1)
        info[0]='s';
    else if(S_ISFIFO(st->st_mode)==1)
        info[0]='p';
    return 0;
}
