#include<stdio.h>
#include<stdlib.h>

#define LEN 7 //stack ���� ����
#define TRUE 1
#define FALSE 0

typedef int Data; //��������(������)

// ���� ����ü �������ֱ�
typedef struct array_stack {
	Data Arr[LEN];
	int top;
} array_stack;

// ���� ��� ��ȯ
typedef array_stack Stack;




// ���� �ʱ� ����
void stack_init(Stack *pstack) {
	pstack->top = -1;
}


//���� ������ ����
void Push(Stack *pstack, Data Data) {
	pstack->top += 1;
	pstack->Arr[pstack->top] = Data;
}



// top index�� ������ Ȯ��
Data peek(Stack *pstack) {
	pstack->Arr[pstack->top];
}

int main(void)
{
	int Cnum, Tnum, Hnum, sum = 0;

	printf("type number of test case T: ", Cnum);
	scanf("%d", &Cnum);

	for (Tnum = 0; Tnum < Cnum; Tnum++) {
		printf("type number of towers: ", Tnum);
		scanf("%d", &Tnum);

		for (Hnum = 0; Hnum < Tnum; Hnum++) {
			printf("type heights of towers: ", Hnum);
			scanf("%d", &Hnum);

			Stack stack;
			stack_init(&stack);

			Push(&stack, Hnum);


		}
	}
	

}
