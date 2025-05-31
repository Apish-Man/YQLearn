/*************************************************************************
  > File Name:    12-1.c
  > Author:       魏新强
  > Description:  
  > Created Time: 2025年05月22日 星期四 20时37分28秒
 ************************************************************************/

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// 最大学生数量
#define STU_MAX 10
// 学科数目
#define NUM_SUB 3
// 学号长度
#define LEN_ID 5
// 姓名长度
#define LEN_NAME 10

// 定义结构体
typedef struct 
{
    char id[LEN_ID];//学号
    char name[LEN_NAME];//姓名
    double scores[NUM_SUB];//成绩
}Student;

// 当前长度
int stu_count=0;

// 定义课程名称
const char* COURSE_NAMES[NUM_SUB]={"语文","数学","英语"};

int main()
{
    // 1.定义初始变量
    Student stuArray[STU_MAX];
    stu_count=0;

    // 2.函数声明
    /*
    * 2.1 添加学生信息
    * 形参：结构体数组：Student* stuArray 当前人数: int* count,学科：COURSE_NAMES
    * 返回值：int ,表示是否成功
    * 返回1，表示成功
    */ 
    int addStudent(Student* stuArray,int *count,const char **couseName);
    /*
    * 2.2 显示所有学生信息
    * 形参：结构体数组：Student* stuArray 当前人数: int count
    * 返回值：void
    */ 
    int showAllStudent(Student* stuArray,int count,const char **couseName);
    /*
    * 2.3 计算每个学生的平均分和总分
    * 形参：结构体数组：Student* stuArray 当前人数: int count
    * 返回值：void
    */ 
    void caculateEveryStudentAverSum(Student* stuArray,int count);
    /*
    * 2.4 根据某科成绩排序
    * 形参：结构体数组：Student* stuArray 当前人数: int count 交换科目：int indexSub 升序降序：1升0降 int flag
    * 返回值：int ,表示是否成功
    * 返回1，表示成功
    * 返回-1，比较索引出错
    * 返回-2，元素交换出错
    */ 
    int sortStudent(Student* stuArray,int count,int indexSub,int flag);
    /*
    * 2.5 查找学生信息
    * 形参：结构体数组：Student* stuArray  当前人数: int count 学号：char* target
    * 返回值：int ,表示是否成功
    * 返回1，表示成功
    */ 
    int findStudent(Student* stuArray,int count,const char *target,const char **couseName);

    // 主函数
    int choice;
    do{
        printf("\t\t欢迎来到学生管理系统v2.0\n");
        printf("\t1. 添加学生信息：输入学号和三门成绩，存储到数组中。\n\t2. 显示所有学生信息：遍历数组，输出每个学生的学号和成绩。\n\t3. 计算每个学生的平均分和总分：遍历数组，计算每行的总分和平均分。\n");
        printf("\t4. 根据某科成绩排序：用户选择科目，然后按该科成绩排序，可以升序或降序。\n\t5. 查找学生信息：按学号查找，显示该生的成绩和平均分。\n\t6. 退出程序。\n");
        scanf("%d",&choice);
        switch(choice){
            case 1:
                // 添加学生信息
                addStudent(stuArray,&stu_count,COURSE_NAMES);
                break;
            case 2:
                // 显示所有学生信息
                showAllStudent(stuArray,stu_count,COURSE_NAMES);
                break;
            case 3:
                // 计算每个学生的平均分和总分
                caculateEveryStudentAverSum(stuArray,stu_count);
                break;
            case 4:
            {
                // 根据某科成绩排序
                // 1.选择排序学科，2.选择升序，降序
                int indexSub=-1,flag=-1;
                int valid=0;
                do{
                    printf("请选择排序科目:\n");
                    for(int i=0;i<NUM_SUB;i++) printf("%d:%s\t",i,COURSE_NAMES[i]);
                    printf("\n");
                    scanf("%d",&indexSub);
                    if(indexSub>=0&&indexSub<NUM_SUB) valid=1;
                    else printf("输入错误\n");
                }while(!valid);
                valid=0;
                do{
                    printf("请选择排序方式:\n0:降序\n1:升序\n");
                    scanf("%d",&flag);
                    if(flag==0||flag==1) valid=1;
                    else printf("输入错误\n");
                }while(!valid);
                sortStudent(stuArray,stu_count,indexSub,flag);
                showAllStudent(stuArray,stu_count,COURSE_NAMES);
                break;
            }
            case 5:
            {
                // 查找学生信息
                char target[LEN_ID]={0};
                // 去掉\n
                getchar();
                printf("请输入要查找的学号（学号长度为%d）：\n",LEN_ID-1);
                int flag=0;
                do{
                    // 1. 读取输入
                    if (fgets(target, LEN_ID, stdin) == NULL) {
                        printf("输入错误\n");
                    }
                    // 2. 去除换行符
                    size_t len = strlen(target);
                    if (len > 0 && target[len-1] == '\n') {
                        target[--len] = '\0';  // 安全替换换行符
                    } else if (len == sizeof(target)-1) {
                        // 输入缓冲区未清空时可能需要清理
                        while (getchar() != '\n');
                    }
                    // 3. 处理空输入
                    if (len == 0) {
                        printf("不能输入空字符串\n");
                    }
                    if(len=LEN_ID-1) flag=1;
                    else{
                        printf("学号输入有误，重新输入\n");
                    }
                }while(!flag);
                findStudent(stuArray,stu_count,target,COURSE_NAMES);
                break;
            }
            case 6:
                // 退出程序
                printf("欢迎下次使用👏\n");
                return 0;
                break;
            default:
                printf("输入选项无效!\n");
                break;
        }
    }while(choice!=6);

    return 0;
}

// 3.函数定义
    /*
    * 2.1 添加学生信息
    * 形参：结构体数组：Student* stuArray 当前人数: int* count,学科：COURSE_NAMES
    * 返回值：int ,表示是否成功
    * 返回1，表示成功
    */ 
int validId(Student* stuArray,int count,char* input);
int addStudent(Student* stuArray,int *count,const char **couseName)
{
    int len=*count;
    if(*count>=STU_MAX){
        printf("当前内存空间已满，无法插入\n");
        return -1;
    }
    //临时变量
    Student student;
    // 1.输入学号
    char tmp[LEN_ID];
    int flag=0;//输入是否有效
    while(getchar()!='\n');//清理缓冲区，前面选择时输入的\n
    do{
        printf("请输入%d位学号(全部为数字)，若多于%d位，将被截断\n",LEN_ID-1,LEN_ID-1);
        // 1.读取输入
        if(fgets(tmp,LEN_ID,stdin)==NULL)           //最多读入LEN_ID-1个字符，最后一个会自动补0
        {
            printf("输入错误\n");
            continue;
        }
        // 2.去除换行符
        size_t len=strlen(tmp);
        if (len >0 && tmp[len-1] == '\n') {//此时未读满
            tmp[--len] = '\0';  // 删除换行符,此时读入为abc\n\0
        }else if(len==sizeof(tmp)-1){//此时读满了
            while(getchar()!='\n');
        }
        // 3.处理空输入
        if(len==0){
            printf("不能输入空字符串\n");
        }
        if(strlen(tmp)==LEN_ID-1&&validId(stuArray,*count,tmp)==1) flag=1;
        else printf("学号输入无效，请仔细检查后重新输入\n");
    }while(!flag);
    strcpy(student.id,tmp); // 录入学号

    // 2.输入姓名
    char tmp_name[LEN_NAME];
    flag=0;//输入是否有效
    do{
        printf("请输入最高%d位姓名，若多于%d位，将被截断\n",LEN_NAME-1,LEN_NAME-1);
        if(fgets(tmp_name,LEN_NAME,stdin)==NULL)   //此处最大LEN_NAME，但fgets会补\0，最多读入LEN_NAME-1
        {
            printf("输入错误\n");
            continue;
        }
        //去除换行或清空缓冲区
        size_t len=strlen(tmp_name);
        if(len>0&&tmp_name[len-1]=='\n')
            tmp_name[--len]='\0';
        else if(len==sizeof(tmp_name)-1)
            while(getchar()!='\n');
        //空字符串
        if(len==0){
            printf("不能输入空字符串\n");
        }
        if(strlen(tmp_name)<LEN_NAME-1) flag=1;
        else printf("姓名输入无效，请仔细检查后重新输入\n");
    }while(!flag);
    // 录入姓名
    strcpy(student.name,tmp_name);

    // 3.录入成绩
    for(int i=0;i<NUM_SUB;)
    {
        double tmp=0.0;
        printf("请输入%s成绩:\n",couseName[i]);
        int key=scanf("%lf",&tmp);//使用key统计正确的输入，可过滤错误输入
        // 清除缓冲区
        while(getchar()!='\n');
        // 验证成绩有效性
        if(!key||tmp<0||tmp>100) {
            printf("录入成绩不合要求，请重新输入\n");
            continue;
        }
        student.scores[i]=tmp;//录入成绩
        i++;
    }
    stuArray[*count]=student;
    (*count)++;
    return 1;
}
    /*
    * 2.1 检查学号输入是否有效
    * 形参：学号：Student* stuArray 当前人数: int count 当前学号 char* p
    * 返回1，表示成功
    */ 
int validId(Student* stuArray,int count,char* input)
{
    int len=strlen(input);
    for(int i=0;i<len;i++)
    {
        if(input[i]<'0'||input[i]>'9') return -1;//输入不符合
    }
    for(int i=0;i<count;i++){
        if(strcmp(stuArray[i].id,input)==0) return 0;//重复
    }
    return 1;
}

    /*
    * 2.2 显示所有学生信息
    * 形参：结构体数组：Student* stuArray 当前人数: int count
    * 返回值：void
    */ 
int showAllStudent(Student* stuArray,int count,const char **couseName)
{
    printf("学号：\t姓名：\t");
    for(int i=0;i<NUM_SUB;i++)
    {
        printf("%s\t\t",couseName[i]);
    }
    for(int i=0;i<count;i++)
    {
        printf("\n%s\t%s\t",stuArray[i].id,stuArray[i].name);
        for(int j=0;j<NUM_SUB;j++)
        {
            printf("%.3lf\t\t",stuArray[i].scores[j]);
        }
    }
    printf("\n");
    return 0;
}

    /*
    * 2.3 计算每个学生的平均分和总分
    * 形参：结构体数组：Student* stuArray 当前人数: int count
    * 返回值：void
    */ 
void caculateEveryStudentAverSum(Student* stuArray,int count)
{
    printf("学号：\t姓名：\t总分\t平均分\t\n");
    for(int i=0;i<count;i++)
    {
        double sum=0,aver=0;
        for(int j=0;j<NUM_SUB;j++)
        {
            sum+=stuArray[i].scores[j];
        }
        aver=sum/NUM_SUB;
        printf("%s\t%s\t%.3lf\t%.3lf\n",stuArray[i].id,stuArray[i].name,sum,aver);
    }
}

    /*
    * 2.5 查找学生信息
    * 形参：结构体数组：Student* stuArray  当前人数: int count 学号：char* target
    * 返回值：int ,表示是否成功
    * 返回1，表示成功
    */ 
int findStudent(Student* stuArray,int count,const char *target,const char **couseName)
{
    for(int i=0;i<count;i++)
    {
        if(strcmp(stuArray[i].id,target)==0)
        {
            // 找到
            // 打印表头
            printf("学号\t姓名\t");
            for(int j=0;j<NUM_SUB;j++) printf("%s\t\t",couseName[j]);
            printf("平均分\t成绩\t\n");
            // 打印数据
            double sum=0,ave=0;
            printf("%s\t%s\t",stuArray[i].id,stuArray[i].name);
            for(int j=0;j<NUM_SUB;j++) {
                sum+=stuArray[i].scores[j];
                printf("%.3lf\t\t",stuArray[i].scores[j]);
            }
            ave=sum/NUM_SUB;
            printf("%.3lf\t%.3lf\t\n",ave,sum);
            return 1;
        }
    }
    printf("查无此人\n");
    return 0;
}

    /*
    * 2.4 根据某科成绩排序
    * 形参：结构体数组：Student* stuArray 当前人数: int count 交换科目：int indexSub 升序降序：1升0降 int flag
    * 返回值：int ,表示是否成功
    * 返回1，表示成功
    * 返回-1，比较索引出错
    * 返回-2，元素交换出错
    */ 
// 交换x,y
int swap(int x,int y,Student* stuArray)
{
    Student tmp=stuArray[x];
    stuArray[x]=stuArray[y];
    stuArray[y]=tmp;

    return 1;
}

int sortStudent(Student* stuArray,int count,int indexSub,int flag)
{
    if(indexSub<0||indexSub>NUM_SUB) return -1;//比较索引出错
    for(int i=0;i<count-1;i++)
    {
        // n-1趟冒泡，每次确定1个的位置，n-1个确定，最后一个也确定
        int is_swap=0;//标志是否交换
        for(int j=0;j<count-1-i;j++)
        {
            if(flag){
                // 升序
                if(stuArray[j].scores[indexSub]>stuArray[j+1].scores[indexSub]){
                    if(swap(j,j+1,stuArray)==1)  //交换成功，返回1
                        is_swap=1;
                    else{
                        printf("元素交换出错\n");
                        return -2;//元素交换出错
                    }
                }
            }else{
                // 降序
                if(stuArray[j].scores[indexSub]<stuArray[j+1].scores[indexSub]){
                    if(swap(j,j+1,stuArray)==1)  //交换成功，返回1
                        is_swap=1;
                    else{
                        printf("元素交换出错\n");
                        return -2;//元素交换出错
                    }
                }
            }
        }
        if(!is_swap) return 1;//结束，没有交换，已经有序
    }
    return 0;
}
