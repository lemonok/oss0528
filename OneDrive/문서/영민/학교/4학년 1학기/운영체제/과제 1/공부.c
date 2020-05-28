#include <stdio.h>
#include <stdlib.h>

void sort(int list[],int n)
{
    int i, j, min;
    for (i=0;i<n-1;i++){
        min=i;
        for (j=i+1;j<n;j++)
            if(list[j]<list[min])
            min=j;
        SWAP(list[i],list[min]);
    }
}

int values(Process *pro,int n){
    int pid;
    int option;
    int burst_time;
    int waiting_time;
	int turnaround_time;

    FILE *fp;
    int j,found;
    fp=fopen("input.txt","r");
    int i=n;

    pro[i].pid=i+1;
    pro[i].burst_time=burst_time;

    fscanf(fp,"\r\n %d %d %d %d %d", pro[i].pid, pro[i].option, pro[i].burst_time,pro[i].waiting_time,pro[i].turnaround_time);
    fclose(fp);


    	FILE *fp;
	fp=fopen("input.txt","r");
	int counter = 1;
	char line[1000];
	Process *ready_queue;
	int value[20][20];

    while(fgets(line, sizeof(line), fp) != NULL){
        counter++;
        //fscanf( fp, "%d %d %d %d %d \n", &pid, &option, &burst_time, &waiting_time,&turnaround_time,line);
    }


}
