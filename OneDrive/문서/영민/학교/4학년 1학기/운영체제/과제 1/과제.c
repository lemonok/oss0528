#include <stdio.h>
#include <stdlib.h>
#define MAX 100
#define MAX_PROCESS 100

int main()
{
    int pid;
    int burst_time;
    int waiting_time;
	int turnaround_time;

    FILE *fp;
    char line[100];
    fp=fopen("input.txt","r");
    fscanf(fp,"%*d %*d %d %d",&pid, &burst_time, &waiting_time, &turnaround_time);

    int counter = 1; /*Number of lines*/

    while(fgets(line, sizeof(line), fp) != NULL){
        counter++;
    }

    printf("Number of lines in the file is %i", counter);

    return 0;
}

