#include<stdio.h>
#include<stdlib.h>
#include<string>

int top = -1;
int cnum, tnum;
int *stack;
int size;
int p;

typedef struct()
{
	int top();
}Tower;

void init_stack(int size)
{
	stack = (int*)malloc(sizeof(int));
	p = -1;
}

void laser() 
{
	int n;
	cin >> n;
	int tower[500010];
	int ans[500010];
	stack<pair<int, int> > s;
	for (int i = 0; i < n; i++) {
		scanf("%d", &Tower[i]);
		ans[i] = 0;
	}
	s.push(make_pair(0, Tower[0]));

	for (int i = 1; i < n; i++) {
		while (1)
		{
			if (s.empty()) {
				s.push(make_pair(i, Tower[i]));
				break;
			}
			else if (s.top().second < Tower[i]) {
				s.pop();
			}
			else {
				ans[i] = s.top().first + 1;
				s.push(make_pair(i, Tower[i]));
				break;
			}
		}
	}

	for (int i = 0; i < n; i++)
		printf("%d ", ans[i]);
}

void main()
{
	//number of test case T
	int i;
	scanf_s("%d", &cnum);

	i = 0;

	//number of towers
	while (i < cnum)
	{
		int j;
		scanf_s("%d", &tnum);

		init_stack(tnum);
		
		//heights of towers
		while (j < tnum)
		{
			scanf("%d", &(tnum.Tower[j]));

			j++;
			
			return 0;
		}
		i++;
	}

	laser();
	return 0;
}