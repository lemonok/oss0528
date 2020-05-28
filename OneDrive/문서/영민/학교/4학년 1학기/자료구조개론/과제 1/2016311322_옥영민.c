#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<iostream>
#define MAX_STACK 100

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
    int height_tower[100];

}hnum;
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
void laser()
{
	int n;
	cin >> n;
	int tower[500010];
	int ans[500010];
	stack<pair<int, int> > s;
	for (int i = 0; i < n; i++) {
		scanf("%d", &tower[i]);
		ans[i] = 0;
	}
	s.push(make_pair(0, tower[0]));

	for (int i = 1; i < n; i++) {
		while (1)
		{
			if (s.empty()) {
				s.push(make_pair(i, tower[i]));
				break;
			}
			else if (s.top().second < tower[i]) {
				s.pop();
			}
			else {
				ans[i] = s.top().first + 1;
				s.push(make_pair(i, tower[i]));
				break;
			}
		}
	}

	for (int i = 0; i < n; i++)
		printf("%d ", ans[i]);
}
int main()
{
    int cnum;
    scanf("%d",&cnum);
    hnum input[cnum];
    for(int i=0;i<cnum;i++)
    {
        scanf("%d",&(input[i].num_tower));
        for(int j=0;j<input[i].num_tower;j++)
        {
            scanf("%d",&(input[i].height_tower[j]));
        }
    }

    laser(&input,test_num);
    return 0;
}

