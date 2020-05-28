#include <stdio.h>
#include <stdlib.h>
#define MAX_STACK 100

typedef enum{fals,true}bool;
typedef int Data;
typedef sturct{
    Data items[MAX_STACK];
    int top;
}Stack;

bool IsParanBalanced(char* exp, int len)
{
    Stack stack;
    InitStack(&stack);
    for(int i=0;i<len;i++){
        if(exp[i]=='(')
           Push(&stack,exp[i]);
        else if(exp[i]==')'){
            if(IsEmpty(&stack))
                return false;
            else
                Pop(&stack);
        }
    }
    if(IsEmpty(&stack))
        return true;
    else
        return false;
}
