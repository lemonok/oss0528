#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_STACK 1000

typedef enum{false,true}bool;
typedef int Data;
typedef struct
{
    Data items[MAX_STACK];
    int top;
}Stack;
typedef struct n
{
    int num_tower;
    int height_tower[1000];

}num_nnh;
void InitStack(Stack *pstack)
{
    pstack->top = -1;
}
bool IsFull(Stack *pstack)
{
    return pstack->top == MAX_STACK-1;
}
bool IsEmpty(Stack *pstack)
{
    return pstack->top == -1;
}
Data Peek(Stack *pstack)
{
    if(IsEmpty(pstack))
    {exit(1);
}
    return pstack->items[pstack->top];
}
void Push(Stack *pstack, Data item)
{
    if(IsFull(pstack))
    {
        exit(1);
    }
    pstack->items[++(pstack->top)]=item;
}
void pop(Stack *pstack)
{
    if(IsEmpty(pstack))
    {
        exit(1);
    }
    --(pstack->top);
}
void laserfunction(num_nnh *n, int test_num)
{
    Stack stack[test_num];
    for(int i=0;i<test_num;i++)
    {
        InitStack(&stack[i]);
    }
    int num;int k=0;
    for(int a=0;a<test_num;a++)
    {
      for(int i=((n+a)->num_tower)-1;0<i;i--)
        {   k=0;
        for(int j=i-1;-1<j;j--)
        {
            if((n[a].height_tower[i])<=(n[a].height_tower[j]))
            {
                Push(&stack[a],j+1);
                k=1;
                break;
            }
        }
        if(k==0)
        {
            Push(&stack[a],0);
        }
    } Push(&stack[a],0);
    }

    for(int a=0;a<test_num;a++)
    {
    while(!IsEmpty(&stack[a]))
    {
        num=Peek(&stack[a]);
        printf("%d ",num);
        pop(&stack[a]);
    }
    printf("\n");
    }
}
int main()
{
    int test_num;
    scanf("%d",&test_num);
    num_nnh input[test_num];
    for(int i=0;i<test_num;i++)
    {
        scanf("%d",&(input[i].num_tower));
        for(int j=0;j<input[i].num_tower;j++)
        {
            scanf("%d",&(input[i].height_tower[j]));
        }
    }

    laserfunction(&input,test_num);
    return 0;
}
