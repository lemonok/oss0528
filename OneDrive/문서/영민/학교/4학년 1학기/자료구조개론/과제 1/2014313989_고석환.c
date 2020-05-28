/*
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define maxstack 100


typedef int data;
typedef struct
{
	data height[maxstack];
	data output[maxstack];
	data numtower_s;
	int top;
}stack;
typedef enum { false, true } bool;

void initstack(stack*pstack);

bool isfull(stack*pstack);

bool isempty(stack*pstack);

data peek(stack*pstack);

void push(stack*pstack, data item);

data pop(stack*pstack);

data search(stack*pstack, int tall_tower);

int main()
{

	int i, casenum, num_tower, tall_tower;
	scanf("%d", &casenum);
	stack*ptower = (stack*)malloc(sizeof(stack)*casenum);
	for (int i2 = 0; i2 < casenum; i2++)
	{
		int k = i2;
		//stack*ptower = &tower[k];
		initstack((ptower + i2));

		scanf("%d", &num_tower);
		for (i = 0; i < num_tower; i++)
		{
			(ptower + i2)->numtower_s = num_tower;
			scanf("%d", &tall_tower);
			push((ptower + i2), tall_tower);
		}
		for (i = num_tower; i > 0; i--)
			(ptower + i2)->output[i - 1] = search((ptower + i2), i);
		(ptower + i2)->top = num_tower - 1;
		i2 = k;


	}

	for (i = 0; i < casenum; i++)
	{
		//stack*ptower = &tower[i];

		int j = (ptower + i)->top;
		for (int k = 0; k < j + 1; k++)
		{
			printf("%d ", (ptower + i)->output[k]);
		}

		printf("\n");
	}
	_getch();


}

void initstack(stack*pstack)//stack�� �������� pstack�� �����͸� ���ڷ� ����
{
	pstack->top = -1;//���߿� array indexǥ���� ���ϰ� �ϱ� ���ؼ�
}

bool isfull(stack*pstack)
{
	return pstack->top == maxstack - 1;
}

bool isempty(stack*pstack)
{
	return pstack->top == -1;
}

data peek(stack*pstack)
{
	if (isempty(pstack))//���� stack�� ��������� isempty�� false�� ��ȯ��
		exit(1);
	return pstack->height[pstack->top];
	//stack�� height�׸� array�� top�� ��ġ�� �ִ� �׸��� ��ȯ
}

void push(stack*pstack, data item)//stack�ּҿ� push�� ����, ��2���� ����
{
	if (isfull(pstack))
		exit(1);//stack�� ����á���� Ȯ��
	pstack->height[++(pstack->top)] = item;//top�� ���ڸ� ���� �ø��� ���ڸ��� item�� push��
}

data pop(stack*pstack)
{
	if (isempty(pstack))
		exit(1);
	return pstack->height[pstack->top--];
}

data search(stack*pstack, int i)
{
	int num, comp_num, m;
	pstack->top = i - 1;
	num = pop(pstack);
	for (m = i; m > 0; m--)
	{
		if (pstack->top == -1)
			continue;
		comp_num = peek(pstack);
		if (num <= comp_num)
			return pstack->output[i - 1] = pstack->top + 1;
		else if (num > comp_num)
			pstack->top--;
		else if (pstack->top == -1)
			return pstack->output[i - 1] = 0;
	}
	return 0;
}
*/