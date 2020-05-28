#include <stdio.h>
#include <stdlib.h>
#define MAX_SIZE 8

void merge(int list[],int left, int mid, int right)
{
    int i,j,k,l;
    int i=left;
    int j=mid+1;
    int k=left;

    while(i<=mid && j<=right){
        if(list[i]<=list[j])
            sorted[k++]=list[i++];
        else
            sorted[k++]=list[j++];
    }

    if(i>mid){
        for(l=j;l<=right;l++)
            sorted[k+]=list[l];
    }

    else{
        for(l=i;l<=mid;l++)
            sorted[k++]=list[l];
    }

    for(l=left;l<=right;l++){
        list[l]=sorted[l];
    }
}

void main()
{
    int i;
    int n = MAX_SIZE;
    int list[n] = {8, 5, 6, 2, 4};

    insertion_sort(list,n);

    for(i=0;i<n;i++){
        printf("%d \n",list[i]);
    }

    return 0;
}
