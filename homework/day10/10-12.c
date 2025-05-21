/*************************************************************************
  > File Name:    10-12.c
  > Author:       魏新强
  > Description:  
  > Created Time: 2025年05月16日 星期五 21时37分01秒
 ************************************************************************/

#include <stdio.h>

// 菜单函数
void menu()
{
  printf("请输入选项：\n");
  printf("1、加法 2、减法 3、乘法 4、除法 0.退出\n");
}

// 加法
double add(double a,double b)
{
  return a+b;
}

// 减法
double minus(double a,double b)
{
  return a-b;
}

// 乘法
double multiply(double a,double b)
{
  return a*b;
}

// 除法
double divide(double a,double b)
{
  return a/b;
}

int main(int argc,char *argv[])
{
  int choice;
  while(1)
  {
    menu();
    scanf("%d",&choice);
    switch(choice){
      case 1:{
        double a,b;
        printf("请输入要相加的两个数：\n");
        scanf("%lf %lf",&a,&b);
        double ans=add(a,b);
        printf("%.3lf+%.3lf的结果为：%.3lf\n",a,b,ans);
        break;
      }
      case 2:{
        double a,b;
        printf("请输入要相减的两个数：\n");
        scanf("%lf %lf",&a,&b);
        double ans=minus(a,b);
        printf("%.3lf-%.3lf的结果为：%.3lf\n",a,b,ans);
        break;
      }
      case 3:{
        double a,b;
        printf("请输入要相乘的两个数：\n");
        scanf("%lf %lf",&a,&b);
        double ans=multiply(a,b);
        printf("%.3lf*%.3lf的结果为：%.3lf\n",a,b,ans);
        break;
      }
      case 4:{
        double a,b;
        printf("请输入要相除的两个数：\n");
        scanf("%lf %lf",&a,&b);
        double ans=divide(a,b);
        printf("%.3lf➗%.3lf的结果为：%.3lf\n",a,b,ans);
        break;
      }
      case 0:{
        printf("感谢使用!\n");
        return 0;
      }
      default:printf("请选择正确选项！\n");break;
    }
  }
  return 0;
}
