#include <stdio.h>

int main()
{
	int value;

	do {
		scanf("%d", &value);
		printf("%d ", value);
		// value���� parameter���ϴ� �Լ�ȣ��κ�
	} while (getc(stdin) == ' ');

	return 0;
}