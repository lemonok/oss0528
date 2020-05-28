#include<stdio.h>
#include<stdlib.h>

#define LEN 7 //stack 길이 정의
#define TRUE 1
#define FALSE 0

typedef int Data; //데이터형(가변성)

// 스택 구조체 정의해주기
typedef struct array_stack {
	Data Arr[LEN];
	int top;
} array_stack;

// 스택 명명 변환
typedef array_stack Stack;




// 스택 초기 설정
void stack_init(Stack *pstack) {
	pstack->top = -1;
}


//스택 데이터 삽입
void Push(Stack *pstack, Data Data) {
	pstack->top += 1;
	pstack->Arr[pstack->top] = Data;
}



// top index의 데이터 확인
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
