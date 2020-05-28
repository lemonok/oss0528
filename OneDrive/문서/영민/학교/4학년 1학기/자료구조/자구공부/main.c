#include <stdio.h>
#define FALSE 0
#define TRUE 1

int *Stack;
int Size;
int Top;

void InitStack(int aSize)
{
    Size=aSize;
    Stack=(int *)malloc(Size *sizeof(int));
    Top=-1;
}

void FreeStack()
{
    free(Stack);
}

void Push(int data)
{
    if (Top<Size-1){
        Top++;
        Stack[Top]=data;
    }else{
    return FALSE;
    }
}

int Pop()
{
    if(Top>=0){
        return Stack[Top--];
    }else{
    return -1;
    }
}

void main()
{
    InitStack(256);
    Push(7);
    Push(0);
    Push(3);
    printf("%d \n",Pop());
    printf("%d \n",Pop());
    FreeStack();
}
