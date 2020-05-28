#include<stdio.h>

void ReversePrint(char* s, int len)
{
	Stack stack;
	char ch;

	InitStack(&stack);// Make a stack empty.
	for (int i = 0; i < len; i++)	// Push characters.
		Push(&stack, s[i]);

	while (!IsEmpty(&stack))// Pop characters.
	{
		ch = Peek(&stack);
		printf("%c", ch);
		Pop(&stack);
	}
}
