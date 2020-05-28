#include <stdio.h>

int a[2][3] = { {10,20,30},{11,22,33} };
int i, k;

void main()
{
	for (i = 0; i < 2; i++)
		for (k = 0; k < 3; k ++)
			prinf("%d \n", a[i][k]);
}