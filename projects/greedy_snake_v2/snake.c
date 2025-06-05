/*************************************************************************
  > File Name:    snake.c
  > Author:       魏新强
  > Description:  
  > Created Time: 2025年06月03日 星期二 20时22分00秒
 ************************************************************************/

 #include "snake.h"

/*
* 初始化蛇函数，创建一个蛇头一个蛇尾一个蛇中间，同时会随机一个位置
* @param container 网格的当前状态
* @param int wid  边界宽度
* @param int len  边界长度
* @return Snake,返回初始化蛇，return NULL表示失败
*/
NODE* init_snake(Block (*newcontainer)[WIDTH_BOUNDARY],Block (*oldcontainer)[WIDTH_BOUNDARY],int len,int wid)
{
  srand(time(NULL));
  //创建头结点
  NODE* snake=createSnakeNode(newcontainer,len,wid);
  if(!snake) 
  {
    printf("头节点创建失败\n");
    return NULL;
  }
  // 更新对应container
  int tmpx=snake->data.i,tmpy=snake->data.j;
  oldcontainer[tmpx][tmpy]=newcontainer[tmpx][tmpy];
  newcontainer[tmpx][tmpy].type=1;//snake
  newcontainer[tmpx][tmpy].type_index=0;
  //dir是方向，范围为0-3
  int dir=rand()%4;
  // int dir=3;
  newcontainer[tmpx][tmpy].dir=dir;
  
  // 在头节点的另外三个位置随机产生一个位置
  int directions[4][2]={{0,-1},{0,1},{-1,0},{1,0}};//上0，下1，左2，右3
  int dir_i=(-1)*directions[dir][1];
  int dir_j=(-1)*directions[dir][0];
  
  // 创建中间节点
  NODE* middle=createSnakeNode(newcontainer,len,wid);
  //设置中间节点在头节点后，且和其方向一致
  middle->data.i=snake->data.i+dir_i;
  middle->data.j=snake->data.j+dir_j;
  if(!middle) return NULL;
  // 尾插法
  int flag=dclist_tail_add(&snake,middle);
  if(flag==-1) 
  {
    printf("尾插失败\n");
    return NULL;
  }
  // 更新对应container
  int midx=middle->data.i,midy=middle->data.j;
  oldcontainer[midx][midy]=newcontainer[midx][midy];
  newcontainer[midx][midy].type=1;//snake
  newcontainer[midx][midy].type_index=1;
  //dir是方向，范围为0-3
  newcontainer[midx][midy].dir=dir;

  // 创建尾巴,下一个节点必须在上一个节点的后面，并且他的方向应该和前面节点的上一个方向一致
  NODE* tail=createSnakeNode(newcontainer,len,wid);
  //设置尾巴节点在头节点后，且和其方向一致
  tail->data.i=middle->data.i+dir_i;
  tail->data.j=middle->data.j+dir_j;
  if(!tail) return NULL;
  // 尾插法
  flag=dclist_tail_add(&snake,tail);
  if(flag==-1) 
  {
    printf("尾插失败\n");
    return NULL;
  }
  // 更新对应container
  int tailx=tail->data.i,taily=tail->data.j;
  oldcontainer[tailx][taily]=newcontainer[tailx][taily];
  newcontainer[tailx][taily].type=1;//snake
  newcontainer[tailx][taily].type_index=2;
  // //dir是方向，范围为0-3
  newcontainer[tailx][taily].dir=dir;
  return snake;
}

//创建节点
NODE* createSnakeNode(Block (*newcontainer)[WIDTH_BOUNDARY],int rows,int cols)//rows对应数组行数(y轴)，cols对应列数(x轴)
{
  //创建头结点
  NODE* snake=(NODE*)malloc(sizeof(NODE));
  if(!snake) return NULL;
  // 头节点，x,y赋值
  int tmp_x,tmp_y;
  //找到合适位置---此时不考虑碰撞，只考虑生成多个无关数据
  do{
    //x是横坐标，范围为mod（WIDTH_BOUNDARY-5*2）5-9  5为设定的边界距离
    tmp_x=rand()%(cols-2*5)+5;
    //y是横坐标，范围为mod (LENTH_BOUNDARY-5*2）
    tmp_y=rand()%(rows-2*5)+5;
  }while(newcontainer[tmp_y][tmp_x].type!=0);
  snake->data.i=tmp_y;
  snake->data.j=tmp_x;
  snake->next=NULL;
  snake->prev=NULL;
  return snake;
}


