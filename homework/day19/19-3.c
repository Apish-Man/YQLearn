/*************************************************************************
  > File Name:    19-2.c
  > Author:       魏新强
  > Description:  
  > Created Time: 2025年05月30日 星期五 09时19分49秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 5

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
    productArray->name="肉松小贝0";
    productArray->id=100001;
    productArray->price=5.21;
    productArray->stock=100;
    (*len)++;
    *(productArray+1)=(Product){"肉松小贝1",10002,52.1,100};//C语言中结构体不能直接通过{...}语法赋值给已存在的结构体变量。
    (*len)++;
    *(productArray+2)=(Product){"肉松小贝2",10003,521,100};
    (*len)++;
    *(productArray+3)=(Product){"肉松小贝3",10004,520,100};
    (*len)++;
    *(productArray+4)=(Product){"肉松小贝4",10005,0.521,100};
    (*len)++;
    return 1;
}

//显示信息
void showInfo(const Product* array,int len)
{
    printf("商品名称\tID\t价格\t\t库存数量\t\n");
    for(int i=0;i<len;i++)
    {
        printf("%s\t%d\t%.3f\t\t%d\t\n",array[i].name,array[i].id,array[i].price,array[i].stock);
    }
}

//查询价格最高的商品
Product getPriceMax(const Product* array,int len)
{
    //if(len==0) return NULL; 结构体无法返回空
    Product max=array[0];
    for(int i=1;i<len;i++)
    {
        if(array[i].price>max.price) max=array[i];
    }
    return max;
}

int main(int argc,char *argv[])
{
    Product productArray[MAX];
    int len=0;//记录当前长度
    //字符指针无法使用%s输入
    init(productArray,&len);
    showInfo(productArray,len);
    Product max=getPriceMax(productArray,len);
    
    printf("单价最高商品：\n");
    printf("%s\t%d\t%.3f\t\t%d\t\n",max.name,max.id,max.price,max.stock);

    printf("----------------动态分配----------------------\n");
    //动态分配
    Product* ptrArray=malloc(sizeof(Product)*MAX);
    if(ptrArray==NULL) return -1;

    //初始化
    memset(ptrArray,0,sizeof(Product)*MAX);
    int len2=0;//记录当前长度
    //字符指针无法使用%s输入
    init(ptrArray,&len2);
    showInfo(ptrArray,len2);
    Product max2=getPriceMax(ptrArray,len2);
    
    printf("单价最高商品：\n");
    printf("%s\t%d\t%.3f\t\t%d\t\n",max2.name,max2.id,max2.price,max2.stock);
    
    free(ptrArray);
    ptrArray=NULL;
    return 0;
}
