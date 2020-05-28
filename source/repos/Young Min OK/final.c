#include <stdio.h>

int main()
{
	int value;

	do {
		scanf("%d", &value);
		printf("%d ", value);
		// value값을 parameter로하는 함수호출부분
	} while (getc(stdin) == ' ');

	return 0;
}