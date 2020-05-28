#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define M 10
#define N 10
#define max(X,Y) ((X) > (Y) ? (X) : (Y))

int row[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
int col[] = { -1, 0, 1, -1, 1, -1, 0, 1 };
int ch;
int max_length;

bool isValid(int x, int y)
{
	return (x >= 0 && x < M && y >= 0 && y < N);
}

int findMaxLength2(int mat[][N], int x, int y, int previous)
{

	if (!isValid(x, y) || previous + 1 != mat[x][y])
		return 0;

	int max_length = 0;

	for (int k = 0; k < 8; k++)
	{
		int len = findMaxLength2(mat, x + row[k], y + col[k], mat[x][y]);
		max_length = max(max_length, 1 + len);
	}

	return max_length;
}

int findMaxLength(int mat[][N], int ch)
{
	int max_length = 0;

	for (int x = 0; x < M; x++)
	{
		for (int y = 0; y <N; y++)
		{
			if (mat[x][y] < mat[x+1][y+1] )
			{
				// recur for all 8 adjacent cells from current cell
				for (int k = 0; k < 8; k++)
				{
					// visit position (x + row[k], y + col[k]) and find maximum length from that path
					int len = findMaxLength2(mat, x + row[k], y + col[k], ch);

					// update the length of longest path if required
					max_length = max(max_length, 1 + len);
				}
			}
		}
	}

	return max_length;
}

int main()
{
    printf("Matrix: ");
    int mat[][N] =
	{
		{ 17,19,38,67,85,77,67,3,97,92 },
		{ 12,4,34,56,12,45,74,11,1,2 },
		{ 9,19,38,67,85,77,67,3,97,92 },
		{ 10,4,34,6,12,4,74,11,1,2 },
		{ 7,19,38,67,15,77,67,3,9,43 }
	};
    printf("\n");

    printf("Starting at: ");
    scanf("%d",&ch);

    printf("Maximum length of the path:");
    findMaxLength(mat,ch);

	printf("The longest path:");
	printf("\n");

	printf("Program ended with exit code:");

	return 0;
}

