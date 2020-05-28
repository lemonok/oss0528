#include <stdio.h>
#include <stdlib.h>

int main()
{
    int num=2;
    int task;
    int n;
    int min=0;
    int max=0;

    FILE *fptr;
    fptr = fopen("3n1.inp", "r");
    if (fptr == NULL)
    {
        printf("error");
        exit(0);
    }

    fscanf(fptr,"%d",&task);

    if(task<0 || task>63){
        printf("error");
        return 0;
    }


    while(num<=65535){
        int k=0;
        n = num;
        while(n!=1){
            k++;
        if(n%2==0){
            n/=2;
        }else if(n%2==1){
            n = 3*n+1;
        }
        }

        if(k==task){
            if(min==0){
                min = num;
            }else{
            max = num;
            }
        }
        num++;
    }
    FILE *writer;
    writer = fopen("3n1.out", "w");
    fprintf(writer,"%d\n",min);
    fprintf(writer,"%d\n",max);

    return 0;

}
