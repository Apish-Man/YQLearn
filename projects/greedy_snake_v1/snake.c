/*************************************************************************
  > File Name:    snake.c
  > Author:       魏新强
  > Description:  
  > Created Time: 2025年06月03日 星期二 20时22分00秒
 ************************************************************************/

 #include "snake.h"

/*
* 初始化食物函数，要求weight和icon数组长度一致且对应
* @param int *weight 食物权重
* @param char *icon  食物对应图标
* @param int len  数组长度
* @return Food *foodArray,返回长度为len的食物列表，return NULL表示失败
*/
Food* init_food(int *weight,char **icon,int len)
{
  Food *foodArray=(Food *)malloc(sizeof(Food)*len);
  if(!foodArray){
    perror("init_food");//内存分配失败
    return NULL;
  }
  for(int i=0;i<len;i++)
  {
    foodArray[i].weight=weight[i];
    foodArray[i].icon=icon[i];
  }
  return foodArray;
}

/*
* 初始化蛇函数，创建一个蛇头一个蛇尾，同时会随机一个位置
* @param int *vis 网格的当前状态，在随机位置时进行标记
* @param int wid  边界宽度
* @param int len  边界长度
* @return Snake,返回初始化蛇，return NULL表示失败
*/
Snake init_snake(int (*vis)[WIDTH_BOUNDARY],int wid,int len)
{
  srand(time(NULL)); 
  //创建头结点
  Snake snake=(SnakeNode*)malloc(sizeof(SnakeNode));
  if(!snake) return NULL;
  snake->icon=1;
  //x是横坐标，范围为1-（WIDTH_BOUNDARY-2）1-48
  snake->x=rand()%(wid-2)+1;
  //y是横坐标，范围为1-LENTH_BOUNDARY-2）1-48
  snake->y=rand()%(len-2)+1;
  //dir是方向，范围为0-3
  snake->dir=rand()%4;
  snake->next=NULL;
  snake->prev=NULL;
  vis[snake->x][snake->y]=1;
  
  // 创建尾巴,下一个节点必须在上一个节点的后面，并且他的方向应该和前面节点的上一个方向一致
  SnakeNode* tail=(SnakeNode*)malloc(sizeof(SnakeNode));
  if(!tail) 
  {
    free(snake);
    snake=NULL;
    return NULL;
  }
  snake->icon=3;
  int directions[4][2]={{0,-1},{-1,0},{0,1},{1,0}};//上，左，下，右，逆时针转
  int tmp_x,tmp_y,tmp_index;
  //找到合适位置
  do{
    tmp_index=rand()%4;
    tmp_x=snake->x+directions[tmp_index][0];
    tmp_y=snake->y+directions[tmp_index][1];
  }while(vis[tmp_x][tmp_y]!=0);
  tail->x=tmp_x;
  tail->y=tmp_y;
  //dir是方向，范围为0-3
  tail->dir=rand()%4;
  tail->next=NULL;
  tail->prev=snake;
  snake->next=tail;
  // 标记
  vis[tail->x][tail->y]=3;
  return snake;
}

/*
* 初始化函数,主要是vis函数赋初值（边界，小蛇，食物）
* @param Food* foodArray 待分配位置的食物
* @param int food_len  食物种类数
* @param Snake snake 待分配位置的小蛇（创建时已分配位置，此处备用）
* @param int *vis 网格的当前状态，在随机位置时进行标记
* @param int food_num  生成食物个数
* @param int wid  边界宽度
* @param int len  边界长度
* @param int num_obstacle 障碍物个数
* @return 返回1表示初始化成功，返回0表示失败
*/
extern int init_game(const Food* foodArray,int food_len,Snake snake,int (*vis)[WIDTH_BOUNDARY],int food_num,int wid,int len,int num_obstacle)
{
  // 初始化边界
  for(int i=0;i<wid;i++) {
    vis[0][i]=-2;
    vis[len-1][i]=-2;
  }
  for(int i=1;i<len-1;i++)
  {
    vis[i][0]=-3;
    vis[i][wid-1]=-3;
  }
  //生成food_num个食物
  for(int i=0;i<food_num;i++)
  {
    int tmp_x,tmp_y,tmp_category;
    //找到合适位置
    do{
      tmp_category=rand()%food_len;
      tmp_x=rand()%(wid-2)+1;
      tmp_y=rand()%(len-2)+1;
    }while(vis[tmp_x][tmp_y]!=0);
    //放置,0类对应于4。。。
    vis[tmp_x][tmp_y]=tmp_category+4;
  }
  // 生成num_obstacle个障碍物
  for(int i=0;i<num_obstacle;i++)
  {
    int tmp_x,tmp_y;
    //找到合适位置
    do{
      tmp_x=rand()%(wid-2)+1;
      tmp_y=rand()%(len-2)+1;
    }while(vis[tmp_x][tmp_y]!=0);
    vis[tmp_x][tmp_y]=-1;
  }
  return 1;
}

/*
* 打印界面函数,按照行遍历vis，根据其值打印对应图标
* @param int *vis 网格的当前状态，在随机位置时进行标记
* @param int wid  边界宽度
* @param int len  边界长度
* @param char *food_icon  食物对应图标
* @param int len_ficon  食物图标长度
* @param char *snake_icon  小蛇对应图标
* @param int len_sicon  小蛇图标长度
* @param char *boundary_icon  边界对应图标
* @param int len_bicon  边界图标宽度（默认2）
* @return 返回1表示初始化成功，返回0表示失败
*/
int printf_game(int (*vis)[WIDTH_BOUNDARY],int wid,int len,char **food_icon,int lficon,char **snake_icon,int lsicon,char **boundary_icon,int lbicon)
{
  for(int i=0;i<len;i++)
  {
    for(int j=0;j<wid;j++)
    {
      int tmp=vis[i][j];
      if(tmp==0) printf(" ");
      // 打印
      if(tmp<0&&tmp>=-3) printf("%s",boundary_icon[(-tmp)-1]);//打印上下边界和障碍物
      if(tmp>0&&tmp<4) printf("%s",snake_icon[tmp-1]);//打印蛇
      if(tmp>=4&&tmp<lficon+4) printf("%s",food_icon[tmp-4]);//打印食物
    }
    printf("\n");
  }
  return 1;
}

