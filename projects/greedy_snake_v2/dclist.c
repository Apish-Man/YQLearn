/*************************************************************************
  > File Name:    snake.c
  > Author:       魏新强
  > Description:  
  > Created Time: 2025年06月03日 星期二 20时22分00秒
 ************************************************************************/

#include "dclist.h"

/**
 * 创建节点（私有函数，只能当前文件类访问）
 * @param data 待插入的目标数据
 * @return 成功返回新节点指针，失败返回NULL
 */
static NODE *__dclist_create_node(DATA data)
{
  // 创建一个新节点
  NODE *p = (NODE*)malloc(sizeof(NODE));
  if (!p) return NULL;

  // 初始化节点
  p->data = data;
  p->prev = p->next = p; // 循环链表，单节点的前驱后继都指向自身

  return p;
}

/**
 * 创建节点后，维护节点的指针
 * @param new_node 新创建的目标节点
 * @param prev     目标节点的上一个节点
 * @param next     目标节点的下一个节点
 */
static void __dclist_add(NODE *new_node, NODE *prev, NODE *next)
{
  new_node->prev = prev; // 目标节点的前驱指向prve节点   
  new_node->next = next; // 目标节点的后继指向next节点
  prev->next = new_node; // prev节点的后继指向目标节点
  next->prev = new_node; // next节点的前驱指向目标节点
}

/**
 * 向链表添加节点数据
 * @param head：待操作的链表
 * @param data：待插入的数据
 * @return 成功返回0，失败返回-1
 */
int dclist_add(NODE **head, DATA data)
{
  // 空链表
  if (*head == NULL)
  {
    // 创建一个新节点，并作为链表的头节点
    *head = __dclist_create_node(data);

    return (*head) ? 0 : -1;
  }
  // 非空链表
  NODE *new_node = __dclist_create_node(data);
  if(!new_node) return -1;

  // 更改指针指向（尾插法）
  __dclist_add(new_node,(*head)->prev,*head);

  return 0;

}

/**
 * 向链表添加节点数据,尾插法
 * @param head：待操作的链表
 * @param node：待插入的节点
 * @return 成功返回0，失败返回-1
 */
int dclist_tail_add(NODE **head, NODE *node)
{
  // 空链表
  if (*head == NULL)
  {
    // 新节点为头节点
    *head=node;
    node->next=node;
    node->prev=node;
  }else
  {
  // 更改指针指向（尾插法）
    NODE* tail=(*head)->prev;
    tail->next=node;
    node->prev=tail;
    node->next=(*head);
    (*head)->prev=node;
  }
  return 0;
}

/**
 * 向链表添加节点数据,头插法
 * @param head：待操作的链表
 * @param node：待插入的节点
 * @return 成功返回0，失败返回-1
 */
int dclist_head_add(NODE **head, NODE *node)
{
  // 空链表
  if (*head == NULL)
  {
    // 新节点为头节点
    *head=node;
    node->next=node;
    node->prev=node;
  }else
  {
  // 更改指针指向（头插法）
    NODE* tail=(*head)->prev;
    tail->next=node;
    node->prev=tail;
    node->next=(*head);
    (*head)->prev=node;
    (*head)=node;
  }
  return 0;
}

/**
 * 在 ncurses 窗口中调试打印双向循环链表
 * @param win: ncurses 窗口指针
 * @param head: 待操作的链表
 * @param title: 调试信息的标题
 */
void dclist_showInWindow(WINDOW *win, const NODE *head, const char *title) {
    if (!head) {
        wprintw(win, "%s: List is NULL\n", title);
        return;
    }

    // 保存原始光标位置
    int orig_y, orig_x;
    getyx(win, orig_y, orig_x);
    
    // 显示标题
    wprintw(win, "%s:\n", title);
    
    // 显示正序
    wprintw(win, "Forward: ");
    const NODE *current = head;
    int count = 0;
    int max_y, max_x;
    getmaxyx(win, max_y, max_x);
    
    do {
        // 检查是否超出窗口边界
        int cur_y, cur_x;
        getyx(win, cur_y, cur_x);
        
        // 如果接近边界，换行显示
        if (cur_x > max_x - 20) {
            wprintw(win, "\n         ");
        }
        
        wprintw(win, "(%d,%d) ", current->data.i, current->data.j);
        
        // 添加方向指示器
        if (current->next != head) {
            wprintw(win, "-> ");
        }
        
        current = current->next;
        count++;
        
        // 防止无限循环
        if (count > 100) {
            wprintw(win, "\n[WARN] Loop detected! Aborting display\n");
            break;
        }
    } while (current != head);
    
    // 显示逆序
    wprintw(win, "\nReverse: ");
    current = head->prev;
    count = 0;
    
    do {
        // 检查是否超出窗口边界
        int cur_y, cur_x;
        getyx(win, cur_y, cur_x);
        
        // 如果接近边界，换行显示
        if (cur_x > max_x - 20) {
            wprintw(win, "\n         ");
        }
        
        wprintw(win, "(%d,%d) ", current->data.i, current->data.j);
        
        // 添加方向指示器
        if (current->prev != head->prev) {
            wprintw(win, "<- ");
        }
        
        current = current->prev;
        count++;
        
        // 防止无限循环
        if (count > 100) {
            wprintw(win, "\n[WARN] Loop detected! Aborting display\n");
            break;
        }
    } while (current != head->prev);
    
    wprintw(win, "\n");
    
    // 恢复原始光标位置
    wmove(win, orig_y, orig_x);
    
    // 刷新窗口显示
    wrefresh(win);
}

/**
 * 遍历链表数据
 * @param head：待操作的链表
 */
void dclist_showAll(const NODE *head)
{
  const NODE *p = head;

  printf("正序输出：");
  while(p)
  {
    printf("%-6d %-6d",p->data.i,p->data.j);
    p = p->next;
    // 循环链表，考虑出口
    if (p == head) break; 
  }

  printf("\n逆序输出：");
  p = p->prev;
  while(p)
  {
    printf("%-6d %-6d",p->data.i,p->data.j);
    p = p->prev;
    // 循环链表，考虑出口
    if (p == head->prev) break; 
  }
  printf("\n");
}

/**
 * 获取链表长度
 * @param head：待操作的链表
 */
int dclist_len(const NODE *head)
{
  const NODE *p = head;
  int ans=0;

  while(p)
  {
    ans++;;
    p = p->next;
    // 循环链表，考虑出口
    if (p == head) break; 
  }
  return ans;
}

/**
 * 根据data查找对应的节点
 * @param head：待操作的链表
 * @param data：待查询的节点数据
 * @return 成功返回节点指针，失败返回NULL
 */
static NODE *__dclist_find(const NODE *head, DATA data)
{
  const NODE *p = head;
  while (p)
  {
    // 查找节点
    if (memcmp(&(p->data), &data, sizeof(DATA)) == 0)
    {
      return (NODE*)p;  
    }
    p = p->next;
    // 终止循环链表
    if (p == head)
    {
      break;
    }
  }
  return NULL;
}

/**
 * 根据old修改对应节点数据为newdata
 * @param head：待操作的链表
 * @param old：待修改的目标节点数据
 * @param newdata：修改的数据
 * @return 成功返回0，失败返回-1
 */
int dclist_update(const NODE *head, DATA old, DATA newdata)
{
  NODE *p = __dclist_find(head, old);
  if(!p) return -1;

  // 更新数据
  p->data = newdata;
  return 0;
}

/**
 * 删除链表节点
 * @param head：待操作的链表
 * @param data：待删除的节点数据
 * @return 成功返回0，失败返回-1
 */
int dclist_delete(NODE **head, DATA data)
{
  // 情景1：空链表
  if (*head == NULL) return -1;

  // 情景2：非空链表中找不到节点
  NODE *del = __dclist_find(*head, data);
  if (!del) return -1;

  // 删除已存在节点
  NODE *p = *head;
  // 删除的节点是头节点
  if (del == p)
  {
    // 情景3：唯一头节点
    if (p->next == p)
    {
      *head = NULL;
    }
    // 情景4：非唯一头节点（包含其他节点）
    else
    {
       p->prev->next = p->next;// 让原头节点的上一个节点的next指向原头节点的下一个节点
       p->next->prev = p->prev;// 让原头节点的下一个节点的prev指向原头节点的上一个节点
       // 更新头节点为原头节点的下一个节点
       *head = p->next;
    }
    free(p);
    return 0;
  }

  // 删除的节点是非头节点
  del->prev->next = del->next;
  del->next->prev = del->prev;
  free(del);

  return 0;
}

/**
 * 删除链表尾节点节点
 * @param head：待操作的链表
 * @return 成功返回0，失败返回-1
 */
int dclist_tail_delete(NODE **head)
{
  if((*head)==NULL) return -1;
  // 只有一个节点
  if((*head)->next==(*head))
  {
    free(*head);
    (*head)=NULL;
    return 0;
  }
  // 多个节点
  NODE *tail=(*head)->prev;
  (*head)->prev=tail->prev;
  tail->prev->next=(*head);
  free(tail);
  return 0;
}

/**
 * 销毁链表
 * @param head：待销毁的链表
 */
void dclist_destroy(NODE **head)
{
  // 空链表
  if (!(*head)) return;

  // 指针尾随依次回收
  NODE *p = *head, *q = NULL;

  // 循环链表回收期，需要将循环链表改造为非循环链表
  p->prev->next = NULL;  // 将尾节点的后继指针置空
  p->prev = NULL;        // 将头节点的前驱指针置空

  while(p)
  {
    q = p;
    p = p->next;
    free(q);
  }
  // 重置头指针
  *head = NULL;
}