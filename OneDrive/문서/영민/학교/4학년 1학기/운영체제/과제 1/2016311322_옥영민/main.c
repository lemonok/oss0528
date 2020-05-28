#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

typedef struct _process{
    int pro_num, cpu_time,arr_t,pri,wait_t,ta_t,rem_t,st,ft;
}process;

int select_menu,cur_menu,ent_key;
int getkeyboard();
void key_check(int n);
void print();

int process_fcfs(process *pro,int n);
int process_srt(process *pro, int n);
int process_pri(process *pro,int n);
int process_rr(process *pro,int n, int Q);
int process_sjf(process *pro, int n);
int process_generate(process *pro, int n);
void at_sort(process *pro, int n);
void resort(process *pro, int n);

void print(){
    switch(select_menu){
        case 1:
            system("clear");
                printf("case 1");
        break;
        case 2:
            system("clear");
                printf("case 2");
        break;
        case 3:
            system("clear");
                printf("case 3");
        break;
        case 7:
            system("clear");
                printf("case 7");
        break;
        case 8:
            system("clear");
                printf("case ");
        break;
    }
}

void key_check(int n){
    switch(n){
        case 65://up-1
        if(select_menu==1)
        {
            select_menu=1;
        }
        else
        {
            select_menu--;
        }
            system("clear");
        print(select_menu);
            ent_key=0;
        break;

    case 66://down+1
        if(select_menu==8)
        {
            select_menu=8;
        }
        else
        {
            select_menu++;
        }
            system("clear");
        print(select_menu);
            ent_key=0;
        break;

    case 10://enter
        if(select_menu==7){
            cur_menu=select_menu;
        ent_key=1;
        }
        else{
            print(select_menu);
    cur_menu=select_menu;
        ent_key=1;
        }
    break;
    }
}

int getkeyboard(){
    int ch;
    ch = getchar();
    return ch;
}

void at_sort(process *pro,int n){
    process temp;
    int i,j;
    for(i=n-1; i>0; i--){
        for(j=0; j<i; j++){
            if(pro[j].arr_t>pro[j+1].arr_t){
                temp=pro[j+1];
                pro[j+1]=pro[j];
                pro[j]=temp;
            }
            else if(pro[j].arr_t==pro[j+1].arr_t&&pro[j].pro_num>pro[j+1].pro_num){
                temp=pro[j+1];
                pro[j+1]=pro[j];
                pro[j]=temp;
            }
        }
    }
}

int process_fcfs(process *pro,int n){
    int temp;
    int wt=0;
    int i,j,k=0;

    for(i=0;i<n;i++){
        temp=0;
        for(j=k;j<i;j++){
            temp=temp+pro[j].cpu_time;
        }
        wt=temp-pro[i].arr_t+pro[k].arr_t;

        if(wt<=0){
            k=i;
            pro[i].wait_t=0;
            pro[i].ta_t=pro[i].cpu_time+pro[i].wait_t;
        }
        else{
            pro[i].wait_t=wt;
            pro[i].ta_t=pro[i].cpu_time+pro[i].wait_t;
        }
    }
}

int process_srtn(process *pro,int n){
    int remain,min,now_p,i,temp[150];
    int now,wt=0;

    remain=n;
    now=pro[0].arr_t;
    while(remain>0){
        min=5000;
        for(i=0;pro[i].arr_t<=now&&i<n;i++){
            if(pro[i].rem_t<min&&pro[i].rem_t>0){
                now_p=i;
                min=pro[i].rem_t;
            }
        }
        now++;
        if(temp[0]!=pro[0].cpu_time){
            for(i=0;i<n;i++){
                temp[i]=pro[i].rem_t;
            }
        }
        pro[now_p].rem_t--;
        if(pro[now_p].rem_t==0){
                pro[now_p].wait_t=now-pro[now_p].arr_t-pro[now_p].cpu_time;
                pro[now_p].ta_t=pro[now_p].cpu_time+pro[now_p].wait_t;
                wt=wt+pro[now_p].wait_t;
                remain--;
    }
    for(i=0;i<n;i++){
        pro[i].rem_t=temp[i];
    }
}

int process_generate(process *pro,int n){
    FILE *fp2;
    int j,found;
    fp2=fopen("input.txt","a+");
    int i=n;
    int bt=(rand()%25)+1;

    pro[i].pro_num=i+1;
    pro[i].cpu_time=bt;

    while(1){
        pro[i].pri=rand()%50;
        found=0;
        for(j=0;j<i;++j){
            if(pro[j].pri==pro[i].pri){
                found=1;
                break;
            }
        }
        if(!found)break;
    }
    while(1){
        pro[i].arr_t=rand()%50;
        found=0;
        for(j=0;j<i;++j){
            if(pro[j].arr_t==pro[i].arr_t){
                found=1;
                break;
            }
        }
        if(!found)break;
    }
    fprintf(fp2,"\r\n%d%d%d%d",pro[i].pro_num,pro[i].cpu_time,pro[i].arr_t,pro[i].pri);
    fclose(fp2);
}

int main(){
    int i,n,Q;
    int index=0;
    float tat;

    process *ready_queue;

    FILE *fp;
    fp=fopen("input.txt","r");

    while(!feof(fp)){
        fscanf(fp,"%d",&ready_queue[i].pro_num);
        fscanf(fp,"%d",&ready_queue[i].cpu_time);
        fscanf(fp,"%d",&ready_queue[i].arr_t);
        fscanf(fp,"%d",&ready_queue[i].pri);
        ready_queue[i].rem_t=ready_queue[i].cpu_time;
        index=index+1;
        i++;
    if(feo(fp)!=0)
        break;
    }
    fclose(fp);
    n=index;
    system("clear");
    select_menu=1;
    cur_menu=select_menu;

    printf("menu");
    while(1){
        key_check(getkeyboard());
        float tat=0.0;
        if(ent_key==1){
            switch(cur_menu){
        case 1:
        printf("menu");
        printf("P# BT AT Pri \n");
        for(i=0;i<n;i++){
            printf("%d %d %d %d \n",ready_queue[i].pro_num, ready_queue[i].cpu_time,ready_queue[i].arr_t,ready_queue[i].pri);
        }
        printf("===\n");
        continue;
        break;

        case 2:
        printf("2\n");
        process_generate(ready_queue,n);
        printf("\n\n ==�����Ϸ�==\n");
        n++;
        continue;
        break;

        case 3:
        at_sort(ready_queue,n);
        process_fcfs(ready_queue,n);
        resort(ready_queue,n);

        printf("3");
        printf("P# BT AT Pri WT TAT \n");
        for(i=0;i<n;i++){
            tat=tat+ready_queue[i].ta_t;
            printf("%d %d %d %d %d",ready_queue[i].pro_num, ready_queue[i].cpu_time,ready_queue[i].arr_t,ready_queue[i].pri);
        }
        printf("average TT: %.2f \n",tat/n);
        printf("== \n");
        continue;
        break;

        case 8:
        printf("\n ==EXIT");
        exit(0);
        break;

        }
    }
    else continue;
    break;
    }
}
}
