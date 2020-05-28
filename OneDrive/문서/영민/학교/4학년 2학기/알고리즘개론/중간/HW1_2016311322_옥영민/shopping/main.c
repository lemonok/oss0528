#include <stdio.h>
#include <stdlib.h>

int main() {
    char str[1000000];
    int j=0,k=0,res=0,i=0,x=0,y=0,t=0,flag=0,len=0,discount=0;
    char buff[10];
    int a[20001];
    FILE *fptr;
    FILE *fpt;
    int num;
    //파일을 못찾을경우 null을 반환
    if ((fptr = fopen("shopping.inp", "r")) == NULL) {
        printf("Error! opening file");
        exit(1);
        }

    // reads text until newline is encountered
    fscanf(fptr, "%[^\t]", str);

    while(str[j])
    {
        if((str[j]>='0')&&(str[j]<='9'))
    {
k=0;
while((str[j]!=' ')&&(str[j]!='\0'))
{
buff[k]=str[j++];
k++;
}
buff[k]=0;
res=atoi(buff);
//Store this result to an array
if(flag==0){
len=res;
flag=1;
}
else{
   a[i]=res;
   i=i+1;}
}
        j++;

    }

    for(x=0;x<i;x++){
        for(y=x+1;y<i;y++){
            if(a[x]<a[y]){
                t=a[x];
                a[x]=a[y];
                a[y]=t;
            }
        }
    }
    for(x=2;x<i;x=x+3){
        discount=discount+a[x];
    }
    //결과값을 파일로 저장함
    fclose(fptr);
    fpt= fopen("shopping.out", "w");
    fprintf(fpt,"%d",discount);
    fclose(fpt);

    return 0;
}
