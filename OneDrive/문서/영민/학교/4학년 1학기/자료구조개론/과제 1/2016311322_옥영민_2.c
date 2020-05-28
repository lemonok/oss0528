#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE    1000
#define DIR_SIZE    6
#define QUEUE_SIZE  1000000

#define TOMATO_RIPEN    1
#define TOMATO_NOT_YET  0
#define TOMATO_EMPTY    -1



    int tomato[MAX_SIZE][MAX_SIZE];

    int M, N;
    int m, n;
    int i, j, k;

    int y_dir[DIR_SIZE] = { 0, 1, 0, 0, -1, 0 };
    int x_dir[DIR_SIZE] = { 1, 0, 0, -1, 0, 0 };
    int dir = 0;

    int queue_y[2][QUEUE_SIZE];
    int queue_x[2][QUEUE_SIZE];
    int queue_head[2] = { 0 };
    int queue_tail[2] = { 0 };
    int queue_cnt[2] = { 0 };
    int queue_switch_index = 0;

    int cnt_ripen = 0;
    int cnt_not_yet = 0;
    int cnt_empty = 0;

    int cur_y, cur_x;
    int day = 0;
    int tc;

int main(void)
{
    scanf("%d",&tc);
    for(int a=0;a<tc;a++)
    {
        scanf("%d %d",&M, &N);
            for (n = 0; n < N; n++)
            {
                for (m = 0; m < M; m++)
                {
                    scanf("%d", &tomato[n][m]);

                    if (tomato[n][m] == TOMATO_RIPEN)
                    {
                        cnt_ripen++;
                        queue_y[queue_switch_index][queue_tail[queue_switch_index]] = n;
                        queue_x[queue_switch_index][queue_tail[queue_switch_index]] = m;
                        queue_tail[queue_switch_index]++;
                        queue_cnt[queue_switch_index]++;
                    }
                    else if (tomato[n][m] == TOMATO_EMPTY)
                    {
                        cnt_empty++;
                    }
                    else
                    {
                        cnt_not_yet++;
                    }
                }
            }

            if (cnt_not_yet == 0)
            {
                printf("0");
                return 0;
            }

            while (queue_cnt[queue_switch_index] > 0)
            {
                while (queue_cnt[queue_switch_index] > 0)
                {
                    cur_y = queue_y[queue_switch_index][queue_head[queue_switch_index]];
                    cur_x = queue_x[queue_switch_index][queue_head[queue_switch_index]];
                    queue_head[queue_switch_index]++;
                    queue_head[queue_switch_index ^ 1] %= QUEUE_SIZE;
                    queue_cnt[queue_switch_index]--;
                    for (dir = 0; dir < DIR_SIZE; dir++)
                    {
                        if ((cur_y + y_dir[dir] >= 0 && cur_y + y_dir[dir] < N)
                            && (cur_x + x_dir[dir] >= 0 && cur_x + x_dir[dir] < M))
                        {
                            if (tomato[cur_y + y_dir[dir]][cur_x + x_dir[dir]] == TOMATO_NOT_YET)
                            {
                                tomato[cur_y + y_dir[dir]][cur_x + x_dir[dir]] = TOMATO_RIPEN;
                                queue_y[queue_switch_index ^ 1][queue_tail[queue_switch_index ^ 1]] = cur_y + y_dir[dir];
                                queue_x[queue_switch_index ^ 1][queue_tail[queue_switch_index ^ 1]] = cur_x + x_dir[dir];
                                queue_tail[queue_switch_index ^ 1]++;
                                queue_tail[queue_switch_index ^ 1] %= QUEUE_SIZE;
                                queue_cnt[queue_switch_index ^ 1]++;
                                cnt_ripen++;
                                cnt_not_yet--;
                            }
                        }
                    }
                }
                queue_head[queue_switch_index] = 0;
                queue_tail[queue_switch_index] = 0;
                queue_cnt[queue_switch_index] = 0;
                queue_switch_index ^= 1;
                day++;
                if (cnt_not_yet == 0) break;
            }
            if (cnt_not_yet > 0)
            {
                printf("-1");
                return 0;
            }

            printf("%d", day);

            return 0;
    }
}
