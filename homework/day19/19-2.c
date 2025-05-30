/*************************************************************************
  > File Name:    19-2.c
  > Author:       魏新强
  > Description:  
  > Created Time: 2025年05月30日 星期五 09时19分49秒
 ************************************************************************/

#include <stdio.h>

#define MAX 3

typedef struct {
    char *name;
    int id;
    float price;
    short stock;
}Product;

//添加商品信息
//int addProduct(Product *product)

//初始化
int init(Product *productArray,int *len)
{
    productArray->name="肉松小贝";
    productArray->id=100001;
    productArray->price=5.21;
    productArray->stock=100;
    (*len)++;
    *(productArray+1)=(Product){"肉松小贝",10001,5.21,100};//C语言中结构体不能直接通过{...}语法赋值给已存在的结构体变量。
    (*len)++;
    return 1;
}

//显示信息
void showInfo(const Product* array,int len)
{
    printf("商品名称\tID\t价格\t库存数量\t\n");
    for(int i=0;i<len;i++)
    {
        printf("%s\t%d\t%.3lf\t%d\t\n",array[i].name,array[i].id,array[i].price,array[i].stock);
    }
}

int main(int argc,char *argv[])
{
    Product productArray[MAX];
    int len=0;//记录当前长度
    //字符指针无法使用%s输入
    init(productArray,&len);
    showInfo(productArray,len);

    return 0;
}
